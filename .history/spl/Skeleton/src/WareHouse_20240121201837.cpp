#include "WareHouse.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
#include "BaseAction.h"
WareHouse::WareHouse(const string &configFilePath): actionsLog(), volunteers(), pendingOrders(), vol(), completedOrders(), customers(), volunteerCounter(0), customerCounter(0),ordercounter(0), isOpen(true) {
    // Member initializer list: Initializing various member variables and vectors to default values
    // actionsLog, volunteers, pendingOrders, vol, completedOrders, and customers vectors are initialized as empty vectors
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
        iss >> keyword;
        parserLine(iss, keyword , true);
    }

    // Close the configuration file
    configFile.close();
}

void WareHouse :: parserLine(std::istringstream& iss ,std::string key , bool itsPhars){
    if (key == "volunteer"){
        Volunteer *newVolunteer = parseVolunteer(iss);
        volunteers.push_back(newVolunteer);}
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

BaseAction* WareHouse::parseOrder(std::istringstream& iss) {
    int orderId;
    iss >> orderId;

    // Create a new Order action based on parsed information
    BaseAction* newOrderAction = new Order(orderId);
    newOrderAction->act(*this);  // Perform the action immediately
    ordercounter++;
    return newOrderAction;
}
BaseAction* WareHouse::parseOrderStatus(std::istringstream& iss) {
    int orderId;
    iss >> orderId;

    // Create a new PrintOrderStatus action based on parsed information
    BaseAction* newOrderStatusAction = new PrintOrderStatus(orderId);
    newOrderStatusAction->act(*this);  // Perform the action immediately

    return newOrderStatusAction;
}
BaseAction* WareHouse::parseCustomerStatus(std::istringstream& iss) {
    int customerId;
    iss >> customerId;

    // Create a new PrintCustomerStatus action based on parsed information
    BaseAction* newCustomerStatusAction = new PrintCustomerStatus(customerId);
    newCustomerStatusAction->act(*this);  // Perform the action immediately

    return newCustomerStatusAction;
}

BaseAction* WareHouse::parseVolunteerStatus(std::istringstream& iss) {
    int volunteerId;
    iss >> volunteerId;

    // Create a new PrintVolunteerStatus action based on parsed information
    BaseAction* newVolunteerStatusAction = new PrintVolunteerStatus(volunteerId);
    newVolunteerStatusAction->act(*this);  // Perform the action immediately

    return newVolunteerStatusAction;
}
BaseAction* WareHouse::parseLog(std::istringstream& iss) {
    // Create a new PrintActionsLog action
    BaseAction* newLogAction = new PrintActionsLog();
    newLogAction->act(*this);
    return newLogAction;
}

BaseAction* WareHouse::parseClose(std::istringstream& iss) {
    // Create a new Close action
    BaseAction* newCloseAction = new Close();
    newCloseAction->act(*this);
    return newCloseAction;
}

BaseAction* WareHouse::parseBackup(std::istringstream& iss) {
    // Create a new BackupWareHouse action
    BaseAction* newBackupAction = new BackupWareHouse();
    newBackupAction->act(*this);
    return newBackupAction;
}

BaseAction* WareHouse::parseRestore(std::istringstream& iss) {
    // Create a new RestoreWareHouse action
    BaseAction* newRestoreAction = new RestoreWareHouse();
    newRestoreAction->act(*this);
    return newRestoreAction;
}


int CountWords(const std::string& input) { //counts the number of words in input
    std::istringstream iss(input);
    std::string word;
    int wordCount = 0;

    while (iss >> word) {
        wordCount++;
    }

    return wordCount;
}

const vector<BaseAction*> & WareHouse :: getActionsLog() const{}
void WareHouse :: addOrder(Order* order){}
void WareHouse :: addAction(BaseAction* action){}
void WareHouse :: printActionsLogs(){}
Customer & WareHouse :: getCustomer(int customerId) const{}
Volunteer & WareHouse :: getVolunteer(int volunteerId) const{}
Order & WareHouse :: getOrder(int orderId) const{}
void WareHouse :: close(){}
void WareHouse :: open(){}
































