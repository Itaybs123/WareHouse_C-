#include "WareHouse.h"
#include "BaseAction.h"
#include <iostream>
#include <sstream>

BaseAction :: BaseAction() : errorMsg("") , {}
ActionStatus BaseAction :: getStatus() const{}
void BaseAction :: complete(){}
void BaseAction :: error(string errorMsg){}
string BaseAction :: getErrorMsg() const{}