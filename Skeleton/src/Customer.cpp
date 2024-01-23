#include "../include/Customer.h"

Customer::Customer(int id, const string &name, int locationDistance, int maxOrders) 
    : id(id), name(name), locationDistance(locationDistance), maxOrders(maxOrders) { }

const string& Customer::getName() const {return name;}
int Customer::getId() const {return id;}
int Customer::getCustomerDistance() const {return locationDistance;}
int Customer::getMaxOrders() const {return maxOrders;}
int Customer::getNumOrders() const {return ordersId.size();} //Returns num of orders the customer has made so far
bool Customer::canMakeOrder() const {return (getNumOrders() < maxOrders);} //Returns true if the customer didn't reach max orders
const vector<int>& Customer::getOrdersIds() const {return ordersId;}




int Customer::addOrder(int orderId) { //return OrderId if order was added successfully, -1 otherwise
    if(canMakeOrder()) {ordersId.push_back(orderId);}
    else return -1;
    } 

