# Decentralized Drop Wallet

A smart contract system built on Xahau for incentivizing network participation through periodic asset distribution.

## Overview

This repository contains the source code for two Hooks on Xahau:

- **Hook 1 (Configuration)**: Sets up trust lines for assets on participant wallets.
- **Hook 2 (Distribution)**: Automatically distributes a fraction (0.005%) of the total assets held in the distribution wallet to all wallets with the correct trust line set up, daily.

## Features

- **Decentralized**: Runs autonomously on the Xahau network without centralized control.
- **Incentive Mechanism**: Encourages new users to join the network by offering them a share of the network's assets.
- **Sustainable**: Designed for long-term distribution, adapting to the number of participants.
- **Scalable**: Can handle an increasing number of participants with minimal changes.

## Components

### Hook 1: Configuration

- **Purpose**: Prepares wallets to receive assets by setting up trust lines.
- **Activation**: Triggered by a payment to the Hook address with a memo specifying the asset's hex code.
- **Functionality**:
  - Parses the memo for the asset's hex code.
  - Sets or verifies a trust line for the asset in the receiving wallet.

### Hook 2: Distribution

- **Purpose**: Distributes assets to participants with configured trust lines.
- **Activation**: Runs daily based on ledger time.
- **Functionality**:
  - Calculates the current total of assets in the distribution wallet.
  - Distributes 0.005% of that total to each eligible wallet.

## Installation

1. **Clone the Repository**:
   - Clone this repository to your local machine.

2. **Compile the Hooks**:
   - Ensure you have the necessary tools for compiling Hooks for Xahau.
   - Follow the specific instructions in each Hook's directory for compilation.

3. **Deploy to Testnet**:
   - Deploy each Hook on the Xahau testnet first to test functionality and security.

4. **Deploy to Mainnet**:
   - After thorough testing, deploy to the Xahau mainnet.

## Usage

- **For Participants**: 
  - Send a payment with the correct memo to the Hook 1 address to configure your wallet. 
  - Await daily distributions from Hook 2 if your wallet is eligible.

- **For Administrators**:
  - Manage the distribution wallet's balance.
  - Monitor Hook execution and network activity.

## Development

- **Prerequisites**: 
  - Knowledge of Xahau, XRP Ledger concepts, and smart contract development in the supported language (e.g., C for Hooks).
  - Development environment set up for compiling and testing Hooks.

- **Contributing**:
  - Fork the repository and submit pull requests.
  - Ensure your changes pass all tests and do not disrupt the existing functionality.

## Security

- Both Hooks are designed with security in mind, but community audits and ongoing scrutiny are encouraged.
- Use best practices for key management, especially for the distribution wallet.

## Testing

- A separate directory with test cases will be provided for each Hook to ensure functionality and robustness.

## License

- {insert licence later}

## Contact

- For questions or issues, please open an issue on this repository or contact handy_4ndy@outlook.com.

This project is still under active development. Contributions, feedback, and participation in testing are very welcome!
