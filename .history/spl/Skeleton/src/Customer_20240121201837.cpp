#include "Customer.h"
#include <iostream>

//לבדוק אם צריך לפתוח מחלקה

Customer::Customer(int id, const string& name, int locationDistance, int maxOrders)
 : id(id), name(name), locationDistance(locationDistance), maxOrders(maxOrders),ordersId(), orderCounter(0) {
}

Customer :: ~Customer(){    /// why it is without Virtual
    if (!ordersId.empty()) {
        ordersId.clear();}
}
Customer :: Customer(const Customer &other): id(other.id), name(other.name), locationDistance(other.locationDistance), maxOrders(other.maxOrders),ordersId(other.ordersId.size()), orderCounter(other.orderCounter) {
    for (int orderId : other.ordersId) {
    this->ordersId.push_back(orderId); // Assuming a function getOrderById exists
}
}

const string& Customer::getName() const {
    return name;
}

int Customer::getId() const {
    return id;
}

int Customer::getCustomerDistance() const {
    return locationDistance;
}

int Customer::getMaxOrders() const {
    return maxOrders;
}

int Customer::getNumOrders() const {
    return orderCounter;
}

bool Customer::canMakeOrder() const {
    return getNumOrders() < maxOrders;
}

vector<int>& Customer::getOrders() const {
    return const_cast<vector<int>&>(ordersId); //?????????????????????????
}


int Customer::addOrder(int orderId) {
    if (canMakeOrder()) {
        ordersId.push_back(orderId);
        orderCounter++;
        return orderId;
    } else {
        return -1; 
    }
}

//class SoldierCustomer: public Customer { checkkkkkkk

    SoldierCustomer::SoldierCustomer(int id, string name, int locationDistance, int maxOrders)//לבדוק אם יותר יעיל באופן ישיר
        : Customer(id, name, locationDistance, maxOrders) {
    }
    SoldierCustomer :: SoldierCustomer(const SoldierCustomer &other) : Customer(other.getId(), other.getName(), other.getCustomerDistance(),other.getMaxOrders() ){}
    SoldierCustomer* SoldierCustomer::clone() const {///////)
        return new SoldierCustomer(*this);/// explain why to tom
    }

//};

CivilianCustomer::CivilianCustomer(int id, string name, int locationDistance, int maxOrders)
    : Customer(id, name, locationDistance, maxOrders) {
}

CivilianCustomer :: CivilianCustomer(const CivilianCustomer &other) : Customer(other.getId(), other.getName(), other.getCustomerDistance(),other.getMaxOrders() ){

}
CivilianCustomer* CivilianCustomer::clone() const {
   return new CivilianCustomer(*this);


}

////// check where i need write copy constactor and distractor