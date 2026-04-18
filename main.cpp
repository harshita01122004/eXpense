#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <sstream>
#include <functional>
#include <cstdlib>

using namespace std;

// Function to hash password
string hashPassword(const string& password) {
    unsigned long hash = 5381;
    for (size_t i = 0; i < password.length(); ++i) {
        hash = ((hash << 5) + hash) + password[i];
    }
    stringstream ss;
    ss << hash;
    return ss.str();
}

// Function to load users
void loadUsers(map<string, string>& users) {
    ifstream file("users.csv");
    if (!file.is_open()) return;

    string line, username, passwordHash;
    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, username, ',');
        getline(ss, passwordHash, ',');
        users[username] = passwordHash;
    }
    file.close();
}

// Function to save user
void saveUser(const string& username, const string& passwordHash) {
    ofstream file("users.csv", ios::app);
    file << username << "," << passwordHash << "\n";
    file.close();
}

// Function to add expense
void addExpense(const string& username) {
    string category, amount, date;
    cout << "Enter Category (e.g., Food, Travel, Rent): ";
    cin >> ws;
    getline(cin, category);
    cout << "Enter Amount: ";
    cin >> amount;
    cout << "Enter Date (YYYY-MM-DD): ";
    cin >> date;

    string filename = username + "_expenses.csv";
    ifstream checkFile(filename.c_str());
    bool fileExists = checkFile.good();
    checkFile.close();

    ofstream file(filename.c_str(), ios::app);
    if (!fileExists) {
        file << "Category,Amount,Date\n";
    }
    file << category << "," << amount << "," << date << "\n";
    file.close();
    cout << "Expense added successfully!\n";
}

// Function to view history
void viewHistory(const string& username) {
    string filename = username + "_expenses.csv";
    ifstream file(filename.c_str());
    if (!file.is_open()) {
        cout << "No expense history found.\n";
        return;
    }

    string line;
    cout << "\n--- Expense History ---\n";
    while (getline(file, line)) {
        cout << line << "\n";
    }
    cout << "-----------------------\n";
    file.close();
}

// Function to trigger Python visualizer
void triggerVisualizer(const string& username) {
    string filename = username + "_expenses.csv";
    string command = "python tracker_viz.py " + filename;
    cout << "Starting Python Visualizer...\n";
    system(command.c_str());
}

int main() {
    map<string, string> users;
    loadUsers(users);

    int choice;
    string username, password;
    string currentUsername = "";

    while (true) {
        if (currentUsername.empty()) {
            cout << "\n=== Multi-Language Project Expense Tracker ===\n";
            cout << "1. Register\n";
            cout << "2. Login\n";
            cout << "3. Exit\n";
            cout << "Enter choice: ";
            
            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid input. Please enter a number.\n";
                continue;
            }

            if (choice == 1) {
                cout << "Enter new username: ";
                cin >> username;
                if (users.find(username) != users.end()) {
                    cout << "Username already exists.\n";
                } else {
                    cout << "Enter password: ";
                    cin >> password;
                    string hashed = hashPassword(password);
                    users[username] = hashed;
                    saveUser(username, hashed);
                    cout << "Registration successful!\n";
                }
            } else if (choice == 2) {
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;

                if (users.find(username) != users.end() && users[username] == hashPassword(password)) {
                    cout << "Login successful. Welcome, " << username << "!\n";
                    currentUsername = username;
                } else {
                    cout << "Invalid credentials.\n";
                }
            } else if (choice == 3) {
                cout << "Goodbye!\n";
                break;
            } else {
                cout << "Invalid choice.\n";
            }
        } else {
            cout << "\n=== User Menu (" << currentUsername << ") ===\n";
            cout << "1. Add Expense\n";
            cout << "2. View History\n";
            cout << "3. Visualize Expenses\n";
            cout << "4. Logout\n";
            cout << "Enter choice: ";
            
            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid input. Please enter a number.\n";
                continue;
            }

            if (choice == 1) {
                addExpense(currentUsername);
            } else if (choice == 2) {
                viewHistory(currentUsername);
            } else if (choice == 3) {
                triggerVisualizer(currentUsername);
            } else if (choice == 4) {
                currentUsername = "";
                cout << "Logged out successfully.\n";
            } else {
                cout << "Invalid choice.\n";
            }
        }
    }

    return 0;
}
