# Project Report: Personal Expense and Budget Tracking System

## 1. Problem statement

College students may find it difficult to track daily allowances and personal spending. This project provides an offline, menu-driven C++ application for recording income and common expenses, monitoring a spending budget, and preserving transaction history locally.

## 2. Objectives

- Record income, food expenses, and travel expenses.
- Calculate income, expenses, balance, and budget usage.
- Demonstrate abstraction, encapsulation, inheritance, and runtime polymorphism.
- Use text-file handling to retain transaction history between program runs.

## 3. Class design

### Transaction (abstract base class)
Stores common transaction details: ID, date, description, and amount. Its pure virtual methods require derived classes to provide a type, signed amount, and display implementation.

### FoodExpense
Represents food-related spending. Its signed amount is negative because it reduces the balance.

### TravelExpense
Represents travel-related spending. Its signed amount is negative because it reduces the balance.

### Income
Represents money received. Its signed amount is positive because it increases the balance.

## 4. OOP concepts

- **Abstraction:** `Transaction` defines a common interface and cannot be instantiated directly.
- **Encapsulation:** fields are private and exposed through getter methods.
- **Inheritance:** each concrete transaction type derives from `Transaction`.
- **Polymorphism:** derived objects are stored through base-class smart pointers and virtual methods are dispatched at runtime.

## 5. File handling

The application writes a budget record and transaction records to `transactions.txt`. On startup, it reads the file and creates the corresponding derived object according to the saved type. The data file is local and is excluded from version control to avoid publishing personal data.

## 6. Test plan

1. Add income of Rs 5,000.
2. Add a food expense of Rs 150 and travel expense of Rs 100.
3. Confirm total income = Rs 5,000, total expenses = Rs 250, and current balance = Rs 4,750.
4. Set a spending budget of Rs 5,000 and verify budget remaining = Rs 4,750.
5. Exit and restart; verify that all transactions load.
6. Try invalid menu entries and non-positive amounts.
7. Confirm that deleting all data clears saved transactions and budget.

## 7. Limitations and future enhancements

Potential improvements include full calendar-date validation, category-wise reports, monthly filtering, CSV export, unit tests, and encrypted storage. The current program is a learning project and is not designed as a secure financial vault.
