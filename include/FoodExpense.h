#ifndef FOOD_EXPENSE_H
#define FOOD_EXPENSE_H

#include "Transaction.h"
#include <iomanip>
#include <iostream>

class FoodExpense : public Transaction {
public:
    FoodExpense(int id, const std::string& date,
                const std::string& description, double amount)
        : Transaction(id, date, description, amount) {}

    std::string getType() const override { return "FOOD"; }
    double getSignedAmount() const override { return -getAmount(); }

    void display() const override {
        std::cout << std::left << std::setw(6) << getId()
                  << std::setw(15) << getDate()
                  << std::setw(15) << getType()
                  << std::setw(28) << getDescription()
                  << std::right << std::fixed << std::setprecision(2)
                  << getAmount() << '\n';
    }
};

#endif
