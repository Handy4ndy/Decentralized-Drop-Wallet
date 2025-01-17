# ConfigForward Hook

This hook is designed to configure the distribution of incoming XAH payments to predefined accounts.

## Overview

The hook currently distributes incoming payments as follows:

- **10%** to the Main Account
- **40%** to the Drop Account
- **40%** to the Treasury Account
- The remaining **10%** stays in the hook wallet

## Hook Workflow

The hook performs the following steps:

1. Retrieves the hook account and the originating account of the transaction.
2. Checks if the transaction is outgoing from the hook account and accepts it if true.
3. Retrieves the transaction amount and ensures it is in Xah (8 bytes).
4. Reserves space for three emitted transactions.
5. Calculates the amounts to be forwarded to each account.
6. Prepares the payment transactions for each account.
7. Emits the transactions and checks if they were successful.
8. Accepts the transaction with a success message if all emissions were successful.

## Error Handling

The hook includes error handling for the following scenarios:

- If the main account is not set.
- If the transaction amount is not in Xah (8 bytes).
- If the transaction is not outgoing from the hook account.

## Issues and TODOs

### Issues

- Rollback needs to be added for any other transactions on the account as this is for configuration only.

### TODOs

- Ensure the transaction amount is exactly 100 XAH.
- Rollback any other transaction.
- Integration of I, C, U params to also emit trustset's to the specified accounts.

## Example Usage

To use this hook, set the parameters as follows:

- Main Account: "M" (HEX_AccountID)
- Drop Account: "D" (HEX_AccountID)
- Treasury Account: "T" (HEX_AccountID)



## Additional Information

For more details, refer to the source code and comments within the hook implementation.

---

This README provides an overview of the ConfigForward hook, its workflow, error handling, and example usage. For further assistance, refer to the provided tools and documentation links.