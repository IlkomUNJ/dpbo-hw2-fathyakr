#include "seller.h"
using namespace std;
seller::seller(Buyer buyer, int sellerId, const std::string &storeName)
    : Buyer(buyer.getId(), buyer.getName(), buyer.getAccount()),
      sellerId(sellerId), storeName(storeName)
{
    Buyer::setId(buyer.getId());
}
bool seller::addNewItem(int newId, const std::string &newName, int newQuantity, double newPrice)
{
    if (itemExists(newId))
    {
        std::cout << "Error: Item with ID " << newId << " already exists!" << std::endl;
        return false;
    }
    Item newItem(newId, newName, newQuantity, newPrice);
    items.push_back(newItem);
    return true;
}
void seller::updateItem(int itemId, const std::string &newName, int newQuantity, double newPrice)
{
    for (auto &item : items)
    {
        if (item.getId() == itemId)
        {
            item.alterItemById(itemId, newName, newQuantity, newPrice);
        }
    }
}
void seller::displayInventory() const
{
    if (items.empty())
    {
        std::cout << "Inventory is empty." << std::endl;
        return;
    }
    std::cout << "\n--- Inventory List ---" << std::endl;
    std::cout << "Total items: " << items.size() << std::endl;
    for (size_t i = 0; i < items.size(); i++)
    {
        std::cout << "\nItem #" << (i + 1) << std::endl;
        std::cout << "  ID: " << items[i].getId() << std::endl;
        std::cout << "  Name: " << items[i].getName() << std::endl;
        std::cout << "  Quantity: " << items[i].getQuantity() << std::endl;
        std::cout << "  Price: $" << items[i].getPrice() << std::endl;
    }
}
bool seller::itemExists(int itemId) const
{
    for (const auto &item : items)
    {
        if (item.getId() == itemId)
        {
            return true;
        }
    }
    return false;
}
Item *seller::getItemById(int itemId)
{
    for (auto &item : items)
    {
        if (item.getId() == itemId)
        {
            return &item;
        }
    }
    return nullptr;
}
bool seller::replenishItem(int itemId, int quantity)
{
    for (auto &item : items)
    {
        if (item.getId() == itemId)
        {
            item.setQuantity(item.getQuantity() + quantity);
            cout << "Item replenished. New quantity: " << item.getQuantity() << endl;
            return true;
        }
    }
    cout << "Item not found!" << endl;
    return false;
}
bool seller::discardItem(int itemId, int quantity)
{
    for (auto &item : items)
    {
        if (item.getId() == itemId)
        {
            if (item.getQuantity() >= quantity)
            {
                item.setQuantity(item.getQuantity() - quantity);
                cout << "Item discarded. Remaining quantity: " << item.getQuantity() << endl;
                return true;
            }
            else
            {
                cout << "Not enough quantity to discard!" << endl;
                return false;
            }
        }
    }
    cout << "Item not found!" << endl;
    return false;
}
bool seller::setItemPrice(int itemId, double newPrice)
{
    for (auto &item : items)
    {
        if (item.getId() == itemId)
        {
            item.setPrice(newPrice);
            cout << "Price updated to $" << newPrice << endl;
            return true;
        }
    }
    cout << "Item not found!" << endl;
    return false;
}
bool seller::reduceItemQuantity(int itemId, int quantity)
{
    for (auto &item : items)
    {
        if (item.getId() == itemId)
        {
            if (item.getQuantity() >= quantity)
            {
                item.setQuantity(item.getQuantity() - quantity);
                return true;
            }
            return false;
        }
    }
    return false;
}
