
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

void WareHouse::start() {
    //TODO need to implement
}
void WareHouse::step() {
    for (auto it = pendingOrders.begin(); it != pendingOrders.end();) {
        for (Volunteer* volunteer : volunteers) {
            if (volunteer->canTakeOrder(**it)) {
                volunteer->acceptOrder(**it);
                inProcessOrders.push_back(*it);
                pendingOrders.erase(it);
                break;
            }
        }
    }
    for (Volunteer* volunteer : volunteers){
        if (volunteer->isBusy()){
            volunteer->step();
            if (volunteer->getActiveOrderId() == volunteer->getCompletedOrderId()){
                Order& order = getOrder(volunteer->getActiveOrderId());
                std::vector<Order*>::iterator it;
                switch(order.getStatus()){
                    case OrderStatus::COLLECTING:
                        it = std::find(inProcessOrders.begin(), inProcessOrders.end(), &order);
                        if (it != inProcessOrders.end()) {
                            pendingOrders.push_back(&order);
                            inProcessOrders.erase(it);
                        }
                        break;
                    case OrderStatus::DELIVERING:
                        order.setStatus(OrderStatus::COMPLETED);
                        it = std::find(inProcessOrders.begin(), inProcessOrders.end(), &order);
                        if (it != inProcessOrders.end()) {
                            completedOrders.push_back(&order);
                            inProcessOrders.erase(it);
                        }
                        if (!volunteer->hasOrdersLeft()){
                            deleteVolunteer(volunteer);
                        }
                        break;
                }
            }
        }
    }
}

bool WareHouse::deleteVolunteer(Volunteer* volunteerId) {
    for (auto it = volunteers.begin(); it != volunteers.end(); ++it) {
        if (*it == volunteerId) {
            delete *it;
            volunteers.erase(it);
            return true;
        }
    }
    return false;
}

WareHouse::WareHouse(const string& configFilePath) : isOpen(true), customerCounter(0), volunteerCounter(0), orderCounter(0) {
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
                    Customer* soldier = new SoldierCustomer(customerCounter, name, distance, maxOrders);
                    addCustomer(soldier);
                } else if (customerType == "civilian") {
                    Customer* civilian = new CivilianCustomer(customerCounter, name, distance, maxOrders);
                    addCustomer(civilian);
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
                        Volunteer* driver = new LimitedDriverVolunteer(volunteerCounter, name, coolDown_maxDistance, distancePerStep, maxOrders);
                        addVolunteer(driver);
                    }
                    else
                    {
                        Volunteer* driver = new DriverVolunteer(volunteerCounter, name, coolDown_maxDistance, distancePerStep);
                        addVolunteer(driver);
                    }
                    
                } else {
                    if (role.find("limited") != std::string::npos)
                    {
                        iss >> maxOrders;
                        Volunteer* collector = new LimitedCollectorVolunteer(volunteerCounter, name, coolDown_maxDistance, maxOrders);
                        addVolunteer(collector);
                    }
                    else
                    {
                        Volunteer* collector = new CollectorVolunteer(volunteerCounter, name, coolDown_maxDistance);
                        addVolunteer(collector);
                    }
                }
            }
        }
    }
}

void WareHouse::addOrder(Order* order) {
    orderCounter++;
    pendingOrders.push_back(order);
}

void WareHouse::addVolunteer(Volunteer* volunteer) {
    volunteerCounter++;
    volunteers.push_back(volunteer);
}

void WareHouse::addCustomer(Customer* customer) {
    customerCounter++;
    customers.push_back(customer);
}

void WareHouse::addAction(BaseAction* action) {
    actionsLog.push_back(action);
}

Customer& WareHouse::getCustomer(int customerId) const {
    if(customerCounter<=customerId || customerId<0) {throw std::exception();}
    for (Customer* customer : customers) {
        if (customer->getId() == customerId) {
            return *customer;
        }
    }
}

bool WareHouse::isVolunteerExist(int volunteerId) const {
    if(volunteerCounter<=volunteerId || volunteerId<0) {return false;}
    for (Volunteer* volunteer : volunteers) {
        if (volunteer->getId() == volunteerId) {
            return true;
        }
    }
    return false;
}

Volunteer& WareHouse::getVolunteer(int volunteerId) const {
    for (Volunteer* volunteer : volunteers) {
        if (volunteer->getId()==volunteerId) {
            return *volunteer;
        }
    }
}

Order& WareHouse::getOrder(int orderId) const {
    if(orderCounter<=orderId || orderId<0) {throw std::exception();}
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

int WareHouse::getOrderCounter() const {return orderCounter;}

int WareHouse::getVolunteerCounter() const {return volunteerCounter;}

int WareHouse::getCustomerCounter() const {return customerCounter;}

void WareHouse::close() {
    for (Order* order : pendingOrders) {
        std::cout << order->toString() << std::endl;
    }
    for (Order* order : inProcessOrders) {
        std::cout << order->toString() << std::endl;
    }
    for (Order* order : completedOrders) {
        std::cout << order->toString() << std::endl;
    }
    isOpen = false;
    }

void WareHouse::open() {isOpen = true;}


//rule of five

WareHouse::~WareHouse() {
    for (auto action : actionsLog) delete action;
    for (auto volunteer : volunteers) delete volunteer;
    for (auto order : pendingOrders) delete order;
    for (auto order : inProcessOrders) delete order;
    for (auto order : completedOrders) delete order;
    for (auto customer : customers) delete customer;
}

WareHouse::WareHouse(const WareHouse& other) 
    : isOpen(other.isOpen), customerCounter(other.customerCounter), volunteerCounter(other.volunteerCounter), orderCounter(other.orderCounter) {
        for (BaseAction* action : other.actionsLog) actionsLog.push_back(action->clone());
        for (Volunteer* volunteer : other.volunteers) volunteers.push_back(volunteer->clone());
        for (Order* order : other.pendingOrders) pendingOrders.push_back(new Order(*order));
        for (Order* order : other.inProcessOrders) inProcessOrders.push_back(new Order(*order));
        for (Order* order : other.completedOrders) completedOrders.push_back(new Order(*order));
        for (Customer* customer : other.customers) customers.push_back(customer->clone());
}

WareHouse& WareHouse::operator=(const WareHouse& other) {
    if (this != &other) {
        // Free current resources
        this->~WareHouse();
        
        isOpen = other.isOpen;
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        orderCounter = other.orderCounter;

        // Deep copy of pointers in vectors
        for (auto action : other.actionsLog) actionsLog.push_back(action->clone());
        for (auto volunteer : other.volunteers) volunteers.push_back(volunteer->clone());
        for (auto order : other.pendingOrders) pendingOrders.push_back(new Order(*order));
        for (auto order : other.inProcessOrders) inProcessOrders.push_back(new Order(*order));
        for (auto order : other.completedOrders) completedOrders.push_back(new Order(*order));
        for (auto customer : other.customers) customers.push_back(customer->clone());
    }
    return *this;
}

WareHouse::WareHouse(WareHouse&& other) noexcept 
    : isOpen(other.isOpen), customerCounter(other.customerCounter), volunteerCounter(other.volunteerCounter),
    orderCounter(other.orderCounter), actionsLog(std::move(other.actionsLog)), volunteers(std::move(other.volunteers)),
    pendingOrders(std::move(other.pendingOrders)), inProcessOrders(std::move(other.inProcessOrders)),
    completedOrders(std::move(other.completedOrders)), customers(std::move(other.customers)) {
        other.isOpen = false;
        other.customerCounter = 0;
        other.volunteerCounter = 0;
        other.orderCounter = 0;
}

WareHouse& WareHouse::operator=(WareHouse&& other) noexcept {
    if (this != &other) {
        this->~WareHouse();
        actionsLog = std::move(other.actionsLog);
        volunteers = std::move(other.volunteers);
        pendingOrders = std::move(other.pendingOrders);
        inProcessOrders = std::move(other.inProcessOrders);
        completedOrders = std::move(other.completedOrders);
        customers = std::move(other.customers);
        isOpen = other.isOpen;
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        orderCounter = other.orderCounter;
        other.isOpen = false;
        other.customerCounter = 0;
        other.volunteerCounter = 0;
        other.orderCounter = 0;
    }
    return *this;
}



