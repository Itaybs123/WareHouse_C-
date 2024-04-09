#include "Volunteer.h"

// Volunteer constructor
Volunteer :: Volunteer(int id, const string &name) : completedOrderId(NO_ORDER) , activeOrderId(NO_ORDER),  id(id) , name(name){}



// methods
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

bool Volunteer:: isBusy() const{
    return activeOrderId != NO_ORDER;
}

void Volunteer :: setActiveOrderId(int orderId){
      activeOrderId = orderId;
}

void Volunteer :: setCompletedOrderId(int orderId){
      completedOrderId = orderId;
}

//---------------------------------------------------------------------

// CollectorVolunteer Class Implementation

// CollectorVolunteer constructor
CollectorVolunteer :: CollectorVolunteer(int id,const string &name, int coolDown) : Volunteer(id, name), coolDown(coolDown), timeLeft(0){}



// methods
CollectorVolunteer* CollectorVolunteer :: clone() const {
    return new CollectorVolunteer(*this);
}

void CollectorVolunteer :: step(){
    // if timeLeft is 0 there is nothing to do because the order is not in proccess
    if(timeLeft > 0) // the volunteer is in a middle of a proccess
    { 
        if(decreaseCoolDown()) // timeLeft = 0, so it means the volunteer just finished a task right now
        {
            setCompletedOrderId(getActiveOrderId()); // just finish order with ActiveOrderId
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
    if(timeLeft != 0)
    {
       timeLeft--;
    }
    return timeLeft == 0;
}

bool CollectorVolunteer :: hasOrdersLeft() const {
    return true;
}

bool CollectorVolunteer :: canTakeOrder(const Order &order) const 
{    
    
    return (!isBusy());
}

void CollectorVolunteer :: acceptOrder(const Order &order)
{
        timeLeft = coolDown;
        setActiveOrderId(order.getId());
}

string CollectorVolunteer::toString() const {
    return "Collector Volunteer - ID: " + std::to_string(getId()) + "\n"
         + "isBusy: " + (isBusy() ? "true" : "false") + "\n"
         + "Order ID: " + (isBusy() ? std::to_string(getActiveOrderId()) : "None") + "\n"
         + "Time Left: " + (isBusy() ? std::to_string(getTimeLeft()) : "None") + "\n"
         + "Orders Left: No Limit";
}

void CollectorVolunteer :: setTimeLeft()
{
    timeLeft = coolDown;
}

int CollectorVolunteer::  WhoAmI() const{
    return 1;
}

//---------------------------------------------------------------------------------------------

// class LimitedCollectorVolunteer: public CollectorVolunteer

// LimitedCollectorVolunteer constractor
LimitedCollectorVolunteer :: LimitedCollectorVolunteer(int id,const string &name, int coolDown ,int maxOrders) : CollectorVolunteer(id, name ,coolDown) , maxOrders(maxOrders), ordersLeft(maxOrders){}


// methods
LimitedCollectorVolunteer * LimitedCollectorVolunteer :: clone() const {
    return new LimitedCollectorVolunteer(*this);
}

bool LimitedCollectorVolunteer :: hasOrdersLeft() const { 
    return ordersLeft != 0;
}

bool LimitedCollectorVolunteer :: canTakeOrder(const Order &order) const {   
   //////// std::cout << "i was here " << std::endl;
    return (!isBusy()) && hasOrdersLeft();
}

void LimitedCollectorVolunteer :: acceptOrder(const Order &order) {
    if(canTakeOrder(order))
    {
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

string LimitedCollectorVolunteer::toString() const {
    return "Limited Collector Volunteer - ID: " + std::to_string(getId()) + "\n"
         + "isBusy: " + (isBusy() ? "true" : "false") + "\n"
         + "Order ID: " + (isBusy() ? std::to_string(getActiveOrderId()) : "None") + "\n"
         + "Time Left: " + (isBusy() ? std::to_string(getTimeLeft()) : "None") + "\n"
         + "Orders Left: " + std::to_string(getNumOrdersLeft());
}

int LimitedCollectorVolunteer::  WhoAmI() const{
    return 2;
}

//---------------------------------------------------------------------------------------------

// class DriverVolunteer: public Volunteer

// DriverVolunteer constractor
DriverVolunteer :: DriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep) : Volunteer(id, name), maxDistance(maxDistance), distancePerStep(distancePerStep), distanceLeft(0){}


// methods
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
    if (distanceLeft > 0) // only if it is positive
    {
        distanceLeft -= distancePerStep; 
        if(distanceLeft < 0)
        {
            distanceLeft = 0;
        }
    }
    return distanceLeft == 0;
}

bool DriverVolunteer :: hasOrdersLeft() const {
    return true;
}

bool DriverVolunteer :: canTakeOrder(const Order &order) const {
    return (!isBusy()) && order.getDistance() <= maxDistance; 
}

void DriverVolunteer :: acceptOrder(const Order &order) {
    if(canTakeOrder(order))
    { 
        distanceLeft = order.getDistance();
        setActiveOrderId(order.getId());
    }
} 

void DriverVolunteer::step() {
    if (distanceLeft > 0) // Check if the driver is currently in the middle of a delivery process, indicated by a distanceLeft greater than 0.
    { 
        // Attempt to decrease the remaining distance for the current task.
        // If decreaseDistanceLeft() returns true, it indicates that the driving task
        // has been completed (distanceLeft has reached 0).
        if(decreaseDistanceLeft()) 
        { 
            setCompletedOrderId(getActiveOrderId()); // Mark the order corresponding to the active order ID as completed.
            setActiveOrderId(NO_ORDER); // Reset the active order ID to indicate that there is no longer an active delivery task.
        }
    }
    // If distanceLeft is 0, no action is taken as it implies there is no ongoing delivery task.
}

int DriverVolunteer::  WhoAmI() const{
    return 3;
}

string DriverVolunteer::toString() const {
    return "Driver Volunteer - ID: " + std::to_string(getId()) + "\n"
         + "isBusy: " + (isBusy() ? "true" : "false") + "\n"
         + "Order ID: " + (isBusy() ? std::to_string(getActiveOrderId()) : "None") + "\n"
         + "Distance Left: " + (isBusy() ? std::to_string(getDistanceLeft()) : "None") + "\n"
         + "Order Left: No Limit";
}

void DriverVolunteer:: SetDistanceLeft(const Order &order){
    distanceLeft = order.getDistance();
}

//---------------------------------------------------------------------------------------------

// class LimitedDriverVolunteer: public DriverVolunteer

// LimitedDriverVolunteer constractor
LimitedDriverVolunteer :: LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep, int maxOrders) : DriverVolunteer(id, name ,distancePerStep ,maxOrders), maxOrders(maxOrders), ordersLeft(maxOrders) {}



// methods
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
    return ordersLeft != 0;
}

bool LimitedDriverVolunteer :: canTakeOrder(const Order &order) const {
    return !isBusy() && hasOrdersLeft() && order.getDistance() <= getMaxDistance();
}

void LimitedDriverVolunteer :: acceptOrder(const Order &order)
{
        if(canTakeOrder(order))
        {  
            SetDistanceLeft(order);
            setActiveOrderId(order.getId());
            ordersLeft--;
        }
}

string LimitedDriverVolunteer::toString() const {
    return "Limited Driver Volunteer - ID: " + std::to_string(getId()) + "\n"
         + "isBusy: " + (isBusy() ? "true" : "false") + "\n"
         + "Order ID: " + (isBusy() ? std::to_string(getActiveOrderId()) : "None") + "\n"
         + "Distance Left: " + (isBusy() ? std::to_string(getDistanceLeft()) : "None") + "\n"
         + "Orders Left: " + std::to_string(getNumOrdersLeft());
}

int LimitedDriverVolunteer :: WhoAmI() const{
    return 4;
}
