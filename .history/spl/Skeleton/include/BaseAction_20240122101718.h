#pragma once
#include <string>
#include <vector>
#include "WareHouse.h"
using std::string;
using std::vector;

enum class ActionStatus{
    COMPLETED, ERROR
};

enum class CustomerType{
    Soldier, Civilian
};


class Customer;

class BaseAction{
    public:
        BaseAction();
        ActionStatus getStatus() const;
        virtual void act(WareHouse& wareHouse)=0;
        virtual string toString() const=0;
        virtual BaseAction* clone() const=0;

    protected:
        void complete();
        void error(string errorMsg);
        string getErrorMsg() const;

    private:
        string errorMsg;
        ActionStatus status;
};

class SimulateStep : public BaseAction {

    public:
        SimulateStep(int numOfSteps);
        void act(WareHouse &wareHouse) override;
        virtual std::string toString() const override;
        virtual SimulateStep *clone() const override;

    private:
        const int numOfSteps;
};

class AddOrder : public BaseAction {
    public:
        AddOrder(int id);
        void act(WareHouse &wareHouse) override;
        virtual string toString() const override;
        virtual AddOrder *clone() const override;

    private:
        const int customerId;
};


class AddCustomer : public BaseAction {
    public:
        AddCustomer(string customerName, string customerType, int distance, int maxOrders);
        void act(WareHouse &wareHouse) override;
        virtual string toString() const override;        
        virtual AddCustomer *clone() const override;

    private:
        const string customerName;
        const CustomerType customerType;
        const int distance;
        const int maxOrders;
};



class PrintOrderStatus : public BaseAction {
    public:
        PrintOrderStatus(int id);
        void act(WareHouse &wareHouse) override;
        virtual string toString() const override;        
        virtual PrintOrderStatus *clone() const override;

    private:
        const int orderId;
};

class PrintCustomerStatus: public BaseAction {
    public:
        PrintCustomerStatus(int customerId);
        void act(WareHouse &wareHouse) override;
        virtual string toString() const override;        
        virtual PrintCustomerStatus *clone() const override;

    private:
        const int customerId;
};


class PrintVolunteerStatus : public BaseAction {
    public:
        PrintVolunteerStatus(int id);
        void act(WareHouse &wareHouse) override;
        virtual string toString() const override;
        virtual PrintVolunteerStatus *clone() const override;
        
    private:
        const int VolunteerId;
};


class PrintActionsLog : public BaseAction {
    public:
        PrintActionsLog();
        void act(WareHouse &wareHouse) override;
        virtual string toString() const override;
        virtual PrintActionsLog *clone() const override;
        
    private:
};

class Close : public BaseAction {
    public:
        Close();
        void act(WareHouse &wareHouse) override;
        virtual string toString() const override;
        virtual Close *clone() const override;
        
    private:
};

class BackupWareHouse : public BaseAction {
    public:
        BackupWareHouse();
        void act(WareHouse &wareHouse) override;
        virtual string toString() const override;        
        virtual BackupWareHouse *clone() const override;

    private:
};


class RestoreWareHouse : public BaseAction {
    public:
        RestoreWareHouse();
        void act(WareHouse &wareHouse) override;
        virtual string toString() const override;
        virtual RestoreWareHouse *clone() const override;

    private:
};