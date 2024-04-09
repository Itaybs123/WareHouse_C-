#include "WareHouse.h"
#include "BaseAction.h"
#include <iostream>
#include <sstream>

BaseAction :: BaseAction() : errorMsg("") , status(ActionStatus :: ERROR){}
ActionStatus BaseAction :: getStatus() const{
    return status;
}
void BaseAction :: complete(){}
void BaseAction :: error(string errorMsg){}
string BaseAction :: getErrorMsg() const{}