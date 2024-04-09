#include "WareHouse.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
#include "BaseAction.h"




WareHouse::WareHouse(const string &configFilePath): actionsLog(), volunteers(), pendingOrders(), inProcessOrders(), completedOrders(), customers(), volunteerCounter(0), customerCounter(0),ordercounter(0), isOpen(true)
, noSuchCustomer(new SoldierCustomer(-1 , "" ,-1 , -1)) , noSuchVolunteer(new LimitedCollectorVolunteer(-1,"",-1 , -1)) , noSuchOrder(new Order(-1,-1,-1)){ ////
noSuchOrder->setStatus(OrderStatus::COLLECTING);
    // Member initializer list: Initializing various member variables and vectors to default values
    // actionsLog, volunteers, pendingOrders, vol, completedOrders, and customers vectors are initialized as empty vectorsjjhujnk
    // volunteerCounter, customerCounter, and isOpen are initialized with specific values
    // This is part of the constructor initialization.

    // Open the configuration file
    std::ifstream configFile(configFilePath);
    if (!configFile.is_open()) {
        std::cerr << "Error: Unable to open configuration file." << std::endl;
        exit(EXIT_FAILURE);
    }
    
    std::string line;
    while (std::getline(configFile, line)) {
        // Read each line from the configuration file
        std::istringstream iss(line);
        std::string keyword;
        iss >> keyword ;
        parserLine(iss, keyword , true);
    }

    // Close the configuration file
    configFile.close();
}

void WareHouse :: start(){///not shure at all
    std::cout << "Welcome to the Warehouse!" << std::endl;

    while (isOpen) {
    std::cout << "Enter an action (e.g., 'addCustomer', 'addVolunteer', 'exit'): ";
    
    std::string userCommand;
    std::cin >> userCommand;  // Read user input into a string variable
    std::cout << "You entered: " << userCommand << std::endl;
    std :: istringstream iss(userCommand);
    std::string key;
    iss >> key;
    parserLine(iss,key,false);


    }
}


// A helper method for the constructor
void WareHouse :: parserLine(std::istringstream& iss ,std::string key , bool itsPhars){
    if (key == "volunteer")
    {
        Volunteer *newVolunteer = parseVolunteer(iss);
        volunteers.push_back(newVolunteer);
    }
    else if (key == "customer"){
        BaseAction *newCustomer = parseCustomer(iss);
        if(!(itsPhars)){
             addAction(newCustomer);} }
    else if(key=="step"){
        BaseAction *step = parseStep(iss);
        addAction(step);}
    else if(key == "order"){
        BaseAction *order = parseOrder(iss);
        addAction(order);}
    else if(key=="orderStatus"){
        BaseAction *orderStatus = parseOrderStatus(iss);
        addAction(orderStatus);}
    else if(key=="customerStatus"){
        BaseAction *customerStatus = parseCustomerStatus(iss);
         addAction(customerStatus);}
    else if(key=="volunteerStatus"){
        BaseAction *volunteerStatus = parseVolunteerStatus(iss);
         addAction(volunteerStatus);} 
    else if(key=="log"){
        BaseAction *log = parseLog(iss);
         addAction(log);}
    else if(key=="close"){
        BaseAction *close = parseClose(iss);
         addAction(close);} 
    else if(key=="backup"){
        BaseAction *backup = parseBackup(iss);
         addAction(backup);}   
    else if(key=="restore"){
        BaseAction *restore = parseRestore(iss);
         addAction(restore);}  
    else {
        std::cerr << "Error: Unknown keyword in configuration file: " << key << std::endl;
        exit(EXIT_FAILURE);}                       

}

// A helper method for the constructor
// Function to parse customer information and create a new AddCustomer action
BaseAction* WareHouse::parseCustomer(std::istringstream& iss) {
    int locationDistance, maxOrders;
    std::string name, customerType;
    iss >> name >> customerType >> locationDistance >> maxOrders;

    // Check if the customer type is valid
    if (customerType == "soldier" || customerType == "civilian") {
        BaseAction* newCustomerAction = new AddCustomer(name, customerType, locationDistance, maxOrders);
        newCustomerAction->act(*this);
        customerCounter++;  // Increment the counter after adding the customer

        return newCustomerAction; 

    } else {
        std::cerr << "Error: Unknown customer type: " << customerType << std::endl;
        exit(EXIT_FAILURE);
    }
}

// A helper method for the constructor
// Function to parse volunteer information and create a new Volunteer action
Volunteer* WareHouse::parseVolunteer(std::istringstream& iss) {
    string name, volunteerRole;
    iss >> name >> volunteerRole;

    // Common attributes for all volunteer types
    int coolDown = 0, maxDistance = 0, distancePerStep = 0, maxOrders = 0;

    // Additional attributes specific to certain volunteer types
    int numberOfWord=CountWords(iss.str());
    if (volunteerRole == "collector") {
        iss >> coolDown;
        if(numberOfWord==5) {
            iss >> maxOrders;
        }
    } else if (volunteerRole == "driver") {
        iss >> maxDistance;
        iss >> distancePerStep;
        if(numberOfWord==5) {
            iss >> maxOrders;
        }
    }


    // Create the appropriate volunteer object based on volunteerRole
    Volunteer* newVolunteer = nullptr;
    if(volunteerRole == "collector") {
        if(numberOfWord == 5){
            newVolunteer = new LimitedCollectorVolunteer(volunteerCounter++, name, coolDown, maxOrders);}
        else{
        newVolunteer = new CollectorVolunteer(volunteerCounter++, name, coolDown);}
    } 
    else if (volunteerRole == "driver") {
        if(numberOfWord == 5){
            newVolunteer = new LimitedDriverVolunteer(volunteerCounter++, name, maxDistance, distancePerStep, maxOrders);}
        else{    
        newVolunteer = new DriverVolunteer(volunteerCounter++, name, maxDistance, distancePerStep);}
    return newVolunteer;
}}

// A helper method for the constructor
BaseAction* WareHouse::parseOrder(std::istringstream& iss) {
    int orderId;
    iss >> orderId;

    // Create a new Order action based on parsed information
    BaseAction* newOrderAction = new AddOrder(orderId);
    newOrderAction->act(*this);  // Perform the action immediately
    ordercounter++;
    return newOrderAction;
}

// A helper method for the constructor
BaseAction* WareHouse::parseOrderStatus(std::istringstream& iss) {
    int orderId;
    iss >> orderId;

    // Create a new PrintOrderStatus action based on parsed information
    BaseAction* newOrderStatusAction = new PrintOrderStatus(orderId);
    newOrderStatusAction->act(*this);  // Perform the action immediately

    return newOrderStatusAction;
}

// A helper method for the constructor
BaseAction* WareHouse::parseCustomerStatus(std::istringstream& iss) {
    int customerId;
    iss >> customerId;

    // Create a new PrintCustomerStatus action based on parsed information
    BaseAction* newCustomerStatusAction = new PrintCustomerStatus(customerId);
    newCustomerStatusAction->act(*this);  // Perform the action immediately

    return newCustomerStatusAction;
}

// A helper method for the constructor
BaseAction* WareHouse::parseVolunteerStatus(std::istringstream& iss) {
    int volunteerId;
    iss >> volunteerId;

    // Create a new PrintVolunteerStatus action based on parsed information
    BaseAction* newVolunteerStatusAction = new PrintVolunteerStatus(volunteerId);
    newVolunteerStatusAction->act(*this);  // Perform the action immediately

    return newVolunteerStatusAction;
}

// A helper method for the constructor
BaseAction* WareHouse::parseLog(std::istringstream& iss) {
    // Create a new PrintActionsLog action
    BaseAction* newLogAction = new PrintActionsLog();
    newLogAction->act(*this);
    return newLogAction;
}

// A helper method for the constructor
BaseAction* WareHouse::parseClose(std::istringstream& iss) {
    // Create a new Close action
    BaseAction* newCloseAction = new Close();
    newCloseAction->act(*this);
    return newCloseAction;
}

// A helper method for the constructor
BaseAction* WareHouse::parseBackup(std::istringstream& iss) {
    // Create a new BackupWareHouse action
    BaseAction* newBackupAction = new BackupWareHouse();
    newBackupAction->act(*this);
    return newBackupAction;
}

// A helper method for the constructor
BaseAction* WareHouse::parseRestore(std::istringstream& iss) {
    // Create a new RestoreWareHouse action
    BaseAction* newRestoreAction = new RestoreWareHouse();
    newRestoreAction->act(*this);
    return newRestoreAction;
}

// A helper method for the constructor
int CountWords(const std::string& input) { //counts the number of words in input
    std::istringstream iss(input);
    std::string word;
    int wordCount = 0;

    while (iss >> word) {
        wordCount++;
    }

    return wordCount;
}

// end parserLine

const vector<BaseAction*> & WareHouse :: getActionsLog() const{
    return actionsLog; /// its is good to return
}

void WareHouse :: addOrder(Order* order){
    OrderStatus status = order->getStatus();
    if(status == OrderStatus :: PENDING || (status == OrderStatus :: COLLECTING && order->getDriverId() == NO_VOLUNTEER)){
       pendingOrders.push_back(order); 
    }
    else if(status == OrderStatus :: DELIVERING){
        inProcessOrders.push_back(order);
    }
    else{
        completedOrders.push_back(order);
    }


    
}

void WareHouse :: addAction(BaseAction* action){
    actionsLog.push_back(action);
}

void WareHouse :: printActionsLogs(){
    for(int i=0; i < actionsLog.size(); i++){// wiht =?)
        cout <<actionsLog[i]->toString() << endl;
        }
}

Customer & WareHouse :: getCustomer(int customerId) const{ // can i assume that the id is exist?
    for(int i=0; i < customers.size(); i++){
        if(customers[i]->getId() == customerId){
            return (*customers[i]);
        }
        
    }
    return *noSuchCustomer;

}

Volunteer & WareHouse :: getVolunteer(int volunteerId) const{
    for(int i=0; i < volunteers.size(); i++){
        if(volunteers[i]->getId() == volunteerId){
            return (*volunteers[i]);
        }
        
    }
    return *noSuchVolunteer;    
}

Order & WareHouse :: getOrder(int orderId) const{
    for (int i = 0; i < pendingOrders.size(); i++) {
        if (pendingOrders[i]->getId() == orderId) {
            return *(pendingOrders[i]);
        }
    }

    for (int i = 0; i < inProcessOrders.size(); i++) {
        if (inProcessOrders[i]->getId() == orderId) {
            return *(vol[i]);
        }
    }

    for (int i = 0; i < completedOrders.size(); i++) {
        if (completedOrders[i]->getId() == orderId) {
            return *(completedOrders[i]);
        }
    }
    return *noSuchOrder;

}

void WareHouse :: close(){
    isOpen=false;
}

void WareHouse :: open(){
    isOpen=true;
}

// Added methods:

const vector<Order*>& WareHouse :: getPendingOrders() const{
    return pendingOrders;
}

const vector<Volunteer*>& WareHouse :: getVolunteers() const{
    return volunteers;
}
const vector<Order*>& WareHouse :: getInProcessOrders() const{
    return inProcessOrders;
}

const vector<Order*>& WareHouse :: getcompletedOrder() const{
    return completedOrders;
}

const vector<Customer*>& WareHouse :: getCustomerVector() const{
    return customers;
}

int WareHouse :: getOrderCounter() const{
    return ordercounter;
}//////what should do here???????????
int WareHouse :: getCustomerCounter() const{
    return customerCounter;
}


void WareHouse :: addCustomer(Customer* customer){
    customers.push_back(customer);
}
void WareHouse :: addVolunteer(Volunteer* volunteer){
    volunteers.push_back(volunteer);
}


Order & WareHouse :: getOrderInPending(int orderId) const{
        for (int i = 0; i < pendingOrders.size(); i++) {
           if (pendingOrders[i]->getId() == orderId) {
             return *(pendingOrders[i]);
        }
        }
         return *noSuchOrder;
}
Order & WareHouse :: getOrderInProccess(int orderId) const{
        for (int i = 0; i < inProcessOrders.size(); i++) {
           if (inProcessOrders[i]->getId() == orderId) {
               return *(inProcessOrders[i]);
        }
        }
        return *noSuchOrder;    
}
Order & WareHouse :: getOrderInCompleted(int orderId) const{
        for (int i = 0; i < completedOrders.size(); i++) {
            if (completedOrders[i]->getId() == orderId) {
               return *(completedOrders[i]);
        }
        }
        return *noSuchOrder;
}

































