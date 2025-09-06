import java.util.*;
import java.io.*;

public class BankingSystem {
    private Map<String, User> users;
    private User currentUser;
    private Scanner scanner;
    private static final String DATA_FILE = "bank_data.txt";

    public BankingSystem() {
        users = new HashMap<>();
        scanner = new Scanner(System.in);
        loadData();
    }

    public static void main(String[] args) {
        BankingSystem bank = new BankingSystem();
        bank.run();
    }

    public void run() {
        System.out.println("=== Welcome to Banking System ===");
        
        while (true) {
            if (currentUser == null) {
                showAuthMenu();
            } else {
                showBankingMenu();
            }
        }
    }

    private void showAuthMenu() {
        System.out.println("\n1. Login");
        System.out.println("2. Register");
        System.out.println("3. Exit");
        System.out.print("Choose option: ");
        
        int choice = scanner.nextInt();
        scanner.nextLine(); // consume newline
        
        switch (choice) {
            case 1:
                login();
                break;
            case 2:
                register();
                break;
            case 3:
                saveData();
                System.exit(0);
                break;
            default:
                System.out.println("Invalid option!");
        }
    }

    private void login() {
        System.out.print("Username: ");
        String username = scanner.nextLine();
        System.out.print("Password: ");
        String password = scanner.nextLine();
        
        User user = users.get(username);
        if (user != null && user.getPassword().equals(password)) {
            currentUser = user;
            System.out.println("Login successful!");
        } else {
            System.out.println("Invalid credentials!");
        }
    }

    private void register() {
        System.out.print("Username: ");
        String username = scanner.nextLine();
        
        if (users.containsKey(username)) {
            System.out.println("Username already exists!");
            return;
        }
        
        System.out.print("Password: ");
        String password = scanner.nextLine();
        System.out.print("Email: ");
        String email = scanner.nextLine();
        
        User newUser = new User(username, password, email);
        users.put(username, newUser);
        System.out.println("Registration successful!");
    }

    private void showBankingMenu() {
        System.out.println("\n=== Banking Dashboard ===");
        System.out.println("Current Balance: $" + String.format("%.2f", currentUser.getBalance()));
        System.out.println("\nPress a number key (1-5):");
        System.out.println("1 - Withdraw Amount");
        System.out.println("2 - Deposit Amount");
        System.out.println("3 - Check Balance");
        System.out.println("4 - Total Spent");
        System.out.println("5 - Quit");
        System.out.print("Choose option: ");
        
        int choice = scanner.nextInt();
        
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
                System.out.println("Invalid option! Please press 1-5.");
        }
    }

    private void withdrawAmount() {
        System.out.print("Enter amount to withdraw: $");
        double amount = scanner.nextDouble();
        
        if (amount <= 0) {
            System.out.println("Invalid amount!");
            return;
        }
        
        if (amount > currentUser.getBalance()) {
            System.out.println("Insufficient balance!");
            return;
        }
        
        currentUser.withdraw(amount);
        System.out.println("Successfully withdrew $" + String.format("%.2f", amount));
        System.out.println("New balance: $" + String.format("%.2f", currentUser.getBalance()));
    }

    private void depositAmount() {
        System.out.print("Enter amount to deposit: $");
        double amount = scanner.nextDouble();
        
        if (amount <= 0) {
            System.out.println("Invalid amount!");
            return;
        }
        
        currentUser.deposit(amount);
        System.out.println("Successfully deposited $" + String.format("%.2f", amount));
        System.out.println("New balance: $" + String.format("%.2f", currentUser.getBalance()));
    }

    private void checkBalance() {
        System.out.println("Current Balance: $" + String.format("%.2f", currentUser.getBalance()));
    }

    private void showTotalSpent() {
        System.out.println("Total Spent: $" + String.format("%.2f", currentUser.getTotalSpent()));
    }

    private void quit() {
        System.out.println("Thank you for using Banking System!");
        saveData();
        currentUser = null;
    }

    private void saveData() {
        try (PrintWriter writer = new PrintWriter(new FileWriter(DATA_FILE))) {
            for (User user : users.values()) {
                writer.println(user.getUsername() + "," + user.getPassword() + "," + 
                             user.getEmail() + "," + user.getBalance() + "," + user.getTotalSpent());
            }
        } catch (IOException e) {
            System.out.println("Error saving data: " + e.getMessage());
        }
    }

    private void loadData() {
        try (BufferedReader reader = new BufferedReader(new FileReader(DATA_FILE))) {
            String line;
            while ((line = reader.readLine()) != null) {
                String[] parts = line.split(",");
                if (parts.length == 5) {
                    User user = new User(parts[0], parts[1], parts[2]);
                    user.setBalance(Double.parseDouble(parts[3]));
                    user.setTotalSpent(Double.parseDouble(parts[4]));
                    users.put(parts[0], user);
                }
            }
        } catch (IOException e) {
            // File doesn't exist yet, that's okay
        }
    }
}

class User {
    private String username;
    private String password;
    private String email;
    private double balance;
    private double totalSpent;

    public User(String username, String password, String email) {
        this.username = username;
        this.password = password;
        this.email = email;
        this.balance = 1000.0; // Starting balance
        this.totalSpent = 0.0;
    }

    public void withdraw(double amount) {
        balance -= amount;
        totalSpent += amount;
    }

    public void deposit(double amount) {
        balance += amount;
    }

    // Getters and Setters
    public String getUsername() { return username; }
    public String getPassword() { return password; }
    public String getEmail() { return email; }
    public double getBalance() { return balance; }
    public double getTotalSpent() { return totalSpent; }
    
    public void setBalance(double balance) { this.balance = balance; }
    public void setTotalSpent(double totalSpent) { this.totalSpent = totalSpent; }
}