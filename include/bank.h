#ifndef BANK_H
#define BANK_H
#include "bank_customer.h"
#include "transaction.h"
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
class Bank
{
private:
    string name;
    vector<BankCustomer *> accounts;
    vector<Transaction> transactions;
    int customerCount;
    int nextTransactionId;

public:
    Bank(const string &name) : name(name), customerCount(0), nextTransactionId(1) {}
    void addAccount(BankCustomer *account)
    {
        accounts.push_back(account);
        customerCount++;
    }
    void recordTransaction(int accountId, TransactionType type, double amount, const string &description)
    {
        Transaction trans(nextTransactionId++, accountId, type, amount, description);
        transactions.push_back(trans);
    }
    void listWeekTransactions() const
    {
        cout << "\n=== Transactions Within Last Week ===" << endl;
        int count = 0;
        for (const auto &trans : transactions)
        {
            if (trans.isWithinDays(7))
            {
                trans.display();
                cout << "---" << endl;
                count++;
            }
        }
        if (count == 0)
        {
            cout << "No transactions in the last week." << endl;
        }
        else
        {
            cout << "Total: " << count << " transactions" << endl;
        }
    }
    void listAllCustomers() const
    {
        cout << "\n=== All Bank Customers ===" << endl;
        if (accounts.empty())
        {
            cout << "No customers found." << endl;
            return;
        }
        for (size_t i = 0; i < accounts.size(); i++)
        {
            cout << "\nCustomer #" << (i + 1) << endl;
            accounts[i]->printInfo();
            cout << "---" << endl;
        }
        cout << "Total customers: " << accounts.size() << endl;
    }
    void listDormantAccounts() const
    {
        cout << "\n=== Dormant Accounts (No Transaction in 30 Days) ===" << endl;
        map<int, bool> activeAccounts;
        for (const auto &trans : transactions)
        {
            if (trans.isWithinDays(30))
            {
                activeAccounts[trans.getAccountId()] = true;
            }
        }
        int dormantCount = 0;
        for (const auto &account : accounts)
        {
            if (activeAccounts.find(account->getId()) == activeAccounts.end())
            {
                cout << "\nDormant Account:" << endl;
                account->printInfo();
                cout << "---" << endl;
                dormantCount++;
            }
        }
        if (dormantCount == 0)
        {
            cout << "No dormant accounts found." << endl;
        }
        else
        {
            cout << "Total dormant accounts: " << dormantCount << endl;
        }
    }
    void listTopUsersToday(int n) const
    {
        cout << "\n=== Top " << n << " Users by Transaction Count Today ===" << endl;
        map<int, int> transactionCount;
        for (const auto &trans : transactions)
        {
            if (trans.isToday())
            {
                transactionCount[trans.getAccountId()]++;
            }
        }
        vector<pair<int, int>> countVec(transactionCount.begin(), transactionCount.end());
        sort(countVec.begin(), countVec.end(),
             [](const pair<int, int> &a, const pair<int, int> &b)
             {
                 return a.second > b.second;
             });
        int displayCount = min(n, static_cast<int>(countVec.size()));
        for (int i = 0; i < displayCount; i++)
        {
            cout << (i + 1) << ". Account ID: " << countVec[i].first
                 << " - Transactions: " << countVec[i].second << endl;
            for (const auto &account : accounts)
            {
                if (account->getId() == countVec[i].first)
                {
                    cout << "   Name: " << account->getName() << endl;
                    cout << "   Balance: $" << account->getBalance() << endl;
                    break;
                }
            }
        }
        if (countVec.empty())
        {
            cout << "No transactions today." << endl;
        }
    }
    int getCustomerCount() const
    {
        return customerCount;
    }
    vector<Transaction> getAccountTransactions(int accountId) const
    {
        vector<Transaction> accountTrans;
        for (const auto &trans : transactions)
        {
            if (trans.getAccountId() == accountId)
            {
                accountTrans.push_back(trans);
            }
        }
        return accountTrans;
    }
    vector<Transaction> getAccountTransactionsLastKDays(int accountId, int k) const
    {
        vector<Transaction> accountTrans;
        for (const auto &trans : transactions)
        {
            if (trans.getAccountId() == accountId && trans.isWithinDays(k))
            {
                accountTrans.push_back(trans);
            }
        }
        return accountTrans;
    }
};
#endif