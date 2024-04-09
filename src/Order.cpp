#include "Order.h"

// Order constructor
Order :: Order(int id, int customerId, int distance) : id(id), customerId(customerId) , distance(distance), status(OrderStatus::PENDING), collectorId(NO_VOLUNTEER), driverId(NO_VOLUNTEER), afterCollecting(false){}

// Order Methods
int Order :: getId() const{
    return id;
}

int Order :: getCustomerId() const{
    return customerId;
}

void Order :: setStatus(OrderStatus status){
    this->status = status;
}

void Order :: setCollectorId(int collectorId){
    this->collectorId = collectorId;
}

void Order :: setDriverId(int driverId){
    this->driverId = driverId;
}

int Order :: getCollectorId() const{
    return collectorId;
}

int Order ::  getDriverId() const{
    return driverId;
}

OrderStatus Order :: getStatus() const{
    return status;
}

const string Order::toString() const {
    return "OrderId: " + std::to_string(id) + "\n" +
           "OrderStatus: " + orderStatusInString(status) + "\n" +
           "CustomerId: " + std::to_string(customerId) + "\n" +
           "Collector: " + ((collectorId != NO_VOLUNTEER) ? std::to_string(collectorId) : "None") + "\n" +
           "Driver: " + ((driverId != NO_VOLUNTEER) ? std::to_string(driverId) : "None");
}

const int Order ::  getDistance() const{
    return distance;
} 

string Order :: orderStatusInString(OrderStatus statusOrder) const{
    std::string converted = "";
    switch (statusOrder) 
    {
        case OrderStatus::PENDING:
            converted = "PENDING";
            break;
        case OrderStatus::COLLECTING:
            converted = "COLLECTING";
            break;
        case OrderStatus::DELIVERING:
            converted = "DELIVERING";
            break;
        case OrderStatus::COMPLETED:
            converted = "COMPLETED";
            break;
        default:
            converted = "UNKNOWN";
            break;
    }
    return converted;
}

bool Order :: getAfterCollecting() const {
    return afterCollecting;
}

void Order :: setAfterCollecting(){
    afterCollecting = true;
}
