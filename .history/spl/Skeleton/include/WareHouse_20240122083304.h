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
        
        // Added methods:
        vector<Order*>& getPendingOrders() ;
        vector<Volunteer*>& getVolunteers() ;
        vector<Order*>& getVol() ;
        vector<Order*>& getcompletedOrder() ;
        vector<Customer*>& getCustomerVector() ;
        int getOrderId() const;
        int getCustomerId() const;
        void addCustomer(Customer* customer);
        void addVolunteer(Volunteer* volunteer);
        Order &getOrderInPending(int orderId) const;
        Order &getOrderInProccess(int orderId) const;
        Order &getOrderInCompleted(int orderId) const;

        // configFilePath
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
        vector<Order*> vol; // In Process orders
        vector<Order*> completedOrders;
        vector<Customer*> customers;
        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter;
        int ordercounter; //For assigning unique volunteer IDs

        // add file
        SoldierCustomer* noSuchCustomer;
        CollectorlimiterVolunteer* noSuchVolunteer;
        Order* noSuchOrder;

};