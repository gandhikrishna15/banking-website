// Banking System JavaScript
class BankingSystem {
    constructor() {
        this.users = JSON.parse(localStorage.getItem('bankUsers')) || {};
        this.currentUser = null;
        this.balance = 1000.00;
        this.totalSpent = 0;
        this.transactions = [];
        this.currentAction = null;
        
        this.initializeEventListeners();
    }

    initializeEventListeners() {
        // Auth form handling
        document.getElementById('auth-form').addEventListener('submit', (e) => {
            e.preventDefault();
            this.handleAuth();
        });

        document.getElementById('toggle-link').addEventListener('click', (e) => {
            e.preventDefault();
            this.toggleAuthMode();
        });

        // Keyboard event for numerical keys
        document.addEventListener('keydown', (e) => {
            if (document.getElementById('banking-section').style.display !== 'none') {
                this.handleKeyPress(e);
            }
        });

        // Confirm button
        document.getElementById('confirm-btn').addEventListener('click', () => {
            this.confirmAction();
        });

        // Enter key on amount input
        document.getElementById('amount-input').addEventListener('keypress', (e) => {
            if (e.key === 'Enter') {
                this.confirmAction();
            }
        });
    }

    toggleAuthMode() {
        const formTitle = document.getElementById('form-title');
        const authBtn = document.getElementById('auth-btn');
        const emailField = document.getElementById('email');
        const toggleText = document.getElementById('toggle-text');
        const toggleLink = document.getElementById('toggle-link');

        if (formTitle.textContent === 'Login') {
            formTitle.textContent = 'Register';
            authBtn.textContent = 'Register';
            emailField.style.display = 'block';
            toggleText.textContent = 'Already have an account?';
            toggleLink.textContent = 'Login';
        } else {
            formTitle.textContent = 'Login';
            authBtn.textContent = 'Login';
            emailField.style.display = 'none';
            toggleText.textContent = "Don't have an account?";
            toggleLink.textContent = 'Register';
        }
    }

    handleAuth() {
        const username = document.getElementById('username').value;
        const password = document.getElementById('password').value;
        const email = document.getElementById('email').value;
        const isLogin = document.getElementById('form-title').textContent === 'Login';

        if (isLogin) {
            if (this.users[username] && this.users[username].password === password) {
                this.currentUser = username;
                this.loadUserData();
                this.showBankingSection();
            } else {
                alert('Invalid credentials!');
            }
        } else {
            if (this.users[username]) {
                alert('Username already exists!');
            } else {
                this.users[username] = {
                    password: password,
                    email: email,
                    balance: 1000.00,
                    totalSpent: 0,
                    transactions: []
                };
                localStorage.setItem('bankUsers', JSON.stringify(this.users));
                alert('Registration successful! Please login.');
                this.toggleAuthMode();
            }
        }
    }

    loadUserData() {
        const userData = this.users[this.currentUser];
        this.balance = userData.balance;
        this.totalSpent = userData.totalSpent;
        this.transactions = userData.transactions || [];
        this.updateDisplay();
    }

    saveUserData() {
        this.users[this.currentUser] = {
            ...this.users[this.currentUser],
            balance: this.balance,
            totalSpent: this.totalSpent,
            transactions: this.transactions
        };
        localStorage.setItem('bankUsers', JSON.stringify(this.users));
    }

    showBankingSection() {
        document.getElementById('auth-section').style.display = 'none';
        document.getElementById('banking-section').style.display = 'block';
        this.updateDisplay();
        this.showTransactionHistory();
    }

    handleKeyPress(e) {
        const key = e.key;
        
        if (key >= '1' && key <= '5') {
            this.hideInputs();
            
            switch(key) {
                case '1':
                    this.initiateWithdraw();
                    break;
                case '2':
                    this.initiateDeposit();
                    break;
                case '3':
                    this.checkBalance();
                    break;
                case '4':
                    this.showTotalSpent();
                    break;
                case '5':
                    this.quit();
                    break;
            }
        }
    }

    hideInputs() {
        document.getElementById('amount-input').style.display = 'none';
        document.getElementById('confirm-btn').style.display = 'none';
    }

    showInputs() {
        document.getElementById('amount-input').style.display = 'block';
        document.getElementById('confirm-btn').style.display = 'block';
        document.getElementById('amount-input').focus();
    }

    initiateWithdraw() {
        this.currentAction = 'withdraw';
        document.getElementById('action-display').innerHTML = 
            '<span class="error">Withdraw Money</span><br>Enter amount to withdraw:';
        this.showInputs();
    }

    initiateDeposit() {
        this.currentAction = 'deposit';
        document.getElementById('action-display').innerHTML = 
            '<span class="success">Deposit Money</span><br>Enter amount to deposit:';
        this.showInputs();
    }

    checkBalance() {
        document.getElementById('action-display').innerHTML = 
            `<span class="info">Current Balance: $${this.balance.toFixed(2)}</span>`;
    }

    showTotalSpent() {
        document.getElementById('action-display').innerHTML = 
            `<span class="error">Total Spent: $${this.totalSpent.toFixed(2)}</span>`;
    }

    quit() {
        if (confirm('Are you sure you want to quit?')) {
            this.saveUserData();
            document.getElementById('banking-section').style.display = 'none';
            document.getElementById('auth-section').style.display = 'block';
            document.getElementById('auth-form').reset();
            document.getElementById('action-display').innerHTML = '';
        }
    }

    confirmAction() {
        const amount = parseFloat(document.getElementById('amount-input').value);
        
        if (isNaN(amount) || amount <= 0) {
            alert('Please enter a valid amount!');
            return;
        }

        if (this.currentAction === 'withdraw') {
            if (amount > this.balance) {
                alert('Insufficient balance!');
                return;
            }
            this.balance -= amount;
            this.totalSpent += amount;
            this.addTransaction(`Withdrew $${amount.toFixed(2)}`);
            document.getElementById('action-display').innerHTML = 
                `<span class="success">Successfully withdrew $${amount.toFixed(2)}</span>`;
        } else if (this.currentAction === 'deposit') {
            this.balance += amount;
            this.addTransaction(`Deposited $${amount.toFixed(2)}`);
            document.getElementById('action-display').innerHTML = 
                `<span class="success">Successfully deposited $${amount.toFixed(2)}</span>`;
        }

        this.updateDisplay();
        this.showTransactionHistory();
        this.hideInputs();
        document.getElementById('amount-input').value = '';
        this.saveUserData();
    }

    addTransaction(description) {
        const transaction = {
            description: description,
            timestamp: new Date().toLocaleString(),
            balance: this.balance
        };
        this.transactions.unshift(transaction);
        if (this.transactions.length > 10) {
            this.transactions = this.transactions.slice(0, 10);
        }
    }

    showTransactionHistory() {
        const historyList = document.getElementById('history-list');
        historyList.innerHTML = '';
        
        this.transactions.forEach(transaction => {
            const li = document.createElement('li');
            li.innerHTML = `${transaction.description} - ${transaction.timestamp}`;
            historyList.appendChild(li);
        });
    }

    updateDisplay() {
        document.getElementById('current-balance').textContent = this.balance.toFixed(2);
    }
}

// Initialize the banking system when page loads
document.addEventListener('DOMContentLoaded', () => {
    new BankingSystem();
});