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
                wareHouse.addOrder(&currOrder);
                //pendingOrders.push_back(*currOrder);
                if(collcetorOrDriver==2 && (currentVolunteer->hasOrdersLeft()) == false){
                  volunteers.erase(volunteers.begin() + i); ///לבדוק אם זה מוחק טוב

                }
            }
            if(collcetorOrDriver == 3 || collcetorOrDriver==4){
               //completedOrder.push_back(pointerToCompletedOrderId);
               currOrder.setStatus(OrderStatus :: COMPLETED);
               wareHouse.addOrder(&currOrder);
               if(collcetorOrDriver==4 && (currentVolunteer->hasOrdersLeft()) == false){
                  volunteers.erase(volunteers.begin() + i);
                  
               }
            }


         }
      }
    }
    complete();
}
std::string SimulateStep ::  toString() const {
        return "SimulateStep " + std::to_string(numOfSteps) + " "+ getStatusInprint();

    
}
SimulateStep * SimulateStep :: clone() const {
    return new SimulateStep(this->numOfSteps);
}




        AddOrder :: AddOrder(int id) :customerId(id) {}
        void AddOrder :: act(WareHouse &wareHouse) {
         Customer& currCustomer = wareHouse.getCustomer(customerId);
         if(currCustomer.getId() == -1 || (currCustomer.getMaxOrders() - currCustomer.getNumOrders() > 0)){
            error("Cannot place this order");
            std::cout << "Cannot place this order" << std::endl;
         }
         else{
         Order* currOrder = new Order(wareHouse.getOrderCounter() , customerId , currCustomer.getCustomerDistance());
         wareHouse.addOrder(currOrder);
         complete();
         }

        }
        string AddOrder ::toString() const {
          return "Order " + std::to_string(customerId) + " "+ getStatusInprint();

        }
        AddOrder * AddOrder ::clone() const {
         return new AddOrder(this->customerId);
        }





        AddCustomer :: AddCustomer(string customerName, string customerType, int distance, int maxOrders) : customerName(customerName) , 
       customerType(converCustomerType(customerType)) , distance(distance) , maxOrders(maxOrders) ,customerTypeInString(customerType){}
       

        void AddCustomer :: act(WareHouse &wareHouse) {
         Customer* curr = nullptr;
         if(customerType == CustomerType :: Soldier){
            curr = new SoldierCustomer(wareHouse.getCustomerCounter() , customerName ,distance ,maxOrders);

         }
         else{
            curr=new CivilianCustomer(wareHouse.getCustomerCounter() , customerName ,distance ,maxOrders);
         }
         wareHouse.addCustomer(curr);
         complete();
        }
      
        string AddCustomer :: toString() const {
         return "customer " + customerName + " " +customerTypeInString +" "+std::to_string(distance) + " "+ std::to_string(maxOrders) + " "+ "COMPLETED";
        }
        AddCustomer * AddCustomer ::clone() const {
         if(customerTypeInString == "Soldier"){
            return new SoldierCustomer()
         }
        }

        CustomerType AddCustomer :: converCustomerType(string customerType){
         if(customerType == "Soldier"){
            return CustomerType :: Soldier;

         }
         else{
            return CustomerType :: Civilian;
         }

        }



