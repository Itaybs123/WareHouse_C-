#include "WareHouse.h"
#include "BaseAction.h"
#include <iostream>
#include <sstream>

BaseAction :: BaseAction() : errorMsg("") , status(){}
ActionStatus BaseAction :: getStatus() const{
    return status;
}
void BaseAction :: complete(){
    status = ActionStatus :: COMPLETED;
}
void BaseAction :: error(string errorMsg){
    status = ActionStatus :: ERROR;
    this->errorMsg=errorMsg;
}
string BaseAction :: getErrorMsg() const{
    return errorMsg;
}
string BaseAction :: getStatusInprint() const{
    if(status == ActionStatus :: COMPLETED){
        return "COMPLETED";  
    }
    else{
        return "ERROR";
    }

}




SimulateStep :: SimulateStep(int numOfSteps) : numOfSteps(numOfSteps){}
void SimulateStep ::  act(WareHouse &wareHouse){
    vector<Order*> pendingOrders = wareHouse.getPendingOrders();
   vector<Volunteer*> volunteers = wareHouse.getVolunteers();
   vector<Order*> InProcessOrders = wareHouse.getInProcessOrders();
   vector<Order*> completedOrder = wareHouse.getcompletedOrder();
   
   for(int i = 0; i < numOfSteps; i++)
   {
      // PENDING (start)
      int startFrom = 0;
      for(int i = 0; i < pendingOrders.size(); i++)
      {
         Order* currentOrder = pendingOrders[i]; 
         bool foundVolunteer = false;

         if(currentOrder->getStatus() == OrderStatus :: PENDING) // found a new pending order, searching volunteer
         {
            for(int j = startFrom; !foundVolunteer && j < volunteers.size(); j++)
            // now searching for an avaliable collector volunteer
            {
               Volunteer* currentVolunteer = volunteers[j];
               int collcetorOrDriver = currentVolunteer->WhoAmI();
               bool isCollector = collcetorOrDriver == 1 || collcetorOrDriver == 2;
               if(isCollector && currentVolunteer->canTakeOrder(*currentOrder)) 
               // found a collector volunteer and he is avaliable to take order
               // make the match between the order and the volunteer
               {
                  startFrom = j;
                  foundVolunteer = true;
                  currentVolunteer->acceptOrder(*currentOrder);
                  currentOrder->setStatus(OrderStatus :: COLLECTING);
                  currentOrder->setCollectorId(currentVolunteer->getId());
                  InProcessOrders.push_back(currentOrder);
                  pendingOrders.erase(pendingOrders.begin() + i); //check
               }
            }
         }
         else // not a new order, wait for collecting, searching for a driver volunteer
         {
            for(int j = 0; !foundVolunteer && j < volunteers.size(); j++)
            {
               // now searching for an avaliable driver volunteer
               Volunteer* currentVolunteer = volunteers[j];
               int collcetorOrDriver = currentVolunteer->WhoAmI();
               bool isDriver = collcetorOrDriver == 3 || collcetorOrDriver == 4;
               if(isDriver && currentVolunteer->canTakeOrder(*currentOrder))
               // found a driver volunteer and he is avaliable to take order
               // make the match between the order and the volunteer
               {
                  foundVolunteer = true;
                  currentVolunteer->acceptOrder(*currentOrder);
                  currentOrder->setStatus(OrderStatus :: DELIVERING);
                  currentOrder->setDriverId(currentVolunteer->getId());
                  InProcessOrders.push_back(currentOrder);
                  pendingOrders.erase(pendingOrders.begin() + i); //check
               }
            }
         }
      }
      // PENDING (end)

      // We will go through all the volunteers and make 1 step forward
      for(int i=0 ; i < volunteers.size() ; i++)
      {
         Volunteer* currentVolunteer = volunteers[i];
         currentVolunteer->step();
         // Checking if the volunteer has now finished a task
         if(currentVolunteer->getCompletedOrderId() != NO_ORDER)
         {
            // 
            //Order* pointerToCompletedOrderId = currentVolunteer->getOrderPointer();// אפשרות אם אסור לעשות קאסט פשוט לחפש איטרטיבי
            // The volunteer finished a task
            Order& currOrder  = wareHouse.getOrder(currentVolunteer->getCompletedOrderId());
            currentVolunteer-> setCompletedOrderId(NO_ORDER);
            int collcetorOrDriver= currentVolunteer->WhoAmI();
            if(collcetorOrDriver == 1 || collcetorOrDriver==2){
                pendingOrders.push_back(*currOrder);
                if(collcetorOrDriver==2 && (currentVolunteer->hasOrdersLeft()) == false){
                  volunteers.erase(volunteers.begin() + i); ///לבדוק אם זה מוחק טוב

                }
            }
            if(collcetorOrDriver == 3 || collcetorOrDriver==4){
               completedOrder.push_back(pointerToCompletedOrderId);
               pointerToCompletedOrderId->setStatus(OrderStatus :: COMPLETED);
               if(collcetorOrDriver==4 && (currentVolunteer->hasOrdersLeft()) == false){
                  volunteers.erase(volunteers.begin() + i);
                  
               }
            }


         }
      }
    }
}
std::string SimulateStep ::  toString() const {
        return "SimulateStep " + std::to_string(numOfSteps) + " "+ getStatusInprint();

    
}
SimulateStep * SimulateStep :: clone() const {
    return new SimulateStep(this->numOfSteps);
}