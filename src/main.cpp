#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <vector>

#include "FoodExpense.h"
#include "Income.h"
#include "TravelExpense.h"

namespace {
const char* DATA_FILE = "transactions.txt";
double budgetLimit = 0.0;
int nextId = 1;
std::vector<std::unique_ptr<Transaction>> transactions;

int readInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        std::cout << "Invalid input. Enter a whole number.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

double readPositiveAmount(const std::string& prompt) {
    double amount;
    while (true) {
        std::cout << prompt;
        if (std::cin >> amount && std::isfinite(amount) && amount > 0.0) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return amount;
        }
        std::cout << "Enter a valid amount greater than zero.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

std::string readRequiredText(const std::string& prompt) {
    std::string value;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, value);
        if (!value.empty() && value.find_first_not_of(" \t\r\n") != std::string::npos)
            return value;
        std::cout << "This field cannot be empty.\n";
    }
}

// Basic format validation for DD/MM/YYYY (not full calendar validation).
bool hasDateFormat(const std::string& date) {
    if (date.size() != 10 || date[2] != '/' || date[5] != '/') return false;
    for (std::size_t i = 0; i < date.size(); ++i) {
        if (i == 2 || i == 5) continue;
        if (date[i] < '0' || date[i] > '9') return false;
    }
    int day = std::stoi(date.substr(0, 2));
    int month = std::stoi(date.substr(3, 2));
    int year = std::stoi(date.substr(6, 4));
    return day >= 1 && day <= 31 && month >= 1 && month <= 12 && year >= 1;
}

std::string readDate() {
    while (true) {
        std::string date = readRequiredText("Enter date (DD/MM/YYYY): ");
        if (hasDateFormat(date)) return date;
        std::cout << "Use DD/MM/YYYY, for example 14/10/2026.\n";
    }
}

double totalIncome() {
    double total = 0.0;
    for (const auto& item : transactions)
        if (item->getType() == "INCOME") total += item->getSignedAmount();
    return total;
}

double totalExpenses() {
    double total = 0.0;
    for (const auto& item : transactions)
        if (item->getType() != "INCOME") total -= item->getSignedAmount();
    return total;
}

double currentBalance() {
    double balance = 0.0;
    for (const auto& item : transactions) balance += item->getSignedAmount();
    return balance;
}

void saveData() {
    std::ofstream file(DATA_FILE, std::ios::trunc);
    if (!file) {
        std::cerr << "Error: could not save data to " << DATA_FILE << ".\n";
        return;
    }
    file << std::fixed << std::setprecision(2);
    file << "BUDGET " << budgetLimit << '\n';
    for (const auto& item : transactions) {
        file << "TRANSACTION " << item->getType() << ' ' << item->getId() << ' '
             << std::quoted(item->getDate()) << ' '
             << std::quoted(item->getDescription()) << ' '
             << item->getAmount() << '\n';
    }
    if (!file) std::cerr << "Error: a problem occurred while writing data.\n";
}

void loadData() {
    std::ifstream file(DATA_FILE);
    if (!file) return; // A missing file is normal on the first run.

    std::string recordType;
    int largestId = 0;
    while (file >> recordType) {
        if (recordType == "BUDGET") {
            double value;
            if (!(file >> value) || !std::isfinite(value) || value < 0.0) {
                std::cerr << "Warning: invalid budget record; loading stopped.\n";
                break;
            }
            budgetLimit = value;
        } else if (recordType == "TRANSACTION") {
            std::string type, date, description;
            int id;
            double amount;
            if (!(file >> type >> id >> std::quoted(date) >> std::quoted(description) >> amount)) {
                std::cerr << "Warning: malformed transaction record; loading stopped.\n";
                break;
            }
            if (id <= 0 || !std::isfinite(amount) || amount <= 0.0) {
                std::cerr << "Warning: invalid transaction skipped.\n";
                continue;
            }

            if (type == "FOOD") {
                transactions.push_back(std::make_unique<FoodExpense>(id, date, description, amount));
            } else if (type == "TRAVEL") {
                transactions.push_back(std::make_unique<TravelExpense>(id, date, description, amount));
            } else if (type == "INCOME") {
                transactions.push_back(std::make_unique<Income>(id, date, description, amount));
            } else {
                std::cerr << "Warning: unknown transaction type skipped: " << type << '\n';
                continue;
            }
            largestId = std::max(largestId, id);
        } else {
            std::cerr << "Warning: unknown record in data file; loading stopped.\n";
            break;
        }
    }
    nextId = largestId + 1;
}

void addTransaction(int choice) {
    const std::string date = readDate();
    const std::string description = readRequiredText("Enter description: ");
    const double amount = readPositiveAmount("Enter amount (Rs): ");

    if (choice == 2)
        transactions.push_back(std::make_unique<Income>(nextId++, date, description, amount));
    else if (choice == 3)
        transactions.push_back(std::make_unique<FoodExpense>(nextId++, date, description, amount));
    else if (choice == 4)
        transactions.push_back(std::make_unique<TravelExpense>(nextId++, date, description, amount));

    saveData();
    std::cout << "Transaction added and saved.\n";
    if (budgetLimit > 0.0 && totalExpenses() > budgetLimit)
        std::cout << "WARNING: your expenses exceed the spending budget.\n";
}

void displayTransactions() {
    if (transactions.empty()) {
        std::cout << "\nNo transactions recorded yet.\n";
        return;
    }
    std::cout << "\nTRANSACTION HISTORY\n";
    std::cout << std::left << std::setw(6) << "ID" << std::setw(15) << "Date"
              << std::setw(15) << "Type" << std::setw(28) << "Description"
              << std::right << "Amount (Rs)\n";
    std::cout << std::string(79, '-') << '\n';
    for (const auto& item : transactions) item->display(); // Runtime polymorphism.
}

void displaySummary() {
    const double income = totalIncome();
    const double expenses = totalExpenses();
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\n========== FINANCIAL SUMMARY ==========\n"
              << "Total income       : Rs " << income << '\n'
              << "Total expenses     : Rs " << expenses << '\n'
              << "Current balance    : Rs " << currentBalance() << '\n'
              << "Spending budget    : Rs " << budgetLimit << '\n';
    if (budgetLimit > 0.0) {
        std::cout << "Budget remaining   : Rs " << budgetLimit - expenses << '\n'
                  << "Budget status      : "
                  << (expenses > budgetLimit ? "OVER BUDGET" : "Within budget") << '\n';
    } else {
        std::cout << "Budget status      : No budget set\n";
    }
    std::cout << "=======================================\n";
}

void deleteAllData() {
    const std::string answer = readRequiredText("Type YES to delete all saved data: ");
    if (answer != "YES") {
        std::cout << "Deletion cancelled.\n";
        return;
    }
    transactions.clear();
    budgetLimit = 0.0;
    nextId = 1;
    saveData();
    std::cout << "All transactions and budget data deleted.\n";
}
} // namespace

int main() {
    loadData();
    std::cout << "=======================================\n"
              << " PERSONAL EXPENSE AND BUDGET TRACKER\n"
              << "=======================================\n";

    while (true) {
        std::cout << "\n1. Set Budget\n"
                  << "2. Add Income\n"
                  << "3. Add Food Expense\n"
                  << "4. Add Travel Expense\n"
                  << "5. Display Transactions\n"
                  << "6. Financial Summary\n"
                  << "7. Delete All Data\n"
                  << "8. Save and Exit\n";
        const int choice = readInt("Enter your choice: ");
        switch (choice) {
            case 1: {
                budgetLimit = readPositiveAmount("Enter spending budget (Rs): ");
                saveData();
                std::cout << "Budget saved.\n";
                if (totalExpenses() > budgetLimit)
                    std::cout << "WARNING: existing expenses exceed this budget.\n";
                break;
            }
            case 2: case 3: case 4:
                addTransaction(choice);
                break;
            case 5:
                displayTransactions();
                break;
            case 6:
                displaySummary();
                break;
            case 7:
                deleteAllData();
                break;
            case 8:
                saveData();
                std::cout << "Data saved. Goodbye!\n";
                return 0;
            default:
                std::cout << "Invalid choice. Select a number from 1 to 8.\n";
        }
    }
}
