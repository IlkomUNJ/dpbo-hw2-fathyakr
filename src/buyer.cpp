#include "buyer.h"
#include <iostream>
using namespace std;
Buyer::Buyer(int id, const string &name, BankCustomer &account0)
    : id(id), name(name), account(account0) {}
void Buyer::addToCart(int itemId, const string &itemName, int sellerId, int quantity, double price)
{
    for (auto &cartItem : shoppingCart)
    {
        if (cartItem.itemId == itemId && cartItem.sellerId == sellerId)
        {
            cartItem.quantity += quantity;
            cout << "Updated quantity in cart." << endl;
            return;
        }
    }
    CartItem item(itemId, itemName, sellerId, quantity, price);
    shoppingCart.push_back(item);
    cout << "Item added to cart." << endl;
}
void Buyer::removeFromCart(int index)
{
    if (index >= 0 && index < static_cast<int>(shoppingCart.size()))
    {
        shoppingCart.erase(shoppingCart.begin() + index);
        cout << "Item removed from cart." << endl;
    }
    else
    {
        cout << "Invalid cart index." << endl;
    }
}
void Buyer::viewCart() const
{
    if (shoppingCart.empty())
    {
        cout << "\nYour cart is empty." << endl;
        return;
    }
    cout << "\n=== Shopping Cart ===" << endl;
    double total = 0.0;
    for (size_t i = 0; i < shoppingCart.size(); i++)
    {
        double itemTotal = shoppingCart[i].quantity * shoppingCart[i].price;
        cout << (i + 1) << ". " << shoppingCart[i].itemName
             << " (Seller ID: " << shoppingCart[i].sellerId << ")"
             << " - Qty: " << shoppingCart[i].quantity
             << " x $" << shoppingCart[i].price
             << " = $" << itemTotal << endl;
        total += itemTotal;
    }
    cout << "\nTotal: $" << total << endl;
}
void Buyer::clearCart()
{
    shoppingCart.clear();
}
void Buyer::addOrderToHistory(int orderIndex)
{
    orderHistory.push_back(orderIndex);
}
