#pragma once
#include <string>
#include <vector>
#include "WareHouse.h"
using std::string;
using std::vector;

enum class ActionStatus
{
    COMPLETED, ERROR
};

enum class CustomerType
{
    Soldier, Civilian
};

class Customer;

class BasedAction {

    public:
        BasedAction();
        ActionStatus getStatus() const;
        virtual void act(WareHouse& wareHouse)=0;
        virtual string toString() const=0;
        virtual BasedAction* clone() const=0;

        virtual ~BasedAction() = default;

    protected:
        void complete();
        void error(string errorMsg);
        string getErrorMsg() const;

        // Added methods
        const string getStatusInprint() const;

    private:
        string errorMsg;
        ActionStatus status;
};

// ---------------------------------------------------------------------------------------------

class SimulateStep : public BasedAction {

    public:
        SimulateStep(int numOfSteps);
        void act(WareHouse &wareHouse) override;
        virtual std::string toString() const override;
        virtual SimulateStep *clone() const override;

        virtual ~SimulateStep() override = default;

    private:
        const int numOfSteps;
};

// ---------------------------------------------------------------------------------------------

class AddOrder : public BasedAction {
    public:
        AddOrder(int id);
        void act(WareHouse &wareHouse) override;
        virtual string toString() const override;
        virtual AddOrder *clone() const override;

        virtual ~AddOrder() override = default;


    private:
        const int customerId;
};

// ---------------------------------------------------------------------------------------------

class AddCustomer : public BasedAction {
    public:
        AddCustomer(const string &customerName, const string &customerType, int distance, int maxOrders);
        void act(WareHouse &wareHouse) override;
        virtual string toString() const override;        
        virtual AddCustomer *clone() const override;

        // Added methods
        CustomerType converCustomerType(string customerType);
        string customerTypeToString(CustomerType type) const;

        virtual ~AddCustomer() override = default;



   
    private:
        const string customerName;
        const CustomerType customerType;
        const int distance;
        const int maxOrders;

        const string customerTypeInString;
};

// ---------------------------------------------------------------------------------------------

class PrintOrderStatus : public BasedAction {
    public:
        PrintOrderStatus(int id);
        void act(WareHouse &wareHouse) override;
        virtual string toString() const override;        
        virtual PrintOrderStatus *clone() const override;


        virtual ~PrintOrderStatus() override = default;


    private:
        const int orderId;
};

// ---------------------------------------------------------------------------------------------

class PrintCustomerStatus: public BasedAction {
    public:
        PrintCustomerStatus(int customerId);
        void act(WareHouse &wareHouse) override;
        virtual string toString() const override;        
        virtual PrintCustomerStatus *clone() const override;


        virtual ~PrintCustomerStatus() override = default;


    private:
        const int customerId;
};

// ---------------------------------------------------------------------------------------------

class PrintVolunteerStatus : public BasedAction {
    public:
        PrintVolunteerStatus(int id);
        void act(WareHouse &wareHouse) override;
        virtual string toString() const override;
        virtual PrintVolunteerStatus *clone() const override;



        virtual ~PrintVolunteerStatus() override = default;
        
    private:
        const int volunteerId;
};

// ---------------------------------------------------------------------------------------------

class PrintActionsLog : public BasedAction {
    public:
        PrintActionsLog();
        void act(WareHouse &wareHouse) override;
        virtual string toString() const override;
        virtual PrintActionsLog *clone() const override;

        virtual ~PrintActionsLog() override = default;
        
    private:
};

// ---------------------------------------------------------------------------------------------

class Close : public BasedAction {
    public:
        Close();
        void act(WareHouse &wareHouse) override;
        virtual string toString() const override;
        virtual Close *clone() const override;

        string OrderStatusInString(const OrderStatus status);

        virtual ~Close() override = default;
        
    private:
};

// ---------------------------------------------------------------------------------------------

extern WareHouse* backup;

class BackupWareHouse : public BasedAction {
    public:
        BackupWareHouse();
        void act(WareHouse &wareHouse) override;
        virtual string toString() const override;        
        virtual BackupWareHouse *clone() const override;

        virtual ~BackupWareHouse() override = default;

    private:
};

// ---------------------------------------------------------------------------------------------
class RestoreWareHouse : public BasedAction {
    public:
        RestoreWareHouse();
        void act(WareHouse &wareHouse) override;
        virtual string toString() const override;
        virtual RestoreWareHouse *clone() const override;

        virtual ~RestoreWareHouse() override = default;

    private:
};