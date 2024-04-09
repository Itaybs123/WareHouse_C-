#include "WareHouse.h"
#include "Action.h"
#include <iostream>
extern WareHouse* backup;
// ---------------------------------------------------------------------------------------------

// BasedAction constructor
BasedAction :: BasedAction() : errorMsg("") , status(){}

// methods
ActionStatus BasedAction :: getStatus() const{
   return status;
}

void BasedAction :: complete(){
   status = ActionStatus :: COMPLETED;
}

void BasedAction :: error(string errorMsg){
   status = ActionStatus :: ERROR;
   this->errorMsg = errorMsg;
}

string BasedAction :: getErrorMsg() const{
   return errorMsg;
}

const string BasedAction :: getStatusInprint() const{
   if(status == ActionStatus :: COMPLETED)
   {
      return "COMPLETED";  
   }
   else
   {
      return "ERROR";
   }
}

// ---------------------------------------------------------------------------------------------

// SimulateStep constructor
SimulateStep :: SimulateStep(int numOfSteps) : BasedAction(), numOfSteps(numOfSteps){}


// methods
void SimulateStep :: act(WareHouse &wareHouse)
{
   vector<Order*>& pendingOrders = wareHouse.getPendingOrders();
   vector<Volunteer*>& volunteers = wareHouse.getVolunteers();

   for(int i = 0; i < numOfSteps; i++) // every step, do:
   {
      // First of all, we want to look for orders the PENDING vector and find an available volunteer for them.
      // Pending order will be passed to collector volunteer.
      // Collecting order will be passed to driver.
      
      // PENDING (start)
      int startFrom = 0; // In order not to go through collector volunteers that can not take an order again
      for(size_t i = 0; i < pendingOrders.size(); i++)
      {
         Order* currentOrder = pendingOrders[i];
         bool foundVolunteer = false;

         // The PENDING vector has two types of orders: PENDING and COLLECTING.
         // For an order with PENDING status, we will look for a collector volunteer
         // and for an order with COLLECTING status, we will look for a driver volunteer

         if(currentOrder->getStatus() == OrderStatus :: PENDING) // found an order with status: PENDING, searching for collector volunteer
         {
            for(size_t j = startFrom; !foundVolunteer && j < volunteers.size(); j++)
            {
               Volunteer* currentVolunteer = volunteers[j];
               int collcetorOrDriver = currentVolunteer->WhoAmI();
               bool isCollector = collcetorOrDriver == 1 || collcetorOrDriver == 2;
               if(isCollector && currentVolunteer->canTakeOrder(*currentOrder)) 
               // found a collector volunteer and he is avaliable to take order
               // make the match between the order and the collector volunteer
               {
                  startFrom = j;
                  foundVolunteer = true;
                  currentVolunteer->acceptOrder(*currentOrder);
                  currentOrder->setStatus(OrderStatus :: COLLECTING);
                  currentOrder->setCollectorId(currentVolunteer->getId());
                  wareHouse.addOrder(currentOrder); // The status is COLLECTING and should be pushed now to InProcess
                  pendingOrders.erase(pendingOrders.begin() + i);
                  i--;
               }
            }
         }

         else // It is not a new order, found an order with status: COLLECTING, searching for driver volunteer
         {
            for(size_t j = 0; !foundVolunteer && j < volunteers.size(); j++)
            {
               Volunteer* currentVolunteer = volunteers[j];
               int collcetorOrDriver = currentVolunteer->WhoAmI();
               bool isDriver = collcetorOrDriver == 3 || collcetorOrDriver == 4;
               if(isDriver && currentVolunteer->canTakeOrder(*currentOrder))
               // found a driver volunteer and he is avaliable to take order
               // make the match between the order and the driver volunteer
               {
                  foundVolunteer = true;
                  currentVolunteer->acceptOrder(*currentOrder);
                  currentOrder->setStatus(OrderStatus :: DELIVERING);
                  currentOrder->setDriverId(currentVolunteer->getId());
                  wareHouse.addOrder(currentOrder); // The status is DELIVERING and should be pushed now to InProcess
                  pendingOrders.erase(pendingOrders.begin() + i);
                  i--;
               }
            }
         }
      }
      // PENDING (end)

      // Now we will go through all the volunteers and make 1 step forward in the time scheme.

      for(size_t i = 0 ; i < volunteers.size() ; i++)
      {
         Volunteer* currentVolunteer = volunteers[i];
         currentVolunteer->step();

         // Checking if the volunteer has now finished a task
         if(currentVolunteer->getCompletedOrderId() != NO_ORDER)
         {
            // it means the volunteer just finished a task right now

            // search the order
            Order& currOrder = wareHouse.getOrderInProccess(currentVolunteer->getCompletedOrderId()); 
            
            // delete the order from the in proccess vector
            wareHouse.deleteFromInProccess(&currOrder);

            // The status of the order can be COLLECTING or DELIVERING
            // now the order should be pushed to COMPLETED vector or to the PENDING vector

            currentVolunteer-> setCompletedOrderId(NO_ORDER);
            
            int collcetorOrDriver = currentVolunteer->WhoAmI();

            if(!currentVolunteer->hasOrdersLeft())
            {
               // If the volunteer has completed all their orders (limited only), remove them from the volunteers list
               if (currentVolunteer != nullptr) // just to make sure
               {
                  delete currentVolunteer; // Free the memory
               }
               currentVolunteer = nullptr;
               volunteers.erase(volunteers.begin() + i);
               i--; // Decrement i to account for the shift in elements
            }
            
            if(collcetorOrDriver == 1 || collcetorOrDriver == 2) // the volunteer that finished the task is a collector
            {
               // The status of the order is COLLECTING
               currOrder.setAfterCollecting();
               // now the order should be pushed to the PENDING vector
            }
            else // the volunteer is a driver
            {
               // The status of the order is DELIVERING
               currOrder.setStatus(OrderStatus ::COMPLETED);
            }

            wareHouse.addOrder(&currOrder);
         }
      }
    }
    complete(); // This action never result an error
}

std::string SimulateStep ::  toString() const{
   return "SimulateStep " + std::to_string(numOfSteps) + " " + getStatusInprint();
}

SimulateStep * SimulateStep :: clone() const{
    return new SimulateStep(*this);
}



// ---------------------------------------------------------------------------------------------

// AddOrder constructor
AddOrder :: AddOrder(int id) : BasedAction(), customerId(id){}


// method
void AddOrder::act(WareHouse &wareHouse) {
    // Retrieve the customer based on the provided customer ID
    Customer& currCustomer = wareHouse.getCustomer(customerId);

    // Check if the customer is invalid or has reached their max order limit
    if(currCustomer.getId() == -1 || (currCustomer.getMaxOrders() - currCustomer.getNumOrders() <= 0)) {
        // Set the error status and message
        error("Cannot place this order");
        std::cout << getErrorMsg() << std::endl; // Print the error message
        return; // Exit the function as the order cannot be placed
    }

    // Create a new order with a unique ID for the customer
    Order* currOrder = new Order(wareHouse.getOrderCounter(), customerId, currCustomer.getCustomerDistance());
    
    // Add the new order to the warehouse
    wareHouse.addOrder(currOrder);

    // Assign the unique order ID to the customer's order
    currCustomer.addOrder(wareHouse.getOrderCounter());

    // Increment the warehouse's order counter to generate a new unique ID for future orders
    wareHouse.plusOrderCounter();

    // Set the status as completed
    complete();
}

string AddOrder ::toString() const {
   return "Order " + std::to_string(customerId) + " " + getStatusInprint();
}

AddOrder * AddOrder ::clone() const {
   return new AddOrder(*this);
}



// ---------------------------------------------------------------------------------------------

// AddCustomer constructor
AddCustomer :: AddCustomer(const string &customerName, const string &customerType, int distance, int maxOrders) : BasedAction(), customerName(customerName), customerType(converCustomerType(customerType)), distance(distance), maxOrders(maxOrders), customerTypeInString(customerType){}


// methods
void AddCustomer :: act(WareHouse &wareHouse) {
   Customer* curr = nullptr;
   if(customerTypeInString == "soldier")
   {
      curr = new SoldierCustomer(wareHouse.getCustomerCounter(), customerName, distance, maxOrders);
   }
   else
   {
      curr = new CivilianCustomer(wareHouse.getCustomerCounter(), customerName, distance, maxOrders);
   }
   wareHouse.addCustomer(curr);
   complete(); // This action never results in an error
}

string AddCustomer :: toString() const {
   return "customer " + customerName + " " + customerTypeInString + " " +std::to_string(distance) + " " + std::to_string(maxOrders) + " " + "COMPLETED";
}

AddCustomer *AddCustomer::clone() const {
    return new AddCustomer(*this);
}

// helper method 1
CustomerType AddCustomer :: converCustomerType(string customerType){
   if(customerType == "Soldier")
   // In C++, you can use the == operator to compare strings, as long as they are of type std::string. This operator compares the content of the strings, not just their memory addresses.
   {
      return CustomerType :: Soldier;
   }
   return CustomerType :: Civilian;
}

// ---------------------------------------------------------------------------------------------

// PrintOrderStatus constructor
PrintOrderStatus :: PrintOrderStatus(int id) : BasedAction(), orderId(id){}



// methods
void PrintOrderStatus::act(WareHouse &wareHouse) {
    // Retrieve a reference to the Order object with the given orderId from the warehouse
    // This uses the getOrder method of the WareHouse class, which searches for the order in various lists (pending, in process, completed)
    Order& currOrder = wareHouse.getOrder(orderId);

    // Check if the retrieved order is a special 'noSuchOrder' indicating it doesn't exist
    if(currOrder.getId() == -1) 
    {
        error("Order doesn’t exist");
        std::cout << getErrorMsg() << std::endl; // Print the error message
        return; // Exit the method since the order doesn't exist
    }

    // If the order exists, print its detailed string representation
    std::cout << currOrder.toString() << std::endl;

    // Mark this action as completed
    complete();
}


string PrintOrderStatus :: toString() const{
      return "orderStatus " + std::to_string(orderId) + " " + getStatusInprint();        
}   

PrintOrderStatus * PrintOrderStatus ::clone() const{
      return new PrintOrderStatus(*this);
}


// ---------------------------------------------------------------------------------------------

// PrintCustomerStatus constructor
PrintCustomerStatus :: PrintCustomerStatus(int customerId) : BasedAction(), customerId(customerId){}


// methods

// Method to print all information about a specific customer in the warehouse
void PrintCustomerStatus::act(WareHouse &wareHouse) {
    // Retrieve a reference to the Customer object with the given customerId from the warehouse
    // This uses the getCustomer method of the WareHouse class
    Customer& customer = wareHouse.getCustomer(customerId);

    // Check if the retrieved customer is a special 'noSuchCustomer' indicating they don't exist
    if (customer.getId() == -1) {
        error("Customer doesn’t exist"); // Set an error state with a message
        std::cout << getErrorMsg() << std::endl; // Print the error message
        return; // Exit the method since the customer doesn't exist
    }

    // Print customer ID
    std::cout << "CustomerID: " << customerId << std::endl;

    // Retrieve and print all orders associated with the customer
    const vector<int>& orders = customer.getOrdersIds();
    for (int orderId : orders) 
    {
        Order& order = wareHouse.getOrder(orderId);
        std::cout << "OrderID: " << orderId << std::endl;
        std::cout << "OrderStatus: " << order.orderStatusInString(order.getStatus()) << std::endl;
    }

    // Print the number of orders the customer can still place
    std::cout << "numOrdersLeft: " << (customer.getMaxOrders() - customer.getNumOrders()) << std::endl;

    // Mark this action as completed
    complete();
}

string PrintCustomerStatus :: toString() const {
    return "customerStatus " + std::to_string(customerId) + " " + getStatusInprint();
}

PrintCustomerStatus* PrintCustomerStatus :: clone() const {
   return new PrintCustomerStatus(*this);
}


// ---------------------------------------------------------------------------------------------

// PrintVolunteerStatus constructor
PrintVolunteerStatus :: PrintVolunteerStatus(int id) : BasedAction(), volunteerId(id){}


// methods

// Method to print all information about a specific volunteer in the warehouse
void PrintVolunteerStatus::act(WareHouse &wareHouse) {
    // Retrieve a reference to the Volunteer object with the given volunteerId from the warehouse
    // This uses the getVolunteer method of the WareHouse class
    Volunteer& currVolunteer = wareHouse.getVolunteer(volunteerId);

    // Check if the retrieved volunteer is a special 'noSuchVolunteer' indicating they don't exist
    if(currVolunteer.getId() == -1) 
    {
        error("Volunteer doesn’t exist"); // Set an error state with a message
        std::cout << getErrorMsg() << std::endl; // Print the error message
        return; // Exit the method since the volunteer doesn't exist
    }

    // Print volunteer information
    std::cout << currVolunteer.toString() << std::endl;

    // Mark this action as completed
    complete();
}

string PrintVolunteerStatus :: toString() const{
   return "volunteerStatus " + std::to_string(volunteerId) + " "+ getStatusInprint();
}
   
PrintVolunteerStatus * PrintVolunteerStatus ::clone() const {
   return new PrintVolunteerStatus(*this);
}



// ---------------------------------------------------------------------------------------------

// PrintActionsLog constructor
PrintActionsLog :: PrintActionsLog() : BasedAction(){}


// methods

void PrintActionsLog::act(WareHouse &wareHouse) {
   // Using range-based for loop to iterate through the container.
   const vector<BasedAction*> &log = wareHouse.getActions();
   for (const BasedAction* action : log) 
   {
      std::cout << action->toString() << std::endl;
   }
   complete();
}

string PrintActionsLog :: toString() const{
   return "log " + getStatusInprint();
}

PrintActionsLog * PrintActionsLog :: clone() const{
   return new PrintActionsLog(*this);
}

// ---------------------------------------------------------------------------------------------

// Close constructor
Close :: Close() : BasedAction(){}



// methods
void Close :: act(WareHouse &wareHouse) {
   int end = wareHouse.getOrderCounter();
   for (int i = 0; i < end; i++)
   {
      // Get the current order from the warehouse
      Order& currOrder = wareHouse.getOrder(i);
      int currCustomerId = currOrder.getCustomerId(); 
      OrderStatus currStatus = currOrder.getStatus();

      // Convert the order status to a string
      string statusInString = currOrder.orderStatusInString(currStatus);
      std::cout << "OrderID: " << i << ", " << "CustomerID: " << currCustomerId << ", " << "OrderStatus: " << statusInString << "\n";
   }

   complete();
   wareHouse.close();
}

string Close :: toString() const{
   return "close " + getStatusInprint();
}

Close * Close :: clone() const {
   return new Close(*this);
}

// ---------------------------------------------------------------------------------------------

// BackupWareHouse constructor
BackupWareHouse :: BackupWareHouse():BasedAction(){}


// methods
void BackupWareHouse :: act(WareHouse &wareHouse) {

   // Delete the existing backup if it exists
   if (backup != nullptr) 
   {
       delete backup;
       backup = nullptr;
   }
   
   backup = new WareHouse(wareHouse); // using copy constructor
   complete();
}

string BackupWareHouse ::  toString() const {
   return "backup " + getStatusInprint();
}       

BackupWareHouse * BackupWareHouse :: clone() const {
   return new BackupWareHouse(*this);
}

// ---------------------------------------------------------------------------------------------

// RestoreWareHouse constructor
RestoreWareHouse :: RestoreWareHouse() : BasedAction(){}



// methods
void RestoreWareHouse::act(WareHouse &wareHouse) {

   // Check if there is a backup available
   if (backup == nullptr)
   {
      // If backup is not available, set an error
      error("No backup available");
      std::cout << getErrorMsg() << std::endl; // Print the error message       
      return;
   }

   // Restore the warehouse state from the backup
   wareHouse = *backup;  // This will use the copy assignment operator of WareHouse

   complete();
}

string RestoreWareHouse ::  toString() const {
   return "restore " + getStatusInprint();
}

RestoreWareHouse * RestoreWareHouse :: clone() const {
   return new RestoreWareHouse(*this);
}
