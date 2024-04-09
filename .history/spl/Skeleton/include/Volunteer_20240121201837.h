#pragma once
#include <string>
#include <vector>
#include "Order.h"
using std::string;
using std::vector;

#define NO_ORDER -1

class Volunteer {
    public:
        Volunteer(int id, const string &name);
        int getId() const;
        const string &getName() const;
        int getActiveOrderId() const;
        void setActiveOrderId(int orderId);//חדש
        int getCompletedOrderId() const;
        void setCompletedOrderId(int orderId);//חדש
        bool isBusy() const; // Signal whether the volunteer is currently processing an order    
        virtual bool hasOrdersLeft() const = 0; // Signal whether the volunteer didn't reach orders limit,Always true for CollectorVolunteer and DriverVolunteer
        virtual bool canTakeOrder(const Order &order) const = 0; // Signal if the volunteer can take the order.      
        virtual void acceptOrder(const Order &order) = 0; // Prepare for new order(Reset activeOrderId,TimeLeft,DistanceLeft,OrdersLeft depends on the volunteer type)
                
        virtual void step() = 0; //Simulate volunteer step,if the volunteer finished the order, transfer activeOrderId to completedOrderId

        virtual string toString() const = 0;
        virtual Volunteer* clone() const = 0; //Return a copy of the volunteer

    protected:
        int completedOrderId; //Initialized to NO_ORDER if no order has been completed yet
        int activeOrderId; //Initialized to NO_ORDER if no order is being processed
    
    private:
        const int id;
        const string name;
       //int orderIDbyVolunteer;    צריך?????

};
/// להשלים 

class CollectorVolunteer: public Volunteer {

    public:
        CollectorVolunteer(int id, string name, int coolDown);
        CollectorVolunteer(const CollectorVolunteer &other);
        virtual CollectorVolunteer *clone() const override;
        virtual void step() override;
        int getCoolDown() const;
        int getTimeLeft() const;
        bool decreaseCoolDown();//Decrease timeLeft by 1,return true if timeLeft=0,false otherwise
        virtual bool hasOrdersLeft() const override;
        virtual bool canTakeOrder(const Order &order) const override;
        virtual void acceptOrder(const Order &order) override;
        virtual string toString() const override;
        void setTimeLeft();
    
    private:
        const int coolDown; // The time it takes the volunteer to process an order
        int timeLeft; // Time left until the volunteer finishes his current order
};

class LimitedCollectorVolunteer: public CollectorVolunteer {
////// לבדוק האם צריך virtual כי זה דריסה?
    public:
        LimitedCollectorVolunteer(int id, string name, int coolDown ,int maxOrders);
        LimitedCollectorVolunteer(const LimitedCollectorVolunteer &other);
        virtual LimitedCollectorVolunteer *clone() const override;
        virtual bool hasOrdersLeft() const override;
        virtual bool canTakeOrder(const Order &order) const override;
        virtual void acceptOrder(const Order &order) override;

        int getMaxOrders() const;
        int getNumOrdersLeft() const;
        virtual string toString() const override;
    
    private:
        const int maxOrders; // The number of orders the volunteer can process in the whole simulation
        int ordersLeft; // The number of orders the volunteer can still take
};

class DriverVolunteer: public Volunteer {

    public:
        DriverVolunteer(int id, string name, int maxDistance, int distancePerStep);
        DriverVolunteer(const DriverVolunteer &other);
        virtual DriverVolunteer *clone() const override;
        int getDistanceLeft() const;
        int getMaxDistance() const;
        int getDistancePerStep() const;  
        bool decreaseDistanceLeft(); //Decrease distanceLeft by distancePerStep,return true if distanceLeft<=0,false otherwise
        virtual bool hasOrdersLeft() const override;
        virtual bool canTakeOrder(const Order &order) const override; // Signal if the volunteer is not busy and the order is within the maxDistance
        virtual void acceptOrder(const Order &order) override; // Reset distanceLeft to maxDistance
        virtual void step() override; // Decrease distanceLeft by distancePerStep
        virtual string toString() const override;
        void SetDistanceLeft(const Order &order);

    private:
        const int maxDistance; // The maximum distance of ANY order the volunteer can take
        const int distancePerStep; // The distance the volunteer does in one step
        int distanceLeft; // Distance left until the volunteer finishes his current order
};

class LimitedDriverVolunteer: public DriverVolunteer {

    public:
        LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep,int maxOrders);
        LimitedDriverVolunteer(const LimitedDriverVolunteer &other);
        LimitedDriverVolunteer *clone() const override;
        int getMaxOrders() const;
        int getNumOrdersLeft() const;
        virtual bool hasOrdersLeft() const override;
        virtual bool canTakeOrder(const Order &order) const override; // Signal if the volunteer is not busy, the order is within the maxDistance and have orders left
        virtual void acceptOrder(const Order &order) override; // Reset distanceLeft to maxDistance and decrease ordersLeft
        virtual string toString() const override;

    private:
        const int maxOrders; // The number of orders the volunteer can process in the whole simulation
        int ordersLeft; // The number of orders the volunteer can still take
};