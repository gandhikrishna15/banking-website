#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_USERS 100
#define MAX_USERNAME 50
#define MAX_PASSWORD 50
#define MAX_EMAIL 100
#define DATA_FILE "bank_data_c.txt"

typedef struct {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    char email[MAX_EMAIL];
    double balance;
    double totalSpent;
} User;

typedef struct {
    User users[MAX_USERS];
    int userCount;
    int currentUserIndex;
} BankingSystem;

BankingSystem bank;

void initializeSystem();
void saveData();
void loadData();
void showAuthMenu();
void showBankingMenu();
void login();
void registerUser();
void withdrawAmount();
void depositAmount();
void checkBalance();
void showTotalSpent();
void quit();
int findUser(const char* username);
void clearInputBuffer();

int main() {
    initializeSystem();
    loadData();
    
    printf("=== Welcome to Banking System ===\n");
    
    while (1) {
        if (bank.currentUserIndex == -1) {
            showAuthMenu();
        } else {
            showBankingMenu();
        }
    }
    
    return 0;
}

void initializeSystem() {
    bank.userCount = 0;
    bank.currentUserIndex = -1;
}

void showAuthMenu() {
    int choice;
    
    printf("\n1. Login\n");
    printf("2. Register\n");
    printf("3. Exit\n");
    printf("Choose option: ");
    
    scanf("%d", &choice);
    clearInputBuffer();
    
    switch (choice) {
        case 1:
            login();
            break;
        case 2:
            registerUser();
            break;
        case 3:
            saveData();
            exit(0);
            break;
        default:
            printf("Invalid option!\n");
    }
}

void login() {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    
    printf("Username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0; // Remove newline
    
    printf("Password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0; // Remove newline
    
    int userIndex = findUser(username);
    if (userIndex != -1 && strcmp(bank.users[userIndex].password, password) == 0) {
        bank.currentUserIndex = userIndex;
        printf("Login successful!\n");
    } else {
        printf("Invalid credentials!\n");
    }
}

void registerUser() {
    if (bank.userCount >= MAX_USERS) {
        printf("Maximum users reached!\n");
        return;
    }
    
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    char email[MAX_EMAIL];
    
    printf("Username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0;
    
    if (findUser(username) != -1) {
        printf("Username already exists!\n");
        return;
    }
    
    printf("Password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0;
    
    printf("Email: ");
    fgets(email, sizeof(email), stdin);
    email[strcspn(email, "\n")] = 0;
    
    // Create new user
    strcpy(bank.users[bank.userCount].username, username);
    strcpy(bank.users[bank.userCount].password, password);
    strcpy(bank.users[bank.userCount].email, email);
    bank.users[bank.userCount].balance = 1000.0;
    bank.users[bank.userCount].totalSpent = 0.0;
    
    bank.userCount++;
    printf("Registration successful!\n");
}

void showBankingMenu() {
    int choice;
    User* currentUser = &bank.users[bank.currentUserIndex];
    
    printf("\n=== Banking Dashboard ===\n");
    printf("Current Balance: $%.2f\n", currentUser->balance);
    printf("\nPress a number key (1-5):\n");
    printf("1 - Withdraw Amount\n");
    printf("2 - Deposit Amount\n");
    printf("3 - Check Balance\n");
    printf("4 - Total Spent\n");
    printf("5 - Quit\n");
    printf("Choose option: ");
    
    scanf("%d", &choice);
    
    switch (choice) {
        case 1:
            withdrawAmount();
            break;
        case 2:
            depositAmount();
            break;
        case 3:
            checkBalance();
            break;
        case 4:
            showTotalSpent();
            break;
        case 5:
            quit();
            break;
        default:
            printf("Invalid option! Please press 1-5.\n");
    }
}

void withdrawAmount() {
    double amount;
    User* currentUser = &bank.users[bank.currentUserIndex];
    
    printf("Enter amount to withdraw: $");
    scanf("%lf", &amount);
    
    if (amount <= 0) {
        printf("Invalid amount!\n");
        return;
    }
    
    if (amount > currentUser->balance) {
        printf("Insufficient balance!\n");
        return;
    }
    
    currentUser->balance -= amount;
    currentUser->totalSpent += amount;
    
    printf("Successfully withdrew $%.2f\n", amount);
    printf("New balance: $%.2f\n", currentUser->balance);
}

void depositAmount() {
    double amount;
    User* currentUser = &bank.users[bank.currentUserIndex];
    
    printf("Enter amount to deposit: $");
    scanf("%lf", &amount);
    
    if (amount <= 0) {
        printf("Invalid amount!\n");
        return;
    }
    
    currentUser->balance += amount;
    
    printf("Successfully deposited $%.2f\n", amount);
    printf("New balance: $%.2f\n", currentUser->balance);
}

void checkBalance() {
    User* currentUser = &bank.users[bank.currentUserIndex];
    printf("Current Balance: $%.2f\n", currentUser->balance);
}

void showTotalSpent() {
    User* currentUser = &bank.users[bank.currentUserIndex];
    printf("Total Spent: $%.2f\n", currentUser->totalSpent);
}

void quit() {
    printf("Thank you for using Banking System!\n");
    saveData();
    bank.currentUserIndex = -1;
}

int findUser(const char* username) {
    for (int i = 0; i < bank.userCount; i++) {
        if (strcmp(bank.users[i].username, username) == 0) {
            return i;
        }
    }
    return -1;
}

void saveData() {
    FILE* file = fopen(DATA_FILE, "w");
    if (file == NULL) {
        printf("Error saving data!\n");
        return;
    }
    
    for (int i = 0; i < bank.userCount; i++) {
        fprintf(file, "%s,%s,%s,%.2f,%.2f\n",
                bank.users[i].username,
                bank.users[i].password,
                bank.users[i].email,
                bank.users[i].balance,
                bank.users[i].totalSpent);
    }
    
    fclose(file);
}

void loadData() {
    FILE* file = fopen(DATA_FILE, "r");
    if (file == NULL) {
        return; // File doesn't exist yet
    }
    
    char line[256];
    while (fgets(line, sizeof(line), file) && bank.userCount < MAX_USERS) {
        char* token = strtok(line, ",");
        if (token) strcpy(bank.users[bank.userCount].username, token);
        
        token = strtok(NULL, ",");
        if (token) strcpy(bank.users[bank.userCount].password, token);
        
        token = strtok(NULL, ",");
        if (token) strcpy(bank.users[bank.userCount].email, token);
        
        token = strtok(NULL, ",");
        if (token) bank.users[bank.userCount].balance = atof(token);
        
        token = strtok(NULL, ",");
        if (token) bank.users[bank.userCount].totalSpent = atof(token);
        
        bank.userCount++;
    }
    
    fclose(file);
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}