#pragma once
#include <string>
#include <vector>
using namespace std;

#include "Order.h"
#include "Customer.h"
#include "Volunteer.h"

class BaseAction;
class Volunteer;

// Warehouse responsible for Volunteers, Customers and Actions.


class WareHouse {

    public:
        WareHouse(const string &configFilePath);
        void start();
        const vector<BaseAction*> &getActionsLog() const;
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        void printActionsLogs();
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        void close();
        void open();


        
        int CountWords(const std::string& input);
        void add_Costumer_vector(Customer* Cost);
        void parseConfigFile(const std::string& configFilePath);
        BaseAction* parseCustomer(std::istringstream& iss);
        BaseAction* parseStep(std::istringstream& iss);
        BaseAction* parseOrder(std::istringstream& iss);
        BaseAction* parseOrderStatus(std::istringstream& iss);
        BaseAction* parseCustomerStatus(std::istringstream& iss);
        BaseAction* parseVolunteerStatus(std::istringstream& iss);
        BaseAction* parseLog(std::istringstream& iss);
        BaseAction* parseClose(std::istringstream& iss);
        BaseAction* parseBackup(std::istringstream& iss);
        BaseAction* parseRestore(std::istringstream& iss);                                                
        Volunteer* parseVolunteer(std::istringstream& iss);
        void parserLine(std::istringstream& iss , std::string key , bool itsPhars);

    private:
        bool isOpen;
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> vol;
        vector<Order*> completedOrders;
        vector<Customer*> customers;
        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter;
        int ordercounter; //For assigning unique volunteer IDs
};