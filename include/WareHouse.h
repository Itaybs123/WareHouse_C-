#pragma once
#include <string>
#include <vector>
#include "Order.h"
#include "Customer.h"
#include "Volunteer.h"
using namespace std;

class BasedAction;
class Volunteer;

// Warehouse responsible for Volunteers, Customers and Actions.

class WareHouse {

 public:
        WareHouse(const string &configFilePath);
        void start();
        const vector<BasedAction*> &getActions() const;
        void addOrder(Order* order);
        void addAction(BasedAction* action);
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        void close();
        void open();
        
        // Added methods:
        vector<Order*>& getPendingOrders();
        vector<Order*>& getInProcessOrders();
        vector<Order*>& getcompletedOrder();
        vector<Volunteer*>& getVolunteers();
        vector<Customer*>& getCustomerVector();

        int getOrderCounter() const;
        int getCustomerCounter() const;

        void addCustomer(Customer* customer);
        void addVolunteer(Volunteer* volunteer);

        Order &getOrderInPending(int orderId) const;
        Order &getOrderInProccess(int orderId) const;
        Order &getOrderInCompleted(int orderId) const;

        void deleteFromInProccess(Order* orderToDelete);
        void  plusOrderCounter();


        // configFilePath

        // void add_Costumer_vector(Customer* Cost); // ????????????????????????????????????????????
        // void parseConfigFile(const std::string& configFilePath); // ????????????????????????????????????????????

        void parserLine(std::istringstream& iss , std::string key , bool itsPhars);
        BasedAction* parseCustomer(std::istringstream& iss); // V
        BasedAction* parseStep(std::istringstream& iss); // V
        BasedAction* parseOrder(std::istringstream& iss); // V
        BasedAction* parseOrderStatus(std::istringstream& iss); // V
        BasedAction* parseCustomerStatus(std::istringstream& iss); // V
        BasedAction* parseVolunteerStatus(std::istringstream& iss); // V
        BasedAction* parseLog(std::istringstream& iss); // V
        BasedAction* parseClose(std::istringstream& iss); // V
        BasedAction* parseBackup(std::istringstream& iss); // V
        BasedAction* parseRestore(std::istringstream& iss);                                          
        Volunteer* parseVolunteer(std::istringstream& iss);

        // Rule of Five:

        // Destructor
        virtual ~WareHouse();

        // Copy constructor
        WareHouse(const WareHouse &other);

        // Copy assignment operator
        WareHouse& operator=(const WareHouse &other);

        // Move Constructor
        WareHouse( WareHouse &&other);

        // Move Assignment Operator
        WareHouse& operator=(WareHouse &&other);

        // void cleanup();

    private:
        bool isOpen;
        vector<BasedAction*> actionsLog;
        vector<Order*> pendingOrders;
        vector<Order*> inProcessOrders;
        vector<Order*> completedOrders;
        vector<Customer*> customers;
        vector<Volunteer*> volunteers;

        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter;
        int ordercounter; //For assigning unique volunteer IDs

        // add file
        SoldierCustomer* noSuchCustomer;
        LimitedCollectorVolunteer* noSuchVolunteer;
        Order* noSuchOrder;
};