#ifndef TRANSACTION_H
#define TRANSACTION_H
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
using namespace std;
enum TransactionType
{
    DEPOSIT,
    WITHDRAW,
    PURCHASE,
    PAYMENT_RECEIVED,
    REFUND
};
class Transaction
{
private:
    int id;
    int accountId;
    TransactionType type;
    double amount;
    time_t timestamp;
    string description;
public:
    Transaction(int id, int accountId, TransactionType type, double amount, const string &description)
        : id(id), accountId(accountId), type(type), amount(amount), description(description)
    {
        timestamp = time(nullptr);
    }
    int getId() const { return id; }
    int getAccountId() const { return accountId; }
    TransactionType getType() const { return type; }
    double getAmount() const { return amount; }
    time_t getTimestamp() const { return timestamp; }
    string getDescription() const { return description; }
    string getDateString() const
    {
        tm *timeinfo = localtime(&timestamp);
        stringstream ss;
        ss << put_time(timeinfo, "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }
    bool isWithinDays(int days) const
    {
        time_t now = time(nullptr);
        double diff = difftime(now, timestamp);
        return (diff / (60 * 60 * 24)) <= days;
    }
    bool isToday() const
    {
        time_t now = time(nullptr);
        tm *nowTm = localtime(&now);
        tm *transTm = localtime(&timestamp);
        return (nowTm->tm_year == transTm->tm_year &&
                nowTm->tm_mon == transTm->tm_mon &&
                nowTm->tm_mday == transTm->tm_mday);
    }
    string getTypeString() const
    {
        switch (type)
        {
        case DEPOSIT:
            return "Deposit";
        case WITHDRAW:
            return "Withdraw";
        case PURCHASE:
            return "Purchase";
        case PAYMENT_RECEIVED:
            return "Payment Received";
        case REFUND:
            return "Refund";
        default:
            return "Unknown";
        }
    }
    void display() const
    {
        cout << "Transaction ID: " << id << endl;
        cout << "Type: " << getTypeString() << endl;
        cout << "Amount: $" << amount << endl;
        cout << "Date: " << getDateString() << endl;
        cout << "Description: " << description << endl;
    }
};
#endif 
