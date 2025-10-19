#include "handlers.h"
#include "transaction.h"
#include "order.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <map>

using namespace std;

enum BuyerMenu
{
    ACCOUNT_STATUS,
    UPGRADE_SELLER,
    BANK_FUNCTIONS,
    SHOPPING_MENU,
    MANAGE_ORDERS,
    VIEW_SPENDING,
    SELLER_MODE,
    LOGOUT_BUYER
};

enum ShoppingMenu
{
    BROWSE_SHOP,
    VIEW_CART,
    CHECKOUT,
    BACK_TO_BUYER_MENU
};

enum BankMenu
{
    VIEW_BALANCE,
    BANK_DEPOSIT,
    BANK_WITHDRAW,
    VIEW_TRANSACTIONS,
    BACK_TO_BUYER
};

enum SellerInventoryMenu
{
    VIEW_INVENTORY,
    ADD_ITEM,
    UPDATE_ITEM,
    SET_PRICE,
    ADD_STOCK,
    DISCARD_STOCK,
    BACK_TO_MAIN_MENU
};

void handleLogin()
{
    cout << "=== LOGIN ===" << endl;
    string name;
    cout << "Enter your name: ";
    cin.ignore();
    getline(cin, name);
    int buyerIndex;
    if (findBuyerByName(name, buyerIndex))
    {
        currentBuyerId = buyerIndex;
        cout << "Login successful! Welcome, " << name << endl;
        for (size_t i = 0; i < sellers.size(); i++)
        {
            if (sellers[i].getName() == name)
            {
                currentUserIsSeller = true;
                break;
            }
        }
        showBuyerMenu();
    }
    else
    {
        cout << "User not found. Please register first." << endl;
    }
}

void handleRegister()
{
    cout << "=== REGISTRATION ===" << endl;
    string name;
    cout << "Enter your name: ";
    cin.ignore();
    getline(cin, name);
    double initialDeposit;
    cout << "Enter initial deposit amount: $";
    cin >> initialDeposit;
    int newId = bankCustomers.size() + 1;
    BankCustomer newBankCustomer(newId, name, initialDeposit);
    bankCustomers.push_back(newBankCustomer);
    mainBank.addAccount(&bankCustomers.back());
    mainBank.recordTransaction(newId, DEPOSIT, initialDeposit, "Initial deposit");
    Buyer newBuyer(newId, name, bankCustomers.back());
    buyers.push_back(newBuyer);
    cout << "\nRegistration successful!" << endl;
    cout << "Bank account created with balance: $" << initialDeposit << endl;
    char createSeller;
    cout << "Do you want to create a seller account? (y/n): ";
    cin >> createSeller;
    if (createSeller == 'y' || createSeller == 'Y')
    {
        cout << "Enter store name: ";
        cin.ignore();
        string storeName;
        getline(cin, storeName);
        seller newSeller(newBuyer, newId, storeName);
        sellers.push_back(newSeller);
        currentUserIsSeller = true;
        cout << "Seller account created successfully!" << endl;
    }
    currentBuyerId = buyers.size() - 1;
    cout << "You are now logged in!" << endl;
    showBuyerMenu();
}

void showBuyerMenu()
{
    BuyerMenu buyerChoice = ACCOUNT_STATUS;
    while (buyerChoice != LOGOUT_BUYER)
    {
        cout << "\n========================================" << endl;
        cout << "   BUYER MENU" << endl;
        cout << "========================================" << endl;
        cout << "1. View Status" << endl;
        cout << "2. Upgrade Account to Seller" << endl;
        cout << "3. Banking Menu" << endl;
        cout << "4. Shopping Menu" << endl;
        cout << "5. View Orders" << endl;
        cout << "6. View Spending Analysis" << endl;
        if (currentUserIsSeller)
        {
            cout << "7. Change to Seller Mode" << endl;
            cout << "8. Logout" << endl;
        }
        else
        {
            cout << "7. Logout" << endl;
        }
        cout << "Select an option: ";
        int choice;
        cin >> choice;
        
        // Adjust for non-seller users (option 7 is logout, not seller mode)
        if (!currentUserIsSeller && choice == 7)
        {
            buyerChoice = LOGOUT_BUYER;
        }
        else
        {
            buyerChoice = static_cast<BuyerMenu>(choice - 1);
        }
        
        switch (buyerChoice)
        {
        case ACCOUNT_STATUS:
            cout << "\n=== ACCOUNT STATUS ===" << endl;
            cout << "Name: " << buyers[currentBuyerId].getName() << endl;
            cout << "Account Type: Buyer";
            if (currentUserIsSeller)
            {
                cout << " & Seller";
                for (size_t i = 0; i < sellers.size(); i++)
                {
                    if (sellers[i].getName() == buyers[currentBuyerId].getName())
                    {
                        cout << "\n--- Store Information ---" << endl;
                        cout << "Store Name: " << sellers[i].getStoreName() << endl;
                        break;
                    }
                }
            }
            cout << endl;
            buyers[currentBuyerId].getAccount().printInfo();
            break;
        case UPGRADE_SELLER:
            handleSellerUpgrade();
            break;
        case BANK_FUNCTIONS:
            handleBankFunctions();
            break;
        case SHOPPING_MENU:
            handleShoppingMenu();
            break;
        case MANAGE_ORDERS:
            handleManageOrders();
            break;
        case VIEW_SPENDING:
            handleViewSpending();
            break;
        case SELLER_MODE:
            if (currentUserIsSeller)
            {
                handleSellerMenu();
            }
            else
            {
                if (buyers[currentBuyerId].getAccount().getBalance() <= 0)
                {
                    cout << "You need funds in your bank account to become a seller!" << endl;
                    break;
                }
                handleSellerUpgrade();
            }
            break;
        case LOGOUT_BUYER:
            cout << "Logging out..." << endl;
            currentBuyerId = -1;
            currentUserIsSeller = false;
            break;
        default:
            cout << "Invalid option." << endl;
            break;
        }
    }
}

void handleSellerUpgrade()
{
    if (currentUserIsSeller)
    {
        cout << "You already have a seller account!" << endl;
        return;
    }
    if (buyers[currentBuyerId].getAccount().getBalance() <= 0)
    {
        cout << "You need a bank account with funds to upgrade to seller!" << endl;
        return;
    }
    cout << "\n=== SELLER UPGRADE ===" << endl;
    cout << "Enter store name: ";
    cin.ignore();
    string storeName;
    getline(cin, storeName);
    seller newSeller(buyers[currentBuyerId], buyers[currentBuyerId].getId(), storeName);
    sellers.push_back(newSeller);
    currentUserIsSeller = true;
    cout << "Seller account created successfully!" << endl;
    cout << "Store: " << storeName << endl;
}

void handleBankFunctions()
{
    BankMenu bankChoice = VIEW_BALANCE;
    while (bankChoice != BACK_TO_BUYER)
    {
        cout << "\n=== BANKING FUNCTIONS ===" << endl;
        cout << "1. View Balance" << endl;
        cout << "2. Top-up" << endl;
        cout << "3. Withdraw" << endl;
        cout << "4. View Transaction" << endl;
        cout << "5. Back to Buyer Menu" << endl;
        int choice;
        cin >> choice;
        bankChoice = static_cast<BankMenu>(choice - 1);
        switch (bankChoice)
        {
        case VIEW_BALANCE:
            cout << "\n=== BALANCE ===" << endl;
            buyers[currentBuyerId].getAccount().printInfo();
            break;
        case BANK_DEPOSIT:
        {
            double amount;
            cout << "Enter deposit amount: $";
            cin >> amount;
            if (amount > 0)
            {
                buyers[currentBuyerId].getAccount().addBalance(amount);
                mainBank.recordTransaction(buyers[currentBuyerId].getId(), DEPOSIT,
                                           amount, "Deposit to account");
                cout << "Deposit successful! New balance: $"
                     << buyers[currentBuyerId].getAccount().getBalance() << endl;
            }
            else
            {
                cout << "Invalid amount!" << endl;
            }
        }
        break;
        case BANK_WITHDRAW:
        {
            double amount;
            cout << "Enter withdrawal amount: $";
            cin >> amount;
            if (buyers[currentBuyerId].getAccount().withdrawBalance(amount))
            {
                mainBank.recordTransaction(buyers[currentBuyerId].getId(), WITHDRAW,
                                           amount, "Withdrawal from account");
                cout << "Withdrawal successful! New balance: $"
                     << buyers[currentBuyerId].getAccount().getBalance() << endl;
            }
        }
        break;
        case VIEW_TRANSACTIONS:
        {
            int days;
            cout << "View transactions for:\n1. Today\n2. Last 30 days\n3. All time\nChoice: ";
            int transChoice;
            cin >> transChoice;
            vector<Transaction> transactions;
            if (transChoice == 1)
            {
                days = 1;
                transactions = mainBank.getAccountTransactionsLastKDays(
                    buyers[currentBuyerId].getId(), days);
                cout << "\n=== Transactions Today ===" << endl;
            }
            else if (transChoice == 2)
            {
                days = 30;
                transactions = mainBank.getAccountTransactionsLastKDays(
                    buyers[currentBuyerId].getId(), days);
                cout << "\n=== Transactions Last 30 Days ===" << endl;
            }
            else
            {
                transactions = mainBank.getAccountTransactions(
                    buyers[currentBuyerId].getId());
                cout << "\n=== All Transactions ===" << endl;
            }
            if (transactions.empty())
            {
                cout << "No transactions found." << endl;
            }
            else
            {
                for (const auto &trans : transactions)
                {
                    trans.display();
                }
                cout << "Total transactions: " << transactions.size() << endl;
            }
        }
        break;
        case BACK_TO_BUYER:
            cout << "Returning to buyer menu..." << endl;
            break;
        default:
            cout << "Invalid option." << endl;
            break;
        }
    }
}

void handleShoppingMenu()
{
    ShoppingMenu shoppingChoice = BROWSE_SHOP;
    while (shoppingChoice != BACK_TO_BUYER_MENU)
    {
        cout << "\n========================================" << endl;
        cout << "   SHOPPING MENU" << endl;
        cout << "========================================" << endl;
        cout << "1. Shop & Browse" << endl;
        cout << "2. View Cart" << endl;
        cout << "3. Checkout" << endl;
        cout << "4. Back to Buyer Menu" << endl;
        cout << "Select an option: ";
        int choice;
        cin >> choice;
        shoppingChoice = static_cast<ShoppingMenu>(choice - 1);
        
        switch (shoppingChoice)
        {
        case BROWSE_SHOP:
            handleBrowseShop();
            break;
        case VIEW_CART:
            handleViewCart();
            break;
        case CHECKOUT:
            handleCheckout();
            break;
        case BACK_TO_BUYER_MENU:
            cout << "Returning to buyer menu..." << endl;
            break;
        default:
            cout << "Invalid option." << endl;
            break;
        }
    }
}

void handleSellerInventory()
{
    int sellerIndex = findSellerIndexByBuyerId(buyers[currentBuyerId].getId());
    if (sellerIndex == -1)
    {
        cout << "Error: Seller account not found!" << endl;
        return;
    }
    SellerInventoryMenu inventoryChoice = VIEW_INVENTORY;
    while (inventoryChoice != BACK_TO_MAIN_MENU)
    {
        cout << "\n=== INVENTORY MANAGEMENT ===" << endl;
        cout << "Store: " << sellers[sellerIndex].getStoreName() << endl;
        cout << "1. View Items" << endl;
        cout << "2. Add Item" << endl;
        cout << "3. Update Item" << endl;
        cout << "4. Set Item Price" << endl;
        cout << "5. Replenish Stock" << endl;
        cout << "6. Discard Stock" << endl;
        cout << "7. Back" << endl;
        int choice;
        cin >> choice;
        inventoryChoice = static_cast<SellerInventoryMenu>(choice - 1);
        switch (inventoryChoice)
        {
        case VIEW_INVENTORY:
        {
            cout << "\n=== CURRENT INVENTORY ===" << endl;
            sellers[sellerIndex].displayInventory();
            break;
        }
        case ADD_ITEM:
        {
            cout << "\n=== ADD NEW ITEM ===" << endl;
            int itemId;
            string itemName;
            int quantity;
            double price;
            cout << "Enter Item ID: ";
            cin >> itemId;
            cin.ignore();
            cout << "Enter Item Name: ";
            getline(cin, itemName);
            cout << "Enter Quantity: ";
            cin >> quantity;
            cout << "Enter Price: $";
            cin >> price;
            if (itemId <= 0)
            {
                cout << "Error: Item ID must be positive!" << endl;
                break;
            }
            if (quantity < 0)
            {
                cout << "Error: Quantity cannot be negative!" << endl;
                break;
            }
            if (price < 0)
            {
                cout << "Error: Price cannot be negative!" << endl;
                break;
            }
            if (itemName.empty())
            {
                cout << "Error: Item name cannot be empty!" << endl;
                break;
            }
            if (sellers[sellerIndex].addNewItem(itemId, itemName, quantity, price))
            {
                cout << "Item added successfully!" << endl;
                cout << "Item ID: " << itemId << endl;
                cout << "Name: " << itemName << endl;
                cout << "Quantity: " << quantity << endl;
                cout << "Price: $" << price << endl;
            }
            break;
        }
        case UPDATE_ITEM:
        {
            cout << "\n=== UPDATE ITEM ===" << endl;
            int itemId;
            string newName;
            int newQuantity;
            double newPrice;
            cout << "Enter Item ID to update: ";
            cin >> itemId;
            if (!sellers[sellerIndex].itemExists(itemId))
            {
                cout << "Error: Item with ID " << itemId << " not found!" << endl;
                break;
            }
            cin.ignore();
            cout << "Enter new Item Name: ";
            getline(cin, newName);
            cout << "Enter new Quantity: ";
            cin >> newQuantity;
            cout << "Enter new Price: $";
            cin >> newPrice;
            if (newQuantity < 0)
            {
                cout << "Error: Quantity cannot be negative!" << endl;
                break;
            }
            if (newPrice < 0)
            {
                cout << "Error: Price cannot be negative!" << endl;
                break;
            }
            if (newName.empty())
            {
                cout << "Error: Item name cannot be empty!" << endl;
                break;
            }
            sellers[sellerIndex].updateItem(itemId, newName, newQuantity, newPrice);
            cout << "Item updated successfully!" << endl;
            cout << "Item ID: " << itemId << endl;
            cout << "New Name: " << newName << endl;
            cout << "New Quantity: " << newQuantity << endl;
            cout << "New Price: $" << newPrice << endl;
            break;
        }
        case SET_PRICE:
        {
            int itemId;
            double newPrice;
            cout << "\n=== SET ITEM PRICE ===" << endl;
            cout << "Enter Item ID: ";
            cin >> itemId;
            cout << "Enter new Price: $";
            cin >> newPrice;
            sellers[sellerIndex].setItemPrice(itemId, newPrice);
            break;
        }
        case ADD_STOCK:
        {
            int itemId, quantity;
            cout << "\n=== REPLENISH STOCK ===" << endl;
            cout << "Enter Item ID: ";
            cin >> itemId;
            cout << "Enter quantity to add: ";
            cin >> quantity;
            sellers[sellerIndex].replenishItem(itemId, quantity);
            break;
        }
        case DISCARD_STOCK:
        {
            int itemId, quantity;
            cout << "\n=== DISCARD STOCK ===" << endl;
            cout << "Enter Item ID: ";
            cin >> itemId;
            cout << "Enter quantity to discard: ";
            cin >> quantity;
            sellers[sellerIndex].discardItem(itemId, quantity);
            break;
        }
        case BACK_TO_MAIN_MENU:
            cout << "Returning..." << endl;
            break;
        default:
            cout << "Invalid option." << endl;
            break;
        }
    }
}

void handleBrowseShop()
{
    if (sellers.empty())
    {
        cout << "\nNo sellers available yet." << endl;
        return;
    }
    cout << "\n=== BROWSE STORES ===" << endl;
    for (size_t i = 0; i < sellers.size(); i++)
    {
        cout << (i + 1) << ". " << sellers[i].getStoreName()
             << " (Seller ID: " << sellers[i].getId() << ")" << endl;
    }
    cout << "\nSelect a store (0 to cancel): ";
    int storeChoice;
    cin >> storeChoice;
    if (storeChoice < 1 || storeChoice > static_cast<int>(sellers.size()))
    {
        return;
    }
    int sellerIdx = storeChoice - 1;
    sellers[sellerIdx].displayInventory();
    cout << "\nAdd item to cart? (y/n): ";
    char addChoice;
    cin >> addChoice;
    if (addChoice == 'y' || addChoice == 'Y')
    {
        int itemId, quantity;
        cout << "Enter Item ID: ";
        cin >> itemId;
        cout << "Enter Quantity: ";
        cin >> quantity;
        Item *item = sellers[sellerIdx].getItemById(itemId);
        if (item && item->getQuantity() >= quantity)
        {
            buyers[currentBuyerId].addToCart(itemId, item->getName(),
                                             sellers[sellerIdx].getId(),
                                             quantity, item->getPrice());
        }
        else if (item)
        {
            cout << "Not enough stock! Available: " << item->getQuantity() << endl;
        }
        else
        {
            cout << "Item not found!" << endl;
        }
    }
}

void handleViewCart()
{
    buyers[currentBuyerId].viewCart();
    if (buyers[currentBuyerId].getCart().empty())
    {
        return;
    }
    cout << "\nRemove item? (y/n): ";
    char removeChoice;
    cin >> removeChoice;
    if (removeChoice == 'y' || removeChoice == 'Y')
    {
        int index;
        cout << "Enter item number to remove: ";
        cin >> index;
        buyers[currentBuyerId].removeFromCart(index - 1);
    }
}

void handleCheckout()
{
    const vector<CartItem> &cart = buyers[currentBuyerId].getCart();
    if (cart.empty())
    {
        cout << "\nYour cart is empty!" << endl;
        return;
    }
    buyers[currentBuyerId].viewCart();
    double total = 0.0;
    for (const auto &item : cart)
    {
        total += item.quantity * item.price;
    }
    cout << "\nProceed with checkout? (y/n): ";
    char confirm;
    cin >> confirm;
    if (confirm != 'y' && confirm != 'Y')
    {
        return;
    }
    if (buyers[currentBuyerId].getAccount().getBalance() < total)
    {
        cout << "\nInsufficient balance! Your balance: $"
             << buyers[currentBuyerId].getAccount().getBalance() << endl;
        return;
    }
    map<int, vector<CartItem>> itemsBySeller;
    for (const auto &item : cart)
    {
        itemsBySeller[item.sellerId].push_back(item);
    }
    for (const auto &sellerItems : itemsBySeller)
    {
        int sellerId = sellerItems.first;
        int orderIdx = mainStore.createOrder(buyers[currentBuyerId].getId(), sellerId);
        Order &order = mainStore.getOrder(orderIdx);
        for (const auto &item : sellerItems.second)
        {
            order.addItem(item.itemId, item.itemName, item.quantity, item.price);
            for (size_t i = 0; i < sellers.size(); i++)
            {
                if (sellers[i].getId() == sellerId)
                {
                    sellers[i].reduceItemQuantity(item.itemId, item.quantity);
                    break;
                }
            }
        }
        double orderTotal = order.getTotalAmount();
        buyers[currentBuyerId].getAccount().withdrawBalance(orderTotal);
        for (size_t i = 0; i < buyers.size(); i++)
        {
            if (buyers[i].getId() == sellerId)
            {
                buyers[i].getAccount().addBalance(orderTotal);
                break;
            }
        }
        mainBank.recordTransaction(buyers[currentBuyerId].getId(), PURCHASE,
                                   orderTotal, "Purchase - Order #" + to_string(order.getOrderId()));
        mainBank.recordTransaction(sellerId, PAYMENT_RECEIVED,
                                   orderTotal, "Sale - Order #" + to_string(order.getOrderId()));
        order.setStatus(PAID);
        buyers[currentBuyerId].addOrderToHistory(orderIdx);
        cout << "\nOrder #" << order.getOrderId() << " created and paid successfully!" << endl;
    }
    buyers[currentBuyerId].clearCart();
    cout << "\nCheckout complete! Total paid: $" << total << endl;
    cout << "New balance: $" << buyers[currentBuyerId].getAccount().getBalance() << endl;
}

void handleManageOrders()
{
    int choice = 0;
    while (choice != 6)
    {
        cout << "\n=== MANAGE ORDERS ===" << endl;
        cout << "1. View All Orders" << endl;
        cout << "2. View Pending Orders" << endl;
        cout << "3. View Paid Orders" << endl;
        cout << "4. View Completed Orders" << endl;
        cout << "5. View Cancelled Orders" << endl;
        cout << "6. Back" << endl;
        cout << "Select an option: ";
        cin >> choice;
        const vector<int> &orderHistory = buyers[currentBuyerId].getOrderHistory();
        const vector<Order> &allOrders = mainStore.getAllOrders();
        OrderStatus filter;
        string statusName;
        switch (choice)
        {
        case 1:
            filter = static_cast<OrderStatus>(-1);
            statusName = "All";
            break;
        case 2:
            filter = PENDING;
            statusName = "Pending";
            break;
        case 3:
            filter = PAID;
            statusName = "Paid";
            break;
        case 4:
            filter = COMPLETED;
            statusName = "Completed";
            break;
        case 5:
            filter = CANCELLED;
            statusName = "Cancelled";
            break;
        case 6:
            continue;
        default:
            cout << "Invalid option." << endl;
            continue;
        }
        cout << "\n=== Your Orders (" << statusName << ") ===" << endl;
        int count = 0;
        for (int orderIdx : orderHistory)
        {
            if (orderIdx < static_cast<int>(allOrders.size()))
            {
                const Order &order = allOrders[orderIdx];
                if (choice == 1 || order.getStatus() == filter)
                {
                    order.display();
                    count++;
                }
            }
        }
        if (count == 0)
            cout << "No orders found." << endl;
        else
            cout << "Total: " << count << " orders" << endl;
    }
}

void handleViewSpending()
{
    int days;
    cout << "\nEnter number of days to analyze: ";
    cin >> days;
    double spending = 0.0;
    const vector<int> &orderHistory = buyers[currentBuyerId].getOrderHistory();
    const vector<Order> &allOrders = mainStore.getAllOrders();
    for (int orderIdx : orderHistory)
    {
        if (orderIdx < static_cast<int>(allOrders.size()))
        {
            const Order &order = allOrders[orderIdx];
            if (order.isWithinDays(days) &&
                (order.getStatus() == PAID || order.getStatus() == COMPLETED))
            {
                spending += order.getTotalAmount();
            }
        }
    }
    cout << "\nTotal spending in last " << days << " days: $" << spending << endl;
    vector<Transaction> transactions = mainBank.getAccountTransactionsLastKDays(
        buyers[currentBuyerId].getId(), days);
    cout << "\n--- Cash Flow (Last " << days << " Days) ---" << endl;
    double totalCredit = 0, totalDebit = 0;
    for (const auto &trans : transactions)
    {
        if (trans.getType() == DEPOSIT || trans.getType() == PAYMENT_RECEIVED ||
            trans.getType() == REFUND)
        {
            totalCredit += trans.getAmount();
        }
        else
        {
            totalDebit += trans.getAmount();
        }
    }
    cout << "Total Credit: $" << totalCredit << endl;
    cout << "Total Debit: $" << totalDebit << endl;
    cout << "Net: $" << (totalCredit - totalDebit) << endl;
}

void handleSellerMenu()
{
    int sellerIdx = findSellerIndexByBuyerId(buyers[currentBuyerId].getId());
    if (sellerIdx == -1)
    {
        cout << "Error: Seller account not found!" << endl;
        return;
    }
    int choice = 0;
    while (choice != 6)
    {
        cout << "\n========================================" << endl;
        cout << "   SELLER MENU - " << sellers[sellerIdx].getStoreName() << endl;
        cout << "========================================" << endl;
        cout << "1. View Inventory" << endl;
        cout << "2. View Orders" << endl;
        cout << "3. Complete Order" << endl;
        cout << "4. Analytics & Reports" << endl;
        cout << "5. View Revenue" << endl;
        cout << "6. Back to Buyer Menu" << endl;
        cout << "Select an option: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            handleSellerInventory();
            break;
        case 2:
        {
            cout << "\n=== Your Orders ===" << endl;
            const vector<Order> &allOrders = mainStore.getAllOrders();
            int count = 0;
            for (const auto &order : allOrders)
            {
                if (order.getSellerId() == sellers[sellerIdx].getId())
                {
                    order.display();
                    count++;
                }
            }
            if (count == 0)
                cout << "No orders yet." << endl;
            break;
        }
        case 3:
        {
            int orderId;
            cout << "\nEnter Order ID to complete: ";
            cin >> orderId;
            bool found = false;
            for (int i = 0; i < mainStore.getOrderCount(); i++)
            {
                Order &order = mainStore.getOrder(i);
                if (order.getOrderId() == orderId &&
                    order.getSellerId() == sellers[sellerIdx].getId())
                {
                    if (order.getStatus() == PAID)
                    {
                        order.setStatus(COMPLETED);
                        cout << "Order #" << orderId << " marked as completed!" << endl;
                    }
                    else
                    {
                        cout << "Order must be paid before completion!" << endl;
                    }
                    found = true;
                    break;
                }
            }
            if (!found)
                cout << "Order not found or doesn't belong to you!" << endl;
            break;
        }
        case 4:
            handleSellerAnalytics();
            break;
        case 5:
        {
            int days;
            cout << "\nEnter number of days to analyze: ";
            cin >> days;
            double revenue = 0.0;
            const vector<Order> &allOrders = mainStore.getAllOrders();
            for (const auto &order : allOrders)
            {
                if (order.getSellerId() == sellers[sellerIdx].getId() &&
                    order.isWithinDays(days) &&
                    (order.getStatus() == PAID || order.getStatus() == COMPLETED))
                {
                    revenue += order.getTotalAmount();
                }
            }
            cout << "\nTotal revenue in last " << days << " days: $" << revenue << endl;
            break;
        }
        case 6:
            cout << "Returning to buyer menu..." << endl;
            break;
        default:
            cout << "Invalid option." << endl;
            break;
        }
    }
}

void handleSellerAnalytics()
{
    int sellerIdx = findSellerIndexByBuyerId(buyers[currentBuyerId].getId());
    if (sellerIdx == -1)
        return;
    int choice = 0;
    while (choice != 3)
    {
        cout << "\n=== SELLER ANALYTICS ===" << endl;
        cout << "1. Top Items This Month" << endl;
        cout << "2. Loyal Customers This Month" << endl;
        cout << "3. Back" << endl;
        cout << "Select an option: ";
        cin >> choice;
        const vector<Order> &allOrders = mainStore.getAllOrders();
        switch (choice)
        {
        case 1:
        {
            int k;
            cout << "Enter number of top items to display: ";
            cin >> k;
            cout << "\n=== Top " << k << " Most Popular Items This Month ===" << endl;
            map<int, pair<string, int>> itemSales;
            for (const auto &order : allOrders)
            {
                if (order.getSellerId() == sellers[sellerIdx].getId() &&
                    order.isWithinDays(30))
                {
                    for (const auto &item : order.getItems())
                    {
                        if (itemSales.find(item.itemId) == itemSales.end())
                        {
                            itemSales[item.itemId] = make_pair(item.itemName, 0);
                        }
                        itemSales[item.itemId].second += item.quantity;
                    }
                }
            }
            vector<pair<int, pair<string, int>>> itemVec(itemSales.begin(), itemSales.end());
            sort(itemVec.begin(), itemVec.end(),
                 [](const pair<int, pair<string, int>> &a, const pair<int, pair<string, int>> &b)
                 {
                     return a.second.second > b.second.second;
                 });
            int displayCount = min(k, static_cast<int>(itemVec.size()));
            for (int i = 0; i < displayCount; i++)
            {
                cout << (i + 1) << ". Item ID: " << itemVec[i].first
                     << " - " << itemVec[i].second.first
                     << " (Sold: " << itemVec[i].second.second << " units)" << endl;
            }
            if (itemVec.empty())
                cout << "No sales this month." << endl;
            break;
        }
        case 2:
        {
            int n;
            cout << "Enter number of top customers to display: ";
            cin >> n;
            cout << "\n=== Top " << n << " Loyal Customers This Month ===" << endl;
            map<int, int> customerPurchases;
            for (const auto &order : allOrders)
            {
                if (order.getSellerId() == sellers[sellerIdx].getId() &&
                    order.isWithinDays(30) &&
                    (order.getStatus() == PAID || order.getStatus() == COMPLETED))
                {
                    customerPurchases[order.getBuyerId()]++;
                }
            }
            vector<pair<int, int>> customerVec(customerPurchases.begin(), customerPurchases.end());
            sort(customerVec.begin(), customerVec.end(),
                 [](const pair<int, int> &a, const pair<int, int> &b)
                 {
                     return a.second > b.second;
                 });
            int displayCount = min(n, static_cast<int>(customerVec.size()));
            for (int i = 0; i < displayCount; i++)
            {
                cout << (i + 1) << ". Buyer ID: " << customerVec[i].first
                     << " - Purchases: " << customerVec[i].second << endl;
            }
            if (customerVec.empty())
                cout << "No customers this month." << endl;
            break;
        }
        case 3:
            break;
        default:
            cout << "Invalid option." << endl;
            break;
        }
    }
}

void handleStoreAnalytics()
{
    int choice = 0;
    while (choice != 7)
    {
        cout << "\n=== STORE ANALYTICS (ADMIN) ===" << endl;
        cout << "1. Transactions in Last K Days" << endl;
        cout << "2. Paid but Incomplete Orders" << endl;
        cout << "3. Most Frequent Items" << endl;
        cout << "4. Most Active Buyers Today" << endl;
        cout << "5. Most Active Sellers Today" << endl;
        cout << "6. Bank Analytics" << endl;
        cout << "7. Back" << endl;
        cout << "Select an option: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
        {
            int k;
            cout << "Enter number of days: ";
            cin >> k;
            mainStore.listTransactionsLastKDays(k);
            break;
        }
        case 2:
            mainStore.listPaidButIncomplete();
            break;
        case 3:
        {
            int m;
            cout << "Enter number of top items: ";
            cin >> m;
            mainStore.listTopMFrequentItems(m);
            break;
        }
        case 4:
        {
            int n;
            cout << "Enter number of top buyers: ";
            cin >> n;
            mainStore.listMostActiveBuyers(n);
            break;
        }
        case 5:
        {
            int n;
            cout << "Enter number of top sellers: ";
            cin >> n;
            mainStore.listMostActiveSellers(n);
            break;
        }
        case 6:
        {
            int bankChoice = 0;
            while (bankChoice != 5)
            {
                cout << "\n=== BANK ANALYTICS ===" << endl;
                cout << "1. Transactions Within a Week" << endl;
                cout << "2. All Bank Customers" << endl;
                cout << "3. Dormant Accounts" << endl;
                cout << "4. Top Users Today" << endl;
                cout << "5. Back" << endl;
                cout << "Select an option: ";
                cin >> bankChoice;
                switch (bankChoice)
                {
                case 1:
                    mainBank.listWeekTransactions();
                    break;
                case 2:
                    mainBank.listAllCustomers();
                    break;
                case 3:
                    mainBank.listDormantAccounts();
                    break;
                case 4:
                {
                    int n;
                    cout << "Enter number of top users: ";
                    cin >> n;
                    mainBank.listTopUsersToday(n);
                    break;
                }
                case 5:
                    break;
                default:
                    cout << "Invalid option." << endl;
                    break;
                }
            }
            break;
        }
        case 7:
            break;
        default:
            cout << "Invalid option." << endl;
            break;
        }
    }
}

bool findBuyerByName(const string &name, int &buyerIndex)
{
    for (size_t i = 0; i < buyers.size(); i++)
    {
        if (buyers[i].getName() == name)
        {
            buyerIndex = i;
            return true;
        }
    }
    return false;
}

int findSellerIndexByBuyerId(int buyerId)
{
    for (size_t i = 0; i < sellers.size(); i++)
    {
        if (sellers[i].getId() == buyerId)
        {
            return i;
        }
    }
    return -1;
}

void clearInputBuffer()
{
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
