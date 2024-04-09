#include "WareHouse.h"
#include "Action.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

// WareHouse constructor
WareHouse :: WareHouse(const string &configFilePath) : isOpen(true), actionsLog(), pendingOrders(), inProcessOrders(), completedOrders(), customers(),  volunteers(), customerCounter(0), volunteerCounter(0), ordercounter(0), noSuchCustomer(new SoldierCustomer(-1, "", -1, -1)), noSuchVolunteer(new LimitedCollectorVolunteer(-1, "", -1, -1)), noSuchOrder(new Order(-1,-1,-1)){ 

    // Open the configuration file
    std::ifstream configFile(configFilePath);
    if (!configFile.is_open()) 
    {
        std::cerr << "Error: Unable to open configuration file." << std::endl;
        exit(EXIT_FAILURE);
    }
    
    std::string line; // declares a string variable line to store each line read from the configuration file.
    while (std::getline(configFile, line)) // This while loop reads each line from the configFile and stores it in the line variable. The loop continues until there are no more lines to read from the file.
    {
        // Read each line from the configuration file
        std::istringstream iss(line); // an istringstream named iss is used to parse the contents of the line. istringstream is a stream that reads from a string.
        std::string keyword; 
        iss >> keyword; // This code reads the first word (token) from the line and stores it in the keyword variable

        // added segment:
        if(keyword != "#" && !line.empty())
        {
            parserLine(iss, keyword, true);
        }
        // This part checks if the first word (or 'keyword') in the line is not equal to "#". In many configuration files, a line starting with "#" is used to indicate a comment. Thus, this condition is ensuring that the line being processed is not a comment.
        // This part checks if the line is not empty. It's essential to ensure that the line contains some data before processing it further. This prevents the processing of blank lines which might be present in the file.
    }


    // Close the configuration file
    configFile.close(); // After all lines have been read and processed, this line closes the configuration file to free up system resources.
}

void WareHouse :: start(){
    std::cout << "Warehouse is open!" << std::endl;
    
    while(isOpen) 
    {
        std::cout << "Enter an action: ";
        std::string userCommand ,key;
        getline(std::cin , userCommand); // This line reads a line of text from the user via the console input (std::cin) and stores it in the userCommand variable.
        std::istringstream iss(userCommand); // This line creates an istringstream object named iss that is used to parse the contents of userCommand. An istringstream is a stream that reads from a string.
        iss >> key; // This code reads the first word (token) from the userCommand string and stores it in the key variable.
        parserLine(iss, key, false);
    }
}

const vector<BasedAction*> & WareHouse :: getActions() const{
    return actionsLog;
}

void WareHouse::addOrder(Order* order)
{
    OrderStatus status = order->getStatus();
    if (status == OrderStatus::PENDING || (status == OrderStatus::COLLECTING && order->getAfterCollecting()))
    {
        // If the order is PENDING or COLLECTING with no assigned collector, add it to the pendingOrders vector.
        pendingOrders.push_back(order);
    }
    else
    {
        if (status == OrderStatus::DELIVERING || status == OrderStatus::COLLECTING)
        {
            // If the order is DELIVERING or COLLECTING (with assigned collector), add it to the inProcessOrders vector.
            inProcessOrders.push_back(order);
        }
        else
        {
            // If the order is COMPLETED, add it to the completedOrders vector.
            completedOrders.push_back(order);
        }
    }
}

void WareHouse :: addAction(BasedAction* action){
    actionsLog.push_back(action);
}

Customer &WareHouse :: getCustomer(int customerId) const {
    // Iterate over each customer in the warehouse
    for (Customer* customerPtr : customers) 
    {
        // Check if the current customer's ID matches the requested ID
        if (customerPtr->getId() == customerId) 
        {
            // If a match is found, return a reference to this customer
            return *customerPtr;
        }
    }

    // If no customer with the given ID is found, return a reference to a predefined 'noSuchCustomer' object
    return *noSuchCustomer;
}

Volunteer &WareHouse :: getVolunteer(int volunteerId) const {
    // Iterate over each volunteer in the warehouse
    for (Volunteer* volunteerPtr : volunteers) 
    {
        // Check if the current volunteer's ID matches the requested ID
        if (volunteerPtr->getId() == volunteerId) 
        {
            // If a match is found, return a reference to this volunteer
            return *volunteerPtr;
        }
    }

    // If no volunteer with the given ID is found, return a reference to a predefined 'noSuchVolunteer' object
    return *noSuchVolunteer;
}

Order &WareHouse::getOrder(int orderId) const {
    // Iterate over each order in the 'pendingOrders' vector
    for (Order* orderPtr : pendingOrders) 
    {
        // Check if the current order's ID matches the requested ID
        if (orderPtr->getId() == orderId) 
        {
            // If a match is found, return a reference to this order
            return *orderPtr;
        }
    }

    // Iterate over each order in the 'inProcessOrders' vector
    for (Order* orderPtr : inProcessOrders)
     {
        // Check if the current order's ID matches the requested ID
        if (orderPtr->getId() == orderId) 
        {
            // If a match is found, return a reference to this order
            return *orderPtr;
        }
    }

    // Iterate over each order in the 'completedOrders' vector
    for (Order* orderPtr : completedOrders) 
    {
        // Check if the current order's ID matches the requested ID
        if (orderPtr->getId() == orderId) 
        {
            // If a match is found, return a reference to this order
            return *orderPtr;
        }
    }

    // If no order with the given ID is found, return a reference to a predefined 'noSuchOrder' object
    return *noSuchOrder;
}

void WareHouse :: close(){
    isOpen = false;
}

void WareHouse :: open(){
    isOpen = true;
}

vector<Order*>& WareHouse :: getPendingOrders(){
    return pendingOrders;
}

vector<Order*>& WareHouse :: getInProcessOrders(){
    return inProcessOrders;
}

vector<Order*>& WareHouse :: getcompletedOrder(){
    return completedOrders;
}

vector<Volunteer*>& WareHouse :: getVolunteers(){
    return volunteers;
}

vector<Customer*>& WareHouse :: getCustomerVector(){
    return customers;
}

int WareHouse :: getOrderCounter() const{
    return ordercounter;
}

int WareHouse :: getCustomerCounter() const{
    return customerCounter;
}

void WareHouse :: addCustomer(Customer* customer){
    customers.push_back(customer);
}

void WareHouse :: addVolunteer(Volunteer* volunteer){
    volunteers.push_back(volunteer);
}

Order & WareHouse :: getOrderInPending(int orderId) const{ 
    // Iterate over each order in the 'pendingOrders' vector
    for (Order* orderPtr : pendingOrders) 
    {
        // Check if the current order's ID matches the requested ID
        if (orderPtr->getId() == orderId) 
        {
            // If a match is found, return a reference to this order
            return *orderPtr;
        }
    }
    // If no order with the given ID is found, return a reference to a predefined 'noSuchOrder' object
    return *noSuchOrder;
}

Order & WareHouse :: getOrderInProccess(int orderId) const{

    // Iterate over each order in the 'inProcessOrders' vector
    for (Order* orderPtr : inProcessOrders)
     {
        // Check if the current order's ID matches the requested ID
        if (orderPtr->getId() == orderId) 
        {
            // If a match is found, return a reference to this order
            return *orderPtr;
        }
    }   

    // If no order with the given ID is found, return a reference to a predefined 'noSuchOrder' object
    return *noSuchOrder;
}

Order & WareHouse :: getOrderInCompleted(int orderId) const{
    // Iterate over each order in the 'completedOrders' vector
    for (Order* orderPtr : completedOrders) 
    {
        // Check if the current order's ID matches the requested ID
        if (orderPtr->getId() == orderId) 
        {
            // If a match is found, return a reference to this order
            return *orderPtr;
        }
    }

    // If no order with the given ID is found, return a reference to a predefined 'noSuchOrder' object
    return *noSuchOrder;
}

void WareHouse::deleteFromInProccess(Order* orderToDelete){
    // Use an iterator to traverse the inProcessOrders vector
    for (std::vector<Order*>::iterator it = inProcessOrders.begin(); it != inProcessOrders.end();)
     {
        // Check if the current order's ID matches the ID to be deleted
        if ((*it)->getId() == orderToDelete->getId()) 
        {
            // Erase the order from the vector and do not increment the iterator
            it = inProcessOrders.erase(it);
        } 
        else
        {
            // Increment the iterator to check the next element
            ++it;
        }
    }
}

void WareHouse :: plusOrderCounter(){
    ordercounter++;
}


// configFilePath ----------------------------------------------------------------------------------------------------------------------------------------

void WareHouse :: parserLine(std::istringstream& iss, std::string key, bool itsPhars) {
    
    if (key == "volunteer") 
    {
        Volunteer *newVolunteer = parseVolunteer(iss);       
        volunteers.push_back(newVolunteer);
    }

    else if (key == "customer") 
    {
        if (!itsPhars) // it is a command from the start() function
        {
           BasedAction *newCustomer = parseCustomer(iss);
           addAction(newCustomer);
        }
        else
        // it is a customer from the configfile
        {
            int locationDistance, maxOrders;
            std::string name, customerType;

            // Parse input data from the string stream
            // Extracts the customer's name, type, location distance, and max orders from the input stream 'iss'
            iss >> name >> customerType >> locationDistance >> maxOrders;
            Customer* curr = nullptr;
            if(customerType == "soldier")
            {
                curr = new SoldierCustomer(customerCounter, name, locationDistance, maxOrders);
            }
            else
            {
                curr = new CivilianCustomer(customerCounter, name, locationDistance, maxOrders);
            }
            customerCounter++;
            customers.push_back(curr);
        }    
    }

    else if (key == "step") 
    {
        BasedAction *step = parseStep(iss);
        addAction(step);
    }

    else if (key == "order")
    {
        BasedAction *order = parseOrder(iss);
        addAction(order);
    }

    else if (key == "orderStatus")
    {
        BasedAction *orderStatus = parseOrderStatus(iss);
        addAction(orderStatus);
    }

    else if (key == "customerStatus")
    {
        BasedAction *customerStatus = parseCustomerStatus(iss);
        addAction(customerStatus);
    }
    
    else if (key == "volunteerStatus") 
    {
        BasedAction *volunteerStatus = parseVolunteerStatus(iss);
        addAction(volunteerStatus);
    }
    
    else if (key == "log") 
    {
        BasedAction *log = parseLog(iss);
        addAction(log);
    }
    
    else if (key == "close") 
    {
        BasedAction *close = parseClose(iss);
        addAction(close);
    }
    
    else if (key == "backup") 
    {
        BasedAction *backup = parseBackup(iss);
        addAction(backup);
    }
    
    else if (key == "restore") 
    {
        BasedAction *restore = parseRestore(iss);
        addAction(restore);
    }
}

// Function to parse customer information and create a new AddCustomer action
BasedAction* WareHouse::parseCustomer(std::istringstream& iss) {
    int locationDistance, maxOrders;
    std::string name, customerType;

    // Parse input data from the string stream
    // Extracts the customer's name, type, location distance, and max orders from the input stream 'iss'
    iss >> name >> customerType >> locationDistance >> maxOrders;

    BasedAction* newCustomerAction = new AddCustomer(name, customerType, locationDistance, maxOrders);
    newCustomerAction->act(*this); // Execute the action to add the customer
    customerCounter++;  // Increment the counter after adding the customer
    return newCustomerAction; 
}

BasedAction* WareHouse::parseStep(std::istringstream& iss) {
    int step;
    iss >> step;

    // Create a new Order action based on parsed information
    BasedAction* newStepAction = new SimulateStep(step);
    newStepAction->act(*this); // Perform the action immediately
    return newStepAction;
}

BasedAction* WareHouse::parseOrder(std::istringstream& iss) {
    int orderId;
    iss >> orderId;

    // Create a new Order action based on parsed information
    BasedAction* newOrderAction = new AddOrder(orderId);
    newOrderAction->act(*this);  // Perform the action immediately
    return newOrderAction;
}

BasedAction* WareHouse::parseOrderStatus(std::istringstream& iss) {
    int orderId;
    iss >> orderId;

    // Create a new PrintOrderStatus action based on parsed information
    BasedAction* newOrderStatusAction = new PrintOrderStatus(orderId);
    newOrderStatusAction->act(*this);  // Perform the action immediately
    return newOrderStatusAction;
}

BasedAction* WareHouse::parseCustomerStatus(std::istringstream& iss) {
    int customerId;
    iss >> customerId;

    // Create a new PrintCustomerStatus action based on parsed information
    BasedAction* newCustomerStatusAction = new PrintCustomerStatus(customerId);
    newCustomerStatusAction->act(*this);  // Perform the action immediately
    return newCustomerStatusAction;
}

BasedAction* WareHouse::parseVolunteerStatus(std::istringstream& iss) {
    int volunteerId;
    iss >> volunteerId;

    // Create a new PrintVolunteerStatus action based on parsed information
    BasedAction* newVolunteerStatusAction = new PrintVolunteerStatus(volunteerId);
    newVolunteerStatusAction->act(*this);  // Perform the action immediately
    return newVolunteerStatusAction;
}

BasedAction* WareHouse::parseLog(std::istringstream& iss) {
    // Create a new PrintActionsLog action
    BasedAction* newLogAction = new PrintActionsLog();
    newLogAction->act(*this);
    return newLogAction;
}

BasedAction* WareHouse::parseClose(std::istringstream& iss) {
    // Create a new Close action
    BasedAction* newCloseAction = new Close();
    newCloseAction->act(*this);
    return newCloseAction;
}

BasedAction* WareHouse::parseBackup(std::istringstream& iss) {
    // Create a new BackupWareHouse action
    BasedAction* newBackupAction = new BackupWareHouse();
    newBackupAction->act(*this);
    return newBackupAction;
}

BasedAction* WareHouse::parseRestore(std::istringstream& iss) {
    // Create a new RestoreWareHouse action
    BasedAction* newRestoreAction = new RestoreWareHouse();
    newRestoreAction->act(*this);
    return newRestoreAction;
}

// Function to parse volunteer information and create a new Volunteer action
Volunteer* WareHouse::parseVolunteer(std::istringstream& iss) {
    // Additional attributes specific to certain volunteer types
    
    string name, volunteerRole;
    iss >> name >> volunteerRole;

    // Common attributes for all volunteer types
    int coolDown = 0, maxDistance = 0, distancePerStep = 0, maxOrders = 0;

    // Create the appropriate volunteer object based on volunteerRole
    Volunteer* newVolunteer = nullptr;

    if(volunteerRole == "collector")
    {
        iss >> coolDown;
        newVolunteer = new CollectorVolunteer(volunteerCounter, name, coolDown);
    }

    else if(volunteerRole == "limited_collector")
    {
        iss >> coolDown;
        iss >> maxOrders;
        newVolunteer = new LimitedCollectorVolunteer(volunteerCounter, name, coolDown, maxOrders);
    }

    else if(volunteerRole == "driver")
    {
        iss >> maxDistance;
        iss >> distancePerStep;
        newVolunteer = new DriverVolunteer(volunteerCounter, name, maxDistance, distancePerStep);
    }

    else
    {
        iss >> maxDistance;
        iss >> distancePerStep;
        iss >> maxOrders;
        newVolunteer = new LimitedDriverVolunteer(volunteerCounter, name, maxDistance, distancePerStep, maxOrders);
    }

    volunteerCounter++;
    return newVolunteer;
}

// configFilePath ----------------------------------------------------------------------------------------------------------------------------------------


// Rule of 5

// WareHouse destructor
WareHouse::~WareHouse() {
    // The purpose of a destructor is to release resources and perform other cleanup processes when an instance of the class is destroyed. 
    // Note: In modern C++ code, checking for null pointers before calling delete is generally not necessary. The delete operator is safe to call on null pointers — it simply does nothing if the pointer is null. This behavior is defined by the C++ standard.

    for (BasedAction* actionPtr : actionsLog) 
    {
        if (actionPtr != nullptr) // meaning it points to a valid object
        {
            delete actionPtr;
        }
    }
    actionsLog.clear(); // This method is used to remove all elements from the vector, effectively reducing its size to 0. ensures that your vector doesn't hold any dangling pointers (pointers that point to deallocated memory).
    

    for (Volunteer* volunteerPtr : volunteers) 
    {
        if (volunteerPtr != nullptr)
         {
            delete volunteerPtr;
        }
    }
    volunteers.clear(); // This method is used to remove all elements from the vector, effectively reducing its size to 0. ensures that your vector doesn't hold any dangling pointers (pointers that point to deallocated memory).

    for (Customer* customerPtr : customers) 
    {
        if (customerPtr != nullptr)
         {
            delete customerPtr;
        }
    }
    customers.clear(); // This method is used to remove all elements from the vector, effectively reducing its size to 0. ensures that your vector doesn't hold any dangling pointers (pointers that point to deallocated memory).

    for (Order* orderPtr : pendingOrders) 
    {
        if (orderPtr != nullptr) 
        {
            delete orderPtr;
        }
    }
    pendingOrders.clear(); // This method is used to remove all elements from the vector, effectively reducing its size to 0. ensures that your vector doesn't hold any dangling pointers (pointers that point to deallocated memory).

    for (Order* orderPtr : inProcessOrders) 
    {
        if (orderPtr != nullptr)
         {
            delete orderPtr;
        }
    }
    inProcessOrders.clear(); // This method is used to remove all elements from the vector, effectively reducing its size to 0. ensures that your vector doesn't hold any dangling pointers (pointers that point to deallocated memory).

    for (Order* orderPtr : completedOrders) 
    {
        if (orderPtr != nullptr) 
        {
            delete orderPtr;
        }
    }
    completedOrders.clear(); // This method is used to remove all elements from the vector, effectively reducing its size to 0. ensures that your vector doesn't hold any dangling pointers (pointers that point to deallocated memory).

    if (noSuchCustomer != nullptr) // meaning it points to a valid object
    {
            delete noSuchCustomer;
    }

    if (noSuchVolunteer != nullptr) // meaning it points to a valid object
    {
            delete noSuchVolunteer;
    }

    if (noSuchOrder != nullptr) // meaning it points to a valid object
    {
            delete noSuchOrder;
    }
}

// Copy constructor for WareHouse
WareHouse::WareHouse(const WareHouse &other) :
    isOpen(other.isOpen), actionsLog(), pendingOrders(), inProcessOrders(), completedOrders(), customers(),  volunteers(),
    customerCounter(other.customerCounter),
    volunteerCounter(other.volunteerCounter),
    ordercounter(other.ordercounter),
    noSuchCustomer(new SoldierCustomer(*(other.noSuchCustomer))),
    noSuchVolunteer(new LimitedCollectorVolunteer(*(other.noSuchVolunteer))),
    noSuchOrder(new Order(*(other.noSuchOrder)))
    {

    // create a new object as a copy of an existing object

    // Deep copy of actionsLog
    for (BasedAction* action : other.actionsLog) 
    {
        actionsLog.push_back(action->clone());
    }

    // Deep copy of volunteers
    for (Volunteer* volunteer : other.volunteers)
    {
        volunteers.push_back(volunteer->clone());
        // explain clone()
        // The clone() method is implemented in each derived class of Volunteer.
        // It creates a new instance of that specific class and returns a pointer to it.
        // it ensures that the correct type of object is created even when you only have a pointer to the base Volunteer type.
        // the clone() method is an elegant solution for copying polymorphic objects.
        // It ensures that each specific type of volunteer is correctly copied with all its unique attributes, maintaining the polymorphism in the copied WareHouse instance. 
    }

    // Deep copy of customers
    for (Customer* customer : other.customers)
    {
        customers.push_back(customer->clone());
    }

    // Deep copy of orders in various stages
    for (Order* order : other.pendingOrders)
    {
        pendingOrders.push_back(new Order(*order));
    }

    for (Order* order : other.inProcessOrders)
    {
        inProcessOrders.push_back(new Order(*order));
    }

    for (Order* order : other.completedOrders) 
    {
        completedOrders.push_back(new Order(*order));
    }
}

// WareHouse copy assignment operator
WareHouse& WareHouse::operator=(const WareHouse &other) {
    // The copy assignment operator is used to define the behavior when an existing instance of a class (this) is assigned the value of another instance of the same class (other). 

    // Check for self-assignment to avoid unnecessary work and potential issues
    if (this != &other) // This check is crucial to prevent issues that could arise from self-assignment, such as inadvertently deleting the object's own data.
    {
        // Before copying data from other, the operator deletes any dynamically allocated memory associated with the current object (this).
        // This is to prevent memory leaks.
        
        // Note: In modern C++ code, checking for null pointers before calling delete is generally not necessary. The delete operator is safe to call on null pointers — it simply does nothing if the pointer is null. This behavior is defined by the C++ standard.

        for (BasedAction* action : actionsLog) 
        {
            if (action != nullptr) // meaning it points to a valid object
            {
                delete action;
            }
        }
        actionsLog.clear(); 

        for (Volunteer* volunteer : volunteers) 
        {
            if (volunteer != nullptr) // meaning it points to a valid object
            {
                delete volunteer;
            }
        }
        volunteers.clear();

        for (Order* order : pendingOrders) 
        {
            if (order != nullptr) // meaning it points to a valid object
            {
                delete order;
            }
        }
        pendingOrders.clear();

        for (Order* order : inProcessOrders) 
        {
            if (order != nullptr) // meaning it points to a valid object
            {
                delete order;
            }            
        }
        inProcessOrders.clear();

        for (Order* order : completedOrders) 
        {
            if (order != nullptr) // meaning it points to a valid object
            {
                delete order;
            }  
        }
        completedOrders.clear();

        for (Customer* customer : customers) 
        {
            if (customer != nullptr) // meaning it points to a valid object
            {
                delete customer;
            }  
        }
        customers.clear();
        // Deleting a dynamically allocated object does not automatically remove the pointer from the vector. It only frees the memory where the object was stored.
        // If you don't use clear(), the vector would still hold pointers, but these pointers would be dangling because they point to freed memory. 
        // Accessing such pointers would lead to undefined behavior.
        // removes all elements from the vector, leaving the container with a size of 0.
        // This ensures that the vectors are emptied and no longer contain any pointers (including dangling pointers). 
        // A dangling pointer is a pointer that refers to memory that has been deallocated or released.

        if (noSuchCustomer != nullptr) // meaning it points to a valid object
        {
            delete noSuchCustomer;
        }

        if (noSuchVolunteer != nullptr) // meaning it points to a valid object
        {
                delete noSuchVolunteer;
        }

        if (noSuchOrder != nullptr) // meaning it points to a valid object
        {
                delete noSuchOrder;
        }

        // Scalar fields can be directly copied
        isOpen = other.isOpen;
        volunteerCounter = other.volunteerCounter;
        customerCounter = other.customerCounter;
        ordercounter = other.ordercounter;

        // Perform a deep copy of the actionsLog
        for (BasedAction* action : other.actionsLog) 
        {
            actionsLog.push_back(action->clone());
        }

        // Perform a deep copy of the volunteers
        for (Volunteer* volunteer : other.volunteers)
         {
            volunteers.push_back(volunteer->clone());
        }

        // Perform a deep copy of the customers
        for (Customer* customer : other.customers) {
            customers.push_back(customer->clone());
        }

        // Deep copy of orders in various stages (pending, in process, completed)
        for (Order* order : other.pendingOrders)
        {
            pendingOrders.push_back(new Order(*order));
        }

        for (Order* order : other.inProcessOrders) 
        {
            inProcessOrders.push_back(new Order(*order));
        }

        for (Order* order : other.completedOrders)
        {
            completedOrders.push_back(new Order(*order));
        }

        // Copy special pointers representing default cases (e.g., no such customer)
        // It's important to clone these to maintain separate instances
        noSuchCustomer = new SoldierCustomer(*(other.noSuchCustomer));
        noSuchVolunteer = new LimitedCollectorVolunteer(*(other.noSuchVolunteer));
        noSuchOrder = new Order(*(other.noSuchOrder));
    }

    // Return a reference to this object
    return *this;
}

// WareHouse move constructor
WareHouse::WareHouse(WareHouse &&other) :  
    isOpen(other.isOpen),
    // Transfers ownership of resources from the source object (other) to this new instance
    // Transfer vectors using std::move. This is efficient as it avoids deep copying.
    // It transfers the ownership of dynamic memory without duplicating the content.
    actionsLog(std::move(other.actionsLog)),
    pendingOrders(std::move(other.pendingOrders)),
    inProcessOrders(std::move(other.inProcessOrders)),
    completedOrders(std::move(other.completedOrders)),
    customers(std::move(other.customers)),
    volunteers(std::move(other.volunteers)),

    customerCounter(other.customerCounter),
    volunteerCounter(other.volunteerCounter),
    ordercounter(other.ordercounter),

    // Transfer ownership of the pointers. After transfer, these pointers in 'other' will no longer be valid.
    noSuchCustomer(other.noSuchCustomer),
    noSuchVolunteer(other.noSuchVolunteer),
    noSuchOrder(other.noSuchOrder) {
    
    // After transferring ownership of the resources, it's essential to set the pointers in the 'other' (source) object to nullptr. 
    // This action is critical to ensure resource safety. When the 'other' object is eventually destroyed or goes out of scope, 
    // its destructor will be invoked. If these pointers are not nullified, the destructor will attempt to delete these resources, 
    // leading to a double deletion error since these resources are now owned and managed by the current (this) object. 
    // Setting these pointers to nullptr effectively tells the destructor of 'other' that there's nothing to delete, 
    // thus preventing any potential memory corruption or program crashes due to double deletion.
    other.noSuchCustomer = nullptr;
    other.noSuchVolunteer = nullptr;
    other.noSuchOrder = nullptr;

    // Clearing vectors in the source object, 'other'. This step is optional as the ownership
    // of the contents has already been transferred. This is more about leaving 'other' in a clean state.
    other.actionsLog.clear();
    other.volunteers.clear();
    other.pendingOrders.clear();
    other.inProcessOrders.clear();
    other.completedOrders.clear();
    other.customers.clear();

    // Reset state variables of 'other'. This is part of good practice to leave the moved-from
    // object in a well-defined state, even though it's typically not going to be used further.
    other.isOpen = false;
    other.volunteerCounter = 0;
    other.customerCounter = 0;
    other.ordercounter = 0;
}

// WareHouse move assignment operator
WareHouse& WareHouse::operator=(WareHouse &&other) {
    // Check for self-assignment
    if (this != &other) // This check is essential to prevent issues that could arise from self-assignment, such as inadvertently deleting the object's own data.
    {
        // Before adopting new resources from other, the operator deletes any dynamically allocated memory associated with the current object (this).
        // This is to prevent memory leaks. It iterates through vectors like actionsLog, volunteers, customers, etc., and deletes each object these vectors are pointing to.

        // Clean up existing resources
        for (BasedAction* action : actionsLog) 
        {
            if (action != nullptr) // meaning it points to a valid object
            {
                delete action;
            }
        }

        for (Volunteer* volunteer : volunteers) 
        {
            if (volunteer != nullptr) // meaning it points to a valid object
            {
                delete volunteer;
            }
        }

        for (Order* order : pendingOrders) 
        {
            if (order != nullptr) // meaning it points to a valid object
            {
                delete order;
            }
        }

        for (Order* order : inProcessOrders) 
        {
            if (order != nullptr) // meaning it points to a valid object
            {
                delete order;
            }
        }

        for (Order* order : completedOrders) 
        {
            if (order != nullptr) // meaning it points to a valid object
            {
                delete order;
            }
        }

        for (Customer* customer : customers) 
        {
            if (customer != nullptr) // meaning it points to a valid object
            {
                delete customer;
            }
        }

        if (noSuchCustomer != nullptr) // meaning it points to a valid object
        {
            delete noSuchCustomer;
        }

        if (noSuchVolunteer != nullptr) // meaning it points to a valid object
        {
                delete noSuchVolunteer;
        }

        if (noSuchOrder != nullptr) // meaning it points to a valid object
        {
                delete noSuchOrder;
        }

        // Scalar fields can be directly transferred
        isOpen = other.isOpen;
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        ordercounter = other.ordercounter;

        // Move the resources and adopt ownership
        // std::move is used.
        // std::move transfers the ownership of these resources from other to the current object (this). 
        // This is efficient as it avoids deep copying; it essentially reassigns the internal pointers and states of these resources.
        actionsLog = std::move(other.actionsLog);
        volunteers = std::move(other.volunteers);
        customers = std::move(other.customers);
        pendingOrders = std::move(other.pendingOrders);
        inProcessOrders = std::move(other.inProcessOrders);
        completedOrders = std::move(other.completedOrders);

        // Transfer ownership of the unique resource pointers
        noSuchCustomer = other.noSuchCustomer;
        noSuchVolunteer = other.noSuchVolunteer;
        noSuchOrder = other.noSuchOrder;

        // Set the resources in 'other' to null or default state
        // This prevents double deletion when the other object is destroyed.
        other.noSuchCustomer = nullptr;
        other.noSuchVolunteer = nullptr;
        other.noSuchOrder = nullptr;
        other.actionsLog.clear();
        other.volunteers.clear();
        other.customers.clear();
        other.pendingOrders.clear();
        other.inProcessOrders.clear();
        other.completedOrders.clear();
        other.isOpen = false;
        other.customerCounter = 0;
        other.volunteerCounter = 0;
        other.ordercounter = 0;
    }

    // Return a reference to this object
    return *this; // This allows for chaining of assignment operations.
}
