#include "../include/Order.h"

Order::Order(int id, int customerId, int distance) : id(id), customerId(customerId), distance(distance) 
{
    driverId = NO_VOLUNTEER;
    collectorId = NO_VOLUNTEER;
}
Order::Order(int id, int customerId, int distance, int collectorId, int driverId) 
    : id(id), customerId(customerId), distance(distance), driverId(driverId), collectorId(collectorId) {} 

int Order::getId() const 
{
    return id;
}

int Order::getCustomerId() const 
{
    return customerId;
}

void Order::setStatus(OrderStatus status) 
{
    this->status = status;
}

void Order::setCollectorId(int collectorId)
{
    this->collectorId = collectorId;
}

void Order::setDriverId(int driverId)
{
    this->driverId = driverId;
}

int Order::getCollectorId() const 
{
    return collectorId;
}

int Order::getDriverId() const 
{
    return driverId;
}

int Order::getDistance() const {
    return distance;
}
OrderStatus Order::getStatus() const
{
    return status;
}
const string Order::toString() const
{
    return  "OrderID: " + std::to_string(id)
        +  ", OrderStatus: " + getStatusString() 
        +  ", CustomerID: " + std::to_string(customerId)
        +  ", Collector: " + std::to_string(collectorId)
        +  ", Driver: " + std::to_string(driverId);
} 
