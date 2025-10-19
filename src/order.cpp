#include "order.h"
Order::Order(int orderId, int buyerId, int sellerId)
    : orderId(orderId), buyerId(buyerId), sellerId(sellerId),
      status(PENDING), totalAmount(0.0)
{
    orderDate = time(nullptr);
}
void Order::addItem(int itemId, const string &itemName, int quantity, double pricePerUnit)
{
    OrderItem item(itemId, itemName, quantity, pricePerUnit);
    items.push_back(item);
    totalAmount += item.getTotalPrice();
}
bool Order::isWithinDays(int days) const
{
    time_t now = time(nullptr);
    double diff = difftime(now, orderDate);
    return (diff / (60 * 60 * 24)) <= days;
}
bool Order::isToday() const
{
    time_t now = time(nullptr);
    tm *nowTm = localtime(&now);
    tm *orderTm = localtime(&orderDate);
    return (nowTm->tm_year == orderTm->tm_year &&
            nowTm->tm_mon == orderTm->tm_mon &&
            nowTm->tm_mday == orderTm->tm_mday);
}
void Order::display() const
{
    cout << "\n=== Order #" << orderId << " ===" << endl;
    cout << "Status: ";
    switch (status)
    {
    case PENDING:
        cout << "Pending";
        break;
    case PAID:
        cout << "Paid";
        break;
    case COMPLETED:
        cout << "Completed";
        break;
    case CANCELLED:
        cout << "Cancelled";
        break;
    }
    cout << endl;
    cout << "Buyer ID: " << buyerId << " | Seller ID: " << sellerId << endl;
    cout << "Items:" << endl;
    for (size_t i = 0; i < items.size(); i++)
    {
        cout << "  " << (i + 1) << ". " << items[i].itemName
             << " (x" << items[i].quantity << ") - $"
             << items[i].pricePerUnit << " each = $"
             << items[i].getTotalPrice() << endl;
    }
    cout << "Total Amount: $" << totalAmount << endl;
}
