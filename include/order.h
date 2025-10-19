#ifndef ORDER_H
#define ORDER_H
#include <string>
#include <vector>
#include <ctime>
#include <iostream>
using namespace std;
enum OrderStatus
{
    PENDING,
    PAID,
    COMPLETED,
    CANCELLED
};
struct OrderItem
{
    int itemId;
    string itemName;
    int quantity;
    double pricePerUnit;
    OrderItem(int id, const string &name, int qty, double price)
        : itemId(id), itemName(name), quantity(qty), pricePerUnit(price) {}
    double getTotalPrice() const
    {
        return quantity * pricePerUnit;
    }
};
class Order
{
private:
    int orderId;
    int buyerId;
    int sellerId;
    vector<OrderItem> items;
    OrderStatus status;
    time_t orderDate;
    double totalAmount;
public:
    Order(int orderId, int buyerId, int sellerId);
    int getOrderId() const { return orderId; }
    int getBuyerId() const { return buyerId; }
    int getSellerId() const { return sellerId; }
    OrderStatus getStatus() const { return status; }
    double getTotalAmount() const { return totalAmount; }
    const vector<OrderItem> &getItems() const { return items; }
    void setStatus(OrderStatus newStatus) { status = newStatus; }
    void addItem(int itemId, const string &itemName, int quantity, double pricePerUnit);
    bool isWithinDays(int days) const;
    bool isToday() const;
    void display() const;
};
#endif
