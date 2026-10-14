# Personal Expense and Budget Tracking System

A lightweight, offline, menu-driven personal finance tracker written in C++17. It is designed as an educational Object-Oriented Programming (OOP) project for recording student income and expenses without an online account.

## Features

- Record income, food expenses, and travel expenses.
- Set a spending budget and receive an over-budget warning.
- View transaction history and a financial summary.
- Calculate total income, total expenses, current balance, and budget remaining.
- Persist transactions locally in `transactions.txt` and reload them on the next run.
- Validate basic menu, amount, and date-format input.
- Use an abstract base class, inheritance, encapsulation, and runtime polymorphism.

## Requirements

- A C++17-compatible compiler (GCC recommended).
- Windows with MSYS2/MinGW-w64, Linux, or macOS with a compatible compiler.

## Project structure

```text
Personal-Expense-Budget-Tracker/
├── include/
│   ├── Transaction.h
│   ├── FoodExpense.h
│   ├── TravelExpense.h
│   └── Income.h
├── src/
│   └── main.cpp
├── docs/
│   └── PROJECT_REPORT.md
├── .gitignore
└── README.md
```

## Build and run on Windows (MSYS2 UCRT64 + PowerShell)

Install GCC in the MSYS2 UCRT64 terminal if needed:

```bash
pacman -S --needed mingw-w64-ucrt-x86_64-gcc
```

In PowerShell, move to the project directory. If `g++` is not on PATH for the current terminal, use:

```powershell
$env:Path += ";C:\msys64\ucrt64\bin"
```

Compile:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic -I include src/main.cpp -o Expense_Tracker.exe
```

Run:

```powershell
.\Expense_Tracker.exe
```

If MSYS2 is installed in a different directory, adjust the path accordingly.

## Build and run on Linux/macOS

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -I include src/main.cpp -o Expense_Tracker
./Expense_Tracker
```

## Menu

1. Set Budget
2. Add Income
3. Add Food Expense
4. Add Travel Expense
5. Display Transactions
6. Financial Summary
7. Delete All Data
8. Save and Exit

The data file is created in the program's current working directory. It is deliberately excluded from Git so private financial records are not published.

## OOP concepts demonstrated

- **Abstraction:** `Transaction` is an abstract base class with pure virtual methods.
- **Inheritance:** `FoodExpense`, `TravelExpense`, and `Income` inherit from `Transaction`.
- **Encapsulation:** transaction data members are private and accessed through getters.
- **Runtime polymorphism:** the program stores derived objects using `std::unique_ptr<Transaction>` and calls virtual functions through base-class pointers.
- **File handling:** `std::ifstream` loads records and `std::ofstream` saves them locally.

## Limitations

- This is a local console application, not an encrypted financial vault. Do not store highly sensitive information in descriptions.
- Date validation checks the `DD/MM/YYYY` shape and broad numeric ranges, but does not validate every calendar date (for example, 31/02).
- Budget remaining is the spending budget minus expenses; current balance is income minus expenses. These values represent different concepts.
- The program is intended for learning and should be tested before relying on its calculations for real financial decisions.

## Academic and source attribution

If this repository was adapted from another public repository, retain the original author's attribution and follow that repository's license and reuse terms. Do not claim third-party code as exclusively original work. Document your own modifications and ensure you understand the implementation before submitting it.
