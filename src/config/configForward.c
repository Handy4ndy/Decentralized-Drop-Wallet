/**
 * This hook is set on payments to the ConfigHook that forwards a percentage of all XAH payments to the hardcoded accounts.
 * 
 * The hook is designed to forward incoming payments to three predefined accounts:
 * - Main Account
 * - Drop Account
 * - Treasury Account
 * 
 * The amounts are distributed as follows:
 * - 10% to the Main Account
 * - 40% to the Drop Account
 * - 40% to the Treasury Account
 * - The remaining 10% stays in the hook wallet
 * 
 * The hook performs the following steps:
 * 1. Retrieves the hook account and the originating account of the transaction.
 * 2. Checks if the transaction is outgoing from the hook account and accepts it if true.
 * 3. Retrieves the transaction amount and ensures it is in Xah (8 bytes).
 * 4. Reserves space for three emitted transactions.
 * 5. Calculates the amounts to be forwarded to each account.
 * 6. Prepares the payment transactions for each account.
 * 7. Emits the transactions and checks if they were successful.
 * 8. Accepts the transaction with a success message if all emissions were successful.
 * 
 * The hook uses the following hardcoded account IDs:
 * - Main Account: 95DB318777D081E08B9D7CE472F2596FE5C30ACE
 * - Drop Account: 6CD18DDE236F92B2AE342E77139F0F72185C5A4F
 * - Treasury Account: 08478B3341BB50FC057368F22AF78C59F85E41C5
 * 
 * Issue: 
 * The accounts are currently hardcoded, this should be changed to hook params to aid user setup.
 * rollback needs to be added for any other transactions on the account as this is for configuration only.
 * 
 * TODO: 
 * Assign params to the hardcoded accounts to aid user setup. 
 * Only accept a payment of 100 XAH, rollback any other transaction.
 * Possible integration of I, C, U params to emit trustlines to the drop and treasury account.
 * 
 */

#include "hookapi.h"

#define DONE(x) accept(SBUF(x), __LINE__)
#define NOPE(x) rollback(SBUF(x), __LINE__)

int64_t hook(uint32_t reserved ) {

    uint8_t main_acc[20] = { 0x95U, 0xDBU, 0x31U, 0x87U, 0x77U, 0xD0U, 0x81U, 0xE0U, 0x8BU, 0x9DU, 0x7CU, 0xE4U, 0x72U, 0xF2U, 0x59U, 0x6FU, 0xE5U, 0xC3U, 0x0AU, 0xCEU };
    uint8_t drop_acc[20] = { 0x6CU, 0xD1U, 0x8DU, 0xDEU, 0x23U, 0x6FU, 0x92U, 0xB2U, 0xAEU, 0x34U, 0x2EU, 0x77U, 0x13U, 0x9FU, 0x0FU, 0x72U, 0x18U, 0x5CU, 0x5AU, 0x4FU };
    uint8_t treasury_acc[20] = { 0x08U, 0x47U, 0x8BU, 0x33U, 0x41U, 0xBBU, 0x50U, 0xFCU, 0x05U, 0x73U, 0x68U, 0xF2U, 0x2AU, 0xF7U, 0x8CU, 0x59U, 0xF8U, 0x5EU, 0x41U, 0xC5U };

    uint8_t hook_acc[20];
    // Get the hook account
    hook_account(SBUF(hook_acc));

    uint8_t account[20];
    // Get the originating account of the transaction
    otxn_field(SBUF(account), sfAccount);

    // If the transaction is outgoing from the hook account, accept it
    if(BUFFER_EQUAL_20(hook_acc, account))
        DONE("Outgoing Transaction");

    uint8_t amount[8];
    // Get the transaction amount
    if(otxn_field(SBUF(amount), sfAmount) != 8)
        DONE("Non-Xah Transaction, accepted!");

    // Reserve space for three emitted transactions
    etxn_reserve(3);

    // Calculate the amounts to be forwarded
    uint64_t total_amount = AMOUNT_TO_DROPS(amount);
    uint64_t amount1 = total_amount * 10 / 100; // 10% to main_acc
    uint64_t amount2 = total_amount * 40 / 100; // 40% to drop_acc
    uint64_t amount3 = total_amount * 40 / 100; // 40% to treasury_acc
    uint64_t remaining_amount = total_amount - amount1 - amount2 - amount3; // 10% remains in hook wallet

    // Convert drops back to Xah for logging (1 Xah = 1,000,000 drops)
    uint64_t total_amount_xah = total_amount / 1000000;
    uint64_t amount1_xah = amount1 / 1000000;
    uint64_t amount2_xah = amount2 / 1000000;
    uint64_t amount3_xah = amount3 / 1000000;
    uint64_t remaining_amount_xah = remaining_amount / 1000000;

    // Log the amounts being forwarded to each account in Xah
    trace_num(SBUF("Total Amount (Xah): "), total_amount_xah);
    trace_num(SBUF("Amount to Main Account (Xah): "), amount1_xah);
    trace_num(SBUF("Amount to Drop Account (Xah): "), amount2_xah);
    trace_num(SBUF("Amount to Treasury Account (Xah): "), amount3_xah);
    trace_num(SBUF("Remaining Amount (Xah): "), remaining_amount_xah);

    uint8_t txn1[PREPARE_PAYMENT_SIMPLE_SIZE];
    uint8_t txn2[PREPARE_PAYMENT_SIMPLE_SIZE];
    uint8_t txn3[PREPARE_PAYMENT_SIMPLE_SIZE];

    // Prepare the payment transactions
    PREPARE_PAYMENT_SIMPLE(txn1, amount1, main_acc, 0, 0);
    PREPARE_PAYMENT_SIMPLE(txn2, amount2, drop_acc, 0, 0);
    PREPARE_PAYMENT_SIMPLE(txn3, amount3, treasury_acc, 0, 0);

    uint8_t emithash1[32];
    uint8_t emithash2[32];
    uint8_t emithash3[32];

    // Emit the transactions and check if they were successful
    if(emit(SBUF(emithash1), SBUF(txn1)) != 32 ||
       emit(SBUF(emithash2), SBUF(txn2)) != 32 ||
       emit(SBUF(emithash3), SBUF(txn3)) != 32) {
        NOPE("Failed Please try again later");
    }

    DONE("Amount forwarded Successfully");

    _g(1,1);   // every hook needs to import guard function and use it at least once
    // unreachable
    return 0;
}