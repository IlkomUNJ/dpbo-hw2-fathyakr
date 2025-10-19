#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "bank_customer.h"
#include "buyer.h"
#include "seller.h"
#include "bank.h"
#include "order.h"
#include "store.h"
#include "handlers.h"

enum PrimaryPrompt
{
    LOGIN,
    REGISTER,
    STORE_ANALYTICS,
    EXIT
};

using namespace std;
vector<BankCustomer> bankCustomers;
vector<Buyer> buyers;
vector<seller> sellers;
Bank mainBank("Bank BCA");
Store mainStore("NVM Store");
int currentBuyerId = -1;
bool currentUserIsSeller = false;

// Serialization functions
void saveData();
void loadData();
void saveBankCustomers();
void loadBankCustomers();
void saveBuyers();
void loadBuyers();
void saveSellers();
void loadSellers();

int main()
{
    // Load data at program start
    loadData();

    PrimaryPrompt prompt = LOGIN;
    while (prompt != EXIT)
    {
        cout << "\n========================================" << endl;
        cout << "   ONLINE MARKETPLACE SYSTEM" << endl;
        cout << "========================================" << endl;
        cout << "1. Login" << endl;
        cout << "2. Register" << endl;
        cout << "3. Store Analytics (Admin)" << endl;
        cout << "4. Exit" << endl;
        cout << "Select an option: ";
        int choice;
        cin >> choice;
        prompt = static_cast<PrimaryPrompt>(choice - 1);
        switch (prompt)
        {
        case LOGIN:
            handleLogin();
            break;
        case REGISTER:
            handleRegister();
            break;
        case STORE_ANALYTICS:
            handleStoreAnalytics();
            break;
        case EXIT:
            cout << "Thank you for using our system. Goodbye!" << endl;
            // Save data before exiting
            saveData();
            break;
        default:
            cout << "Invalid option." << endl;
            break;
        }
    }
    return 0;
}

// Serialization Implementation
void saveData()
{
    cout << "Saving data..." << endl;
    saveBankCustomers();
    saveBuyers();
    saveSellers();
    cout << "Data saved successfully!" << endl;
}

void loadData()
{
    cout << "Loading data..." << endl;
    loadBankCustomers();
    loadBuyers();
    loadSellers();
    cout << "Data loaded successfully!" << endl;
}

void saveBankCustomers()
{
    ofstream file("bank_customers.dat");
    if (!file.is_open())
    {
        cerr << "Error: Could not open bank_customers.dat for writing" << endl;
        return;
    }

    file << bankCustomers.size() << endl;
    for (const auto &customer : bankCustomers)
    {
        file << customer.getId() << endl;
        file << customer.getName() << endl;
        file << customer.getBalance() << endl;
    }
    file.close();
}

void loadBankCustomers()
{
    ifstream file("bank_customers.dat");
    if (!file.is_open())
    {
        cout << "No existing bank customer data found. Starting fresh." << endl;
        return;
    }

    bankCustomers.clear();
    int count;
    file >> count;
    file.ignore();

    for (int i = 0; i < count; i++)
    {
        int id;
        string name;
        double balance;

        file >> id;
        file.ignore();
        getline(file, name);
        file >> balance;
        file.ignore();

        BankCustomer customer(id, name, balance);
        bankCustomers.push_back(customer);
    }
    file.close();
}

void saveBuyers()
{
    ofstream file("buyers.dat");
    if (!file.is_open())
    {
        cerr << "Error: Could not open buyers.dat for writing" << endl;
        return;
    }

    file << buyers.size() << endl;
    for (const auto &buyer : buyers)
    {
        file << buyer.getId() << endl;
        file << buyer.getName() << endl;
    }
    file.close();
}

void loadBuyers()
{
    ifstream file("buyers.dat");
    if (!file.is_open())
    {
        cout << "No existing buyer data found. Starting fresh." << endl;
        return;
    }

    buyers.clear();
    int count;
    file >> count;
    file.ignore();

    for (int i = 0; i < count; i++)
    {
        int id;
        string name;

        file >> id;
        file.ignore();
        getline(file, name);

        // Find corresponding bank customer
        bool found = false;
        for (auto &bankCustomer : bankCustomers)
        {
            if (bankCustomer.getId() == id)
            {
                Buyer buyer(id, name, bankCustomer);
                buyers.push_back(buyer);
                found = true;
                break;
            }
        }

        if (!found)
        {
            cerr << "Warning: Bank customer not found for buyer ID " << id << endl;
        }
    }
    file.close();
}

void saveSellers()
{
    ofstream file("sellers.dat");
    if (!file.is_open())
    {
        cerr << "Error: Could not open sellers.dat for writing" << endl;
        return;
    }

    file << sellers.size() << endl;
    for (const auto &seller : sellers)
    {
        file << seller.getId() << endl;
        file << seller.getName() << endl;
        file << seller.getSellerId() << endl;
        file << seller.getStoreName() << endl;
    }
    file.close();
}

void loadSellers()
{
    ifstream file("sellers.dat");
    if (!file.is_open())
    {
        cout << "No existing seller data found. Starting fresh." << endl;
        return;
    }

    sellers.clear();
    int count;
    file >> count;
    file.ignore();

    for (int i = 0; i < count; i++)
    {
        int id, sellerId;
        string name;
        string storeName;

        file >> id;
        file.ignore();
        getline(file, name);
        file >> sellerId;
        file.ignore();
        getline(file, storeName);

        // Find corresponding buyer
        bool found = false;
        for (auto &buyer : buyers)
        {
            if (buyer.getId() == id)
            {
                seller newSeller(buyer, sellerId, storeName);
                sellers.push_back(newSeller);
                found = true;
                break;
            }
        }

        if (!found)
        {
            cerr << "Warning: Buyer not found for seller ID " << id << endl;
        }
    }
    file.close();
}