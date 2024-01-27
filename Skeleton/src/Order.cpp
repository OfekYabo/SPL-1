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
    status = status;
}

void Order::setCollectorId(int collectorId)
{
    collectorId = collectorId;
}

void Order::setDriverId(int driverId)
{
    driverId = driverId;
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
    return "Order ID: " + std::to_string(getId())
        +  ", Customer ID: " + std::to_string(getCustomerId())
        +  ", Status: " + OrderStatusToString.at(getStatus()) 
        +  ", Collector ID: " + std::to_string(getCollectorId())
        +  ", Driver ID: " + std::to_string(getDriverId());
} 
