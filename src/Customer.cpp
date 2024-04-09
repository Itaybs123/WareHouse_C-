#include "Customer.h"

// Customer constructor
Customer :: Customer(int id, const string& name, int locationDistance, int maxOrders) : id(id), name(name), locationDistance(locationDistance), maxOrders(maxOrders), ordersId(), orderCounter(0) {}


// Customer methods
const string& Customer::getName() const {
    return name;
}

int Customer :: getId() const {
    return id;
}

int Customer :: getCustomerDistance() const {
    return locationDistance;
}

int Customer :: getMaxOrders() const {
    return maxOrders;
}

int Customer :: getNumOrders() const {
    return orderCounter;
}

bool Customer :: canMakeOrder() const {
    return orderCounter < maxOrders;
}

const vector<int>& Customer :: getOrdersIds() const {
    return ordersId;
}

int Customer :: addOrder(int orderId) {
    if (canMakeOrder()) 
    {
        ordersId.push_back(orderId);
        orderCounter++;
        return orderId;
    } 
    else 
    {
        return -1; 
    }
}

//---------------------------------------------------------------------------------------------

//class SoldierCustomer: public Customer 

// SoldierCustomer constructor
SoldierCustomer :: SoldierCustomer(int id, const string &name, int locationDistance, int maxOrders) : Customer(id, name, locationDistance, maxOrders){}


// SoldierCustomer methods
SoldierCustomer* SoldierCustomer :: clone() const {
    return new SoldierCustomer(*this);
}

//---------------------------------------------------------------------------------------------

// class CivilianCustomer: public Customer 

// CivilianCustomer constructor
CivilianCustomer::CivilianCustomer(int id, const string &name, int locationDistance, int maxOrders) : Customer(id, name, locationDistance, maxOrders) {}

// CivilianCustomer methods
CivilianCustomer* CivilianCustomer :: clone() const {
    return new CivilianCustomer(*this);
}
