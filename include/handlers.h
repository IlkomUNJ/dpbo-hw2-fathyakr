#ifndef HANDLERS_H
#define HANDLERS_H

#include <vector>
#include <string>
#include "buyer.h"
#include "seller.h"
#include "bank_customer.h"
#include "bank.h"
#include "store.h"

// External references to global data
extern std::vector<BankCustomer> bankCustomers;
extern std::vector<Buyer> buyers;
extern std::vector<seller> sellers;
extern Bank mainBank;
extern Store mainStore;
extern int currentBuyerId;
extern bool currentUserIsSeller;

// Handler function declarations
void handleLogin();
void handleRegister();
void showBuyerMenu();
void handleSellerUpgrade();
void handleBankFunctions();
void handleShoppingMenu();
void handleSellerInventory();
void handleSellerMenu();
void handleBrowseShop();
void handleViewCart();
void handleCheckout();
void handleManageOrders();
void handleViewSpending();
void handleStoreAnalytics();
void handleSellerAnalytics();

// Utility functions
bool findBuyerByName(const std::string &name, int &buyerIndex);
int findSellerIndexByBuyerId(int buyerId);
void clearInputBuffer();

#endif
