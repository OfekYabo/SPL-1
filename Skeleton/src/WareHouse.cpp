
#include "../include/WareHouse.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "../include/Order.h"
#include "../include/Customer.h"
#include "../include/Action.h"
#include "../include/Volunteer.h"


WareHouse::WareHouse(const std::string& configFilePath) : isOpen(true), customerCounter(0), volunteerCounter(0), orderCounter(0) {
    std::ifstream file(configFilePath);
    std::string line;
    while (std::getline(file, line)) 
    {
        if (line[0] != '#' && !line.empty()) 
        {
            std::istringstream iss(line);
            std::string type;
            iss >> type;
            if (type == "customer") 
            {
                std::string name, customerType;
                int distance, maxOrders;
                iss >> name >> customerType >> distance >> maxOrders;
                if (customerType == "soldier") {
                    Customer* soldier = new SoldierCustomer(customerCounter+1, name, distance, maxOrders);
                    customerCounter++;
                    customers.push_back(soldier);
                } else if (customerType == "civilian") {
                    Customer* civilian = new CivilianCustomer(customerCounter+1, name, distance, maxOrders);
                    customerCounter++;
                    customers.push_back(civilian);
                }
            } 
            else if (type == "volunteer") 
            {
                std::string name, role;
                int coolDown_maxDistance, maxOrders, distancePerStep;
                iss >> name >> role >> coolDown_maxDistance;
                if (role.find("driver") != std::string::npos)
                {
                    iss >> distancePerStep;
                    if (role.find("limited") != std::string::npos)
                    {
                        iss >> maxOrders;
                        Volunteer* driver = new LimitedDriverVolunteer(volunteerCounter+1, name, coolDown_maxDistance, distancePerStep, maxOrders);
                        volunteers.push_back(driver);
                    }
                    else
                    {
                        Volunteer* driver = new DriverVolunteer(volunteerCounter+1, name, coolDown_maxDistance, distancePerStep);
                        volunteers.push_back(driver);
                    }
                    
                } else {
                    if (role.find("limited") != std::string::npos)
                    {
                        iss >> maxOrders;
                        Volunteer* collector = new LimitedCollectorVolunteer(volunteerCounter+1, name, coolDown_maxDistance, maxOrders);
                        volunteerCounter++;
                        volunteers.push_back(collector);
                    }
                    else
                    {
                        Volunteer* collector = new CollectorVolunteer(volunteerCounter+1, name, coolDown_maxDistance);
                        volunteerCounter++;
                        volunteers.push_back(collector);
                    }
                }
            }
        }
    }
}

void WareHouse::addOrder(Order* order) {
    pendingOrders.push_back(order);
}
void WareHouse::addAction(BaseAction* action) {
    actionsLog.push_back(action);
}
Customer& WareHouse::getCustomer(int customerId) const {
    if(customerCounter<customerId || customerId<1) {throw std::exception();}
    for (Customer* customer : customers) {
        if (customer->getId() == customerId) {
            return *customer;
        }
    }
}
Volunteer& WareHouse::getVolunteer(int volunteerId) const {
    if(volunteerCounter<volunteerId || volunteerId<1) {throw std::exception();}
    for (Volunteer* volunteer : volunteers) {
        if (volunteer->getId()==volunteerId) {
            return *volunteer;
        }
    }
}
Order& WareHouse::getOrder(int orderId) const {
    if(orderCounter<orderId || orderId<1) {throw std::exception();}
    for (Order* order : pendingOrders) {
        if (order->getId()==orderId) {
            return *order;
        }
    }
    for (Order* order : inProcessOrders) {
        if (order->getId()==orderId) {
            return *order;
        }
    }
    for (Order* order : completedOrders) {
        if (order->getId()==orderId) {
            return *order;
        }
    }
}
const vector<BaseAction*>& WareHouse::getActions() const {return actionsLog;}
void WareHouse::close() {isOpen = false;}
void WareHouse::open() {isOpen = true;}




