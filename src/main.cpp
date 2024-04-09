#include "WareHouse.h"
#include <iostream>

using namespace std;

WareHouse* backup = nullptr;

int main(int argc, char** argv)
{
    if(argc!=2)
    {
        std::cout << "usage: warehouse <config_path>" << std::endl;
        return 0;
    }
    string configurationFile = argv[1];
    WareHouse wareHouse(configurationFile);
    wareHouse.start();
    if(backup!=nullptr)
    {
    	delete backup;
        // The line delete backup; in your main function calls the destructor of the WareHouse object pointed to by the backup pointer.
        // It deallocates the memory used by that specific WareHouse object.
    	backup = nullptr;
    }
    return 0; 
}