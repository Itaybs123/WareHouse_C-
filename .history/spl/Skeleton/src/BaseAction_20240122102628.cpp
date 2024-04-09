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




SimulateStep :: SimulateStep(int numOfSteps) : numOfSteps(numOfSteps){}
void SimulateStep ::  act(WareHouse &wareHouse){}
std::string SimulateStep ::  toString() const {}
SimulateStep * SimulateStep :: clone() const {}