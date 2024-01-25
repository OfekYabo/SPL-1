#include "../include/Volunteer.h"
#include "../include/Order.h"


Volunteer::Volunteer(int id, const string &name) : id(id), name(name) 
{
    completedOrderId = -1;
    activeOrderId = -1;
}

Volunteer::Volunteer(int id, const string &name, int completedOrderId, int activeOrderId) : id(id), name(name), completedOrderId(completedOrderId), activeOrderId(activeOrderId) {}

int Volunteer::getId() const
{
    return id;
}

const string& Volunteer::getName() const 
{
    return name;
}

int Volunteer::getActiveOrderId() const 
{
    return activeOrderId;
}

bool Volunteer::isBusy() const
{
    return (activeOrderId != -1);
}

CollectorVolunteer::CollectorVolunteer(int id, const string &name, const int coolDown) : Volunteer(id, name) , coolDown(coolDown)
{}
CollectorVolunteer* CollectorVolunteer::clone() const {}
void CollectorVolunteer::step()
{

}
int CollectorVolunteer::getCoolDown() const
{
    return coolDown;
}

int CollectorVolunteer::getTimeLeft() const 
{
    return timeLeft;
}

bool CollectorVolunteer::decreaseCoolDown() //Decrease timeLeft by 1,return true if timeLeft=0,false otherwise
{
    timeLeft = timeLeft - 1;
    return (timeLeft == 0);
}

bool CollectorVolunteer::hasOrdersLeft() const  // Signal whether the volunteer didn't reach orders limit,Always true for CollectorVolunteer and DriverVolunteer
{
    return true;
} 

bool CollectorVolunteer::canTakeOrder(const Order &order) const
{
    return order.getStatus() == OrderStatus::PENDING;
}
void CollectorVolunteer::acceptOrder(const Order &order) {}
string CollectorVolunteer::toString() const {}
