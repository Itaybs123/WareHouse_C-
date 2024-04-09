#include "Volunteer.h"
#include <iostream>
#include <sstream>

// constructor
Volunteer :: Volunteer(int id, const string &name) : id(id) , name(name) , completedOrderId(NO_ORDER) , activeOrderId(NO_ORDER) , orderPointer(0){}

// getters:
int Volunteer :: getId() const{
    return id;
}

const string & Volunteer :: getName() const{
    return name;
}

int Volunteer :: getActiveOrderId() const{
    return activeOrderId;
}

int Volunteer :: getCompletedOrderId() const{
    return completedOrderId;
}

void Volunteer :: setActiveOrderId(int orderId){ /////אולי אפשר לוותר על הפונקציות האלו
      activeOrderId = orderId;
}

void Volunteer :: setCompletedOrderId(int orderId){
      completedOrderId = orderId;
}

bool Volunteer:: isBusy() const{
    return activeOrderId!=-1;
}

// orderPointer
Order* Volunteer :: getOrderPointer() const////////////////////////////////////////////////////////
{
    return orderPointer;
}

// orderPointer
void Volunteer :: setOrderPointer(Order& newOrderPointer)////////////////////////////////////////////////////////
{
    orderPointer = &newOrderPointer;
}

//---------------------------------------------------------------------

// CollectorVolunteer Class Implementation

CollectorVolunteer :: CollectorVolunteer(int id, string name, int coolDown):Volunteer(id, name) ,coolDown(coolDown), timeLeft(0) {}

// copy constractor
CollectorVolunteer :: CollectorVolunteer(const CollectorVolunteer &other) : Volunteer(other.getId() , other.getName()) , coolDown(other.coolDown) ,timeLeft(other.timeLeft) {}

CollectorVolunteer * CollectorVolunteer :: clone() const {
    return new CollectorVolunteer(*this);
}

void CollectorVolunteer :: step() {
    if(timeLeft > 0){ //if timeLeft is 0 so the order is not in proccess
        if( decreaseCoolDown()){
            setCompletedOrderId(getActiveOrderId());   // just finish order with ActiveOrderId
            setActiveOrderId(NO_ORDER);  
        }
    }
}

int  CollectorVolunteer :: getCoolDown() const{
    return coolDown;
}

int CollectorVolunteer :: getTimeLeft() const{
    return timeLeft;
}

bool CollectorVolunteer:: decreaseCoolDown(){
    if(timeLeft!=0){
       timeLeft=timeLeft-1;}
    return timeLeft==0;
}

bool CollectorVolunteer :: hasOrdersLeft() const { //there is no limit
    return true;
}

bool CollectorVolunteer :: canTakeOrder(const Order &order) const 
{ 
    return (!isBusy());
}

void CollectorVolunteer :: acceptOrder(const Order &order)
{
    if(canTakeOrder(order))
    {
        setOrderPointer(const_cast<Order&>(order));/////////////////////////////////////////למה זה עובד?
        timeLeft = coolDown;
        setActiveOrderId(order.getId());
    }
}

string CollectorVolunteer :: toString() const {
    std::stringstream ss;
    if(isBusy){ 
        ss << "Collector Volunteer - ID: " << getId() << ", isBusy: " << isBusy() <<
        ", orderId: " << getActiveOrderId() << ", Time Left: " << timeLeft
        << ", OrderLeft: " <<"No Limit";

    }
    else{
            ss << "Collector Volunteer - ID: " << getId() << ", isBusy: " << "false" <<
        ", orderId: " << "None" << ", Time Left: " << timeLeft
        << ", OrderLeft: " <<"No Limit";
    }

    return ss.str();
}


void CollectorVolunteer :: setTimeLeft()
{
    timeLeft = coolDown;
}

int CollectorVolunteer::  WhoAmI() const{
    return 1;
}





// LimitedCollectorVolunteer
LimitedCollectorVolunteer :: LimitedCollectorVolunteer(int id, string name, int coolDown ,int maxOrders) :CollectorVolunteer(id, name ,coolDown) ,maxOrders(maxOrders), ordersLeft(maxOrders){}

LimitedCollectorVolunteer :: LimitedCollectorVolunteer(const LimitedCollectorVolunteer &other): CollectorVolunteer(other) ,maxOrders(other.maxOrders) ,ordersLeft(other.ordersLeft){}

LimitedCollectorVolunteer * LimitedCollectorVolunteer :: clone() const {
    return new LimitedCollectorVolunteer(*this);
}

bool LimitedCollectorVolunteer :: hasOrdersLeft() const { 
    return ordersLeft!=0;
}

bool LimitedCollectorVolunteer :: canTakeOrder(const Order &order) const {   
    return (!isBusy()) & hasOrdersLeft();
}

void LimitedCollectorVolunteer :: acceptOrder(const Order &order) 
{
    if(canTakeOrder(order))
    {

        setOrderPointer(const_cast<Order&>(order));
        setTimeLeft();
        setActiveOrderId(order.getId());
        ordersLeft--;
    }
}

int LimitedCollectorVolunteer :: getMaxOrders() const{
    return maxOrders;
}

int LimitedCollectorVolunteer :: getNumOrdersLeft() const{
    return ordersLeft;
}

string LimitedCollectorVolunteer :: toString() const {
    std::stringstream ss;
    if(isBusy){ 
        ss << "Collector Volunteer - ID: " << getId() << ", isBusy: " << isBusy() <<
        ", orderId: " << getActiveOrderId() << ", Time Left: " << getTimeLeft()
        << ", OrderLeft: " <<ordersLeft;

    }
    else{
            ss << "Collector Volunteer - ID: " << getId() << ", isBusy: " << "false" <<
        ", orderId: " << "None" << ", Time Left: " << getTimeLeft()
        << ", OrderLeft: " <<ordersLeft;
    }
    return ss.str();
}

int LimitedCollectorVolunteer::  WhoAmI() const{
    return 2;
}









 // DriverVolunteer Class Implementation
  
    DriverVolunteer :: DriverVolunteer(int id, string name, int maxDistance, int distancePerStep) : Volunteer(id, name),maxDistance(maxDistance),distancePerStep(distancePerStep),distanceLeft(0) {}

    DriverVolunteer :: DriverVolunteer(const DriverVolunteer &other) : Volunteer(other.getId() , other.getName()), maxDistance(other.maxDistance),distancePerStep(other.distancePerStep),distanceLeft(other.maxDistance) {}

    DriverVolunteer * DriverVolunteer :: clone() const {
        return new DriverVolunteer(*this);
    }

    int DriverVolunteer :: getDistanceLeft() const{
        return distanceLeft;
    }
        
    int DriverVolunteer :: getMaxDistance() const{
        return maxDistance;
    }

    int DriverVolunteer :: getDistancePerStep() const{
        return distancePerStep;
    }
    bool DriverVolunteer :: decreaseDistanceLeft(){
        if (distanceLeft > 0) {//only if its positive
            distanceLeft = distanceLeft - distancePerStep; 
            if(distanceLeft <= 0){ //if its is negetive after the deacrse
                distanceLeft=0;
        }}
        return distanceLeft==0;
    }

    bool DriverVolunteer :: hasOrdersLeft() const {
        return true;
    }
    bool DriverVolunteer :: canTakeOrder(const Order &order) const {//צריך include לorder?
        return (!isBusy()) & order.getDistance() <= maxDistance; 

    }
    
    void DriverVolunteer :: acceptOrder(const Order &order) 
    {
        if (canTakeOrder(order))
        { 
            setOrderPointer(const_cast<Order&>(order));
            SetDistanceLeft(order);
            distanceLeft = order.getDistance();
            setActiveOrderId(order.getId());
        }
    }// Reset distanceLeft to maxDistance

    void DriverVolunteer :: step() {
        if (distanceLeft > 0) { // if in proccess 
            if(decreaseDistanceLeft()){ // the proccess is done
                setCompletedOrderId(getActiveOrderId());  
                setActiveOrderId(NO_ORDER);
            }
        }

    } 
    int DriverVolunteer::  WhoAmI() const{
    return 3;
    }


    string DriverVolunteer :: toString() const {
        std::stringstream ss;
        if(isBusy){ 
            ss << "Driver Volunteer - ID: " << getId() << ", isBusy: " << isBusy() <<
            ", orderId: " << getActiveOrderId() << ", Distance Left: " << getDistanceLeft()
            << ", OrderLeft: " <<"No Limit";

        }
        else{
            ss << "Collector Volunteer - ID: " << getId() << ", isBusy: " << "false" <<
            ", orderId: " << "None" << ", Distance Left: " << getDistanceLeft()
            << ", OrderLeft: " <<"No Limit";
        }

        return ss.str();
    }


    void DriverVolunteer:: SetDistanceLeft(const Order &order){
        distanceLeft=order.getDistance();
    }



///Implemntation of LimitedDriverVolunteer

        LimitedDriverVolunteer :: LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep,int maxOrders) : DriverVolunteer(id, name ,distancePerStep ,maxOrders),maxOrders(maxOrders),ordersLeft(maxOrders) {}

        LimitedDriverVolunteer :: LimitedDriverVolunteer(const LimitedDriverVolunteer &other) : DriverVolunteer(other) , maxOrders(other.maxOrders),ordersLeft(other.ordersLeft) {}

        LimitedDriverVolunteer * LimitedDriverVolunteer :: clone() const {
            return new LimitedDriverVolunteer(*this);
        }

        int LimitedDriverVolunteer :: getMaxOrders() const{
            return maxOrders;
        }

        int LimitedDriverVolunteer :: getNumOrdersLeft() const{
            return ordersLeft;
        }

        bool LimitedDriverVolunteer :: hasOrdersLeft() const {
            return ordersLeft!=0;
        }

        bool LimitedDriverVolunteer :: canTakeOrder(const Order &order) const {
             return (!isBusy()) && hasOrdersLeft() && order.getDistance() <= getMaxDistance();
        }

        void LimitedDriverVolunteer :: acceptOrder(const Order &order)
        {
                if(canTakeOrder(order))
                {   setOrderPointer(const_cast<Order&>(order));
                    SetDistanceLeft(order);
                    setActiveOrderId(order.getId());
                    ordersLeft--;
                    ////////////////////////////////////////////////////////
                }
        }

        string LimitedDriverVolunteer :: toString() const {
            std::stringstream ss;
            if(isBusy){ 
                ss << "Driver Volunteer - ID: " << getId() << ", isBusy: " << isBusy() <<
                ", orderId: " << getActiveOrderId() << ", Distance Left: " << getDistanceLeft()
                << ", OrderLeft: " <<ordersLeft;

            }
            else{
                ss << "Collector Volunteer - ID: " << getId() << ", isBusy: " << "false" <<
                ", orderId: " << "None" << ", Distance Left: " << getDistanceLeft()
                << ", OrderLeft: " <<ordersLeft;
            }
             return ss.str();
      }
          int DriverVolunteer::  WhoAmI() const{
              return 4;
           }