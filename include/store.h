#ifndef STORE_H
#define STORE_H
#include <string>
#include <vector>
#include "order.h"
using namespace std;
class Store
{
private:
    string storeName;
    vector<Order> orders;
    int nextOrderId;

public:
    Store(const string &name);
    int createOrder(int buyerId, int sellerId);
    Order &getOrder(int index);
    const vector<Order> &getAllOrders() const;
    int getOrderCount() const;
    void listTransactionsLastKDays(int k) const;
    void listPaidButIncomplete() const;
    void listTopMFrequentItems(int m) const;
    void listMostActiveBuyers(int topN) const;
    void listMostActiveSellers(int topN) const;
};
#endif
