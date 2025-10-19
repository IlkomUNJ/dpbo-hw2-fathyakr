#pragma once
#include "buyer.h"
#include "item.h"
#include <string>
#include <vector>
#include <iostream>
class seller : public Buyer
{
private:
    int sellerId;
    std::string storeName;
    vector<Item> items;

public:
    seller() = default;
    seller(
        Buyer buyer, 
        int sellerId, 
        const std::string &storeName
    );
    virtual ~seller() = default;
    bool addNewItem(int newId, const std::string &newName, int newQuantity, double newPrice);
    void updateItem(int itemId, const std::string &newName, int newQuantity, double newPrice);
    int getSellerId() const { return sellerId; }
    std::string getStoreName() const { return storeName; }
    void setStoreName(const std::string &newStoreName) { storeName = newStoreName; }
    void displayInventory() const;
    int getItemsCount() const { return items.size(); }
    bool itemExists(int itemId) const;
    Item *getItemById(int itemId);
    bool replenishItem(int itemId, int quantity);
    bool discardItem(int itemId, int quantity);
    bool setItemPrice(int itemId, double newPrice);
    bool reduceItemQuantity(int itemId, int quantity);
    const vector<Item> &getItems() const { return items; }
};
