#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

// Abstract base class shared by every financial transaction.
class Transaction {
private:
    int id_;
    std::string date_;
    std::string description_;
    double amount_;

public:
    Transaction(int id, const std::string& date,
                const std::string& description, double amount)
        : id_(id), date_(date), description_(description), amount_(amount) {}

    virtual ~Transaction() = default;

    int getId() const { return id_; }
    const std::string& getDate() const { return date_; }
    const std::string& getDescription() const { return description_; }
    double getAmount() const { return amount_; }

    // Pure virtual methods make Transaction abstract.
    virtual std::string getType() const = 0;
    virtual double getSignedAmount() const = 0;
    virtual void display() const = 0;
};

#endif
