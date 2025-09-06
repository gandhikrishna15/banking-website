# Banking System

A complete banking system implementation with login/registration and numerical key navigation.

## Features

- **Login/Registration System**: Secure user authentication
- **Numerical Key Navigation**: Press 1-5 to access different features
  - 1: Withdraw Amount
  - 2: Deposit Amount  
  - 3: Check Balance
  - 4: Total Spent
  - 5: Quit
- **Data Persistence**: User data is saved and loaded
- **Transaction History**: Track recent transactions (web version)

## Files Included

### Web Version (HTML/CSS/JavaScript)
- `index.html` - Main web interface
- `style.css` - Styling for the web interface
- `script.js` - JavaScript functionality

### Java Console Version
- `BankingSystem.java` - Complete Java implementation

### C Console Version
- `banking_system.c` - Complete C implementation

## How to Run

### Web Version
1. Open `index.html` in a web browser
2. Register a new account or login
3. Use numerical keys 1-5 to navigate

### Java Version
```bash
javac BankingSystem.java
java BankingSystem
```

### C Version
```bash
gcc banking_system.c -o banking_system
./banking_system
```

## Usage Instructions

1. **Registration**: Create a new account with username, password, and email
2. **Login**: Use your credentials to access the banking system
3. **Navigation**: Use keys 1-5 to access different features:
   - Press 1 for withdrawals
   - Press 2 for deposits
   - Press 3 to check current balance
   - Press 4 to view total amount spent
   - Press 5 to quit/logout

## Default Settings

- Starting balance: $1000.00
- Data is automatically saved and loaded
- Web version includes transaction history
- Console versions save data to text files

## Security Features

- Password-protected accounts
- Input validation for amounts
- Insufficient balance protection
- Data persistence across sessions