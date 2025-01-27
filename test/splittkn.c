#include "hookapi.h"

#define DONE(x) accept(SBUF(x), __LINE__)
#define NOPE(x) rollback(SBUF(x), __LINE__)
#define GUARD(maxiter) _g(__LINE__, (maxiter)+1)

int64_t hook(uint32_t reserved ) {

    uint8_t main_acc[20];
    // Get the main account from the hook params
    if(hook_param(SBUF(main_acc), "M", 1) != 20)
        NOPE("Forwarder: Error: Main Account not set");

    uint8_t drop_acc[20];
    // Get the drop account from the hook params
    if(hook_param(SBUF(drop_acc), "D", 1) != 20)
        NOPE("Forwarder: Error: Drop Account not set");

    TRACEHEX(main_acc);
    TRACEHEX(drop_acc);

    uint8_t hook_acc[20];
    // Get the hook account
    hook_account(SBUF(hook_acc));

    uint8_t account[20];
    // Get the originating account of the transaction
    otxn_field(SBUF(account), sfAccount);

    // If the transaction is outgoing from the hook account, accept it
    if(BUFFER_EQUAL_20(hook_acc, account))
        DONE("Outgoing Transaction");

    otxn_slot(1); 
    slot_subfield(1, sfAmount, 1); 
    int64_t txn_amount = slot_float(1);
    if(slot_type (1, 1) != 0) 
        NOPE("Invalid Issued Currency.");    

    trace_float(SBUF("Transaction Amount: "), txn_amount);

    // If the transaction is Xah accept the payment
    uint8_t amount[48];
    // Get the transaction amount
    if (otxn_field(SBUF(amount), sfAmount) != 48)
        DONE("Xah Transaction, accepted!");

    // Parse the amount field for currency code, value amount, and issuer address
    uint8_t currency_code[20];
    uint8_t issuer[20];

    // Manually copy the bytes for currency code (next 20 bytes)
    for (int i = 0; GUARD(20), i < 20; i++) {
        currency_code[i] = amount[i + 8];
    }

    // Manually copy the bytes for issuer address (last 20 bytes)
    for (int i = 0; GUARD(20), i < 20; i++) {
        issuer[i] = amount[i + 28];
    }

    TRACEHEX(currency_code);
    TRACEHEX(issuer);

    // Reserve space for two emitted transactions
    // Reserve space for two emitted transactions
etxn_reserve(2);

// Convert the floating point number to an integer with the correct number of decimal places
int64_t txn_amount_drops = float_int(txn_amount, 6, 1); 
//trace_num(SBUF("Transaction Amount (Drops): "), txn_amount_drops);

// Convert drops to tkn for easier splitting
int64_t txn_amount_tkn = txn_amount_drops / 1000000;
trace_num(SBUF("Transaction Amount (XRM): "), txn_amount_tkn);

// Split the amount
uint64_t amount1 = txn_amount_tkn * 10 / 100; // 10% to main_acc
uint64_t amount2 = txn_amount_tkn * 10 / 100; // 10% to drop_acc
uint64_t remaining_amount = txn_amount_tkn - amount1 - amount2; // 80% remains in hook wallet

// Log the split amounts
trace_num(SBUF("Amount to Main Account: "), amount1);
trace_num(SBUF("Amount to Drop Account: "), amount2);
trace_num(SBUF("Remaining Amount: "), remaining_amount);

// Issued asset revert
int64_t main_amount = amount1 * 1000000;
int64_t drop_amount = amount2 * 1000000;

// Prepare the payment transactions for the issued asset
uint8_t amt_out1[48];
uint8_t amt_out2[48];

if (float_sto(amt_out1 -1, 49, SBUF(currency_code), SBUF(issuer), main_amount, -1) < 0)
    rollback(SBUF("Could not dump main amount into sto"), 1);

if (float_sto(amt_out2 -1, 49, SBUF(currency_code), SBUF(issuer), drop_amount, -1) < 0)
    rollback(SBUF("Could not dump main amount into sto"), 1);

// Set the currency code and issuer in the amount field
for (int i = 0; GUARD(20), i < 20; ++i) {
    amt_out1[i + 28] = issuer[i];
    amt_out1[i +  8] = currency_code[i];
    amt_out2[i + 28] = issuer[i];
    amt_out2[i +  8] = currency_code[i];
}

// Prepare the outgoing transactions
uint8_t txn1[PREPARE_PAYMENT_SIMPLE_TRUSTLINE_SIZE];
uint8_t txn2[PREPARE_PAYMENT_SIMPLE_TRUSTLINE_SIZE];

PREPARE_PAYMENT_SIMPLE_TRUSTLINE(txn1, amt_out1, main_acc, 0, 0);
PREPARE_PAYMENT_SIMPLE_TRUSTLINE(txn2, amt_out2, drop_acc, 0, 0);

uint8_t emithash1[32];
uint8_t emithash2[32];

// Emit the transactions and check if they were successful
if (emit(SBUF(emithash1), SBUF(txn1)) != 32 || emit(SBUF(emithash2), SBUF(txn2)) != 32) {
    NOPE("Failed to emit transactions");
}

    DONE("Amounts forwarded Successfully");

    _g(1,1);   // every hook needs to import guard function and use it at least once
    // unreachable
    return 0;
}