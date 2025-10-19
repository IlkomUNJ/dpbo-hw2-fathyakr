#include "store.h"
#include <iostream>
#include <map>
#include <algorithm>
using namespace std;
Store::Store(const string &name) : storeName(name), nextOrderId(1) {}
int Store::createOrder(int buyerId, int sellerId)
{
    orders.push_back(Order(nextOrderId, buyerId, sellerId));
    return nextOrderId++;
}
Order &Store::getOrder(int index)
{
    return orders[index];
}
const vector<Order> &Store::getAllOrders() const
{
    return orders;
}
int Store::getOrderCount() const
{
    return static_cast<int>(orders.size());
}
void Store::listTransactionsLastKDays(int k) const
{
    int count = 0;
    for (size_t i = 0; i < orders.size(); ++i)
    {
        if (orders[i].isWithinDays(k) && orders[i].getStatus() == PAID)
        {
            cout << "Order #" << orders[i].getOrderId() << " - Buyer: " << orders[i].getBuyerId()
                 << ", Seller: " << orders[i].getSellerId() << ", Status: Paid, Total: $" << orders[i].getTotalAmount() << endl;
            count++;
        }
    }
    if (count == 0)
    {
        cout << "No paid transactions in the last " << k << " days." << endl;
    }
}
void Store::listPaidButIncomplete() const
{
    int count = 0;
    for (size_t i = 0; i < orders.size(); ++i)
    {
        if (orders[i].getStatus() == PAID && orders[i].getItems().empty())
        {
            cout << "Order #" << orders[i].getOrderId() << " - Buyer: " << orders[i].getBuyerId()
                 << ", Seller: " << orders[i].getSellerId() << " (Paid but no items)" << endl;
            count++;
        }
    }
    if (count == 0)
    {
        cout << "No paid orders with incomplete items." << endl;
    }
}
void Store::listTopMFrequentItems(int m) const
{
    map<int, int> itemFrequency;
    for (size_t i = 0; i < orders.size(); ++i)
    {
        const vector<OrderItem> &items = orders[i].getItems();
        for (size_t j = 0; j < items.size(); ++j)
        {
            itemFrequency[items[j].itemId] += items[j].quantity;
        }
    }
    vector<pair<int, int>> freqVec(itemFrequency.begin(), itemFrequency.end());
    sort(freqVec.begin(), freqVec.end(), [](const pair<int, int> &a, const pair<int, int> &b)
         { return a.second > b.second; });
    cout << "Top " << m << " most frequent items:\n";
    int count = 0;
    for (size_t i = 0; i < freqVec.size() && count < m; ++i, ++count)
    {
        cout << "  Item ID: " << freqVec[i].first << " - Quantity sold: " << freqVec[i].second << endl;
    }
    if (count == 0)
    {
        cout << "No items found." << endl;
    }
}
void Store::listMostActiveBuyers(int topN) const
{
    map<int, int> buyerOrderCount;
    for (size_t i = 0; i < orders.size(); ++i)
    {
        buyerOrderCount[orders[i].getBuyerId()]++;
    }
    vector<pair<int, int>> buyers(buyerOrderCount.begin(), buyerOrderCount.end());
    sort(buyers.begin(), buyers.end(), [](const pair<int, int> &a, const pair<int, int> &b)
         { return a.second > b.second; });
    cout << "Top " << topN << " most active buyers:\n";
    int count = 0;
    for (size_t i = 0; i < buyers.size() && count < topN; ++i, ++count)
    {
        cout << "  Buyer ID: " << buyers[i].first << " - Orders: " << buyers[i].second << endl;
    }
    if (count == 0)
    {
        cout << "No buyers found." << endl;
    }
}
void Store::listMostActiveSellers(int topN) const
{
    map<int, int> sellerOrderCount;
    for (size_t i = 0; i < orders.size(); ++i)
    {
        sellerOrderCount[orders[i].getSellerId()]++;
    }
    vector<pair<int, int>> sellers(sellerOrderCount.begin(), sellerOrderCount.end());
    sort(sellers.begin(), sellers.end(), [](const pair<int, int> &a, const pair<int, int> &b)
         { return a.second > b.second; });
    cout << "Top " << topN << " most active sellers:\n";
    int count = 0;
    for (size_t i = 0; i < sellers.size() && count < topN; ++i, ++count)
    {
        cout << "  Seller ID: " << sellers[i].first << " - Orders: " << sellers[i].second << endl;
    }
    if (count == 0)
    {
        cout << "No sellers found." << endl;
    }
}
