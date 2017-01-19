//
//  AIMORouter.cpp
//  AIMO
//
//  Created by Luke Saxton on 19/01/2017.
//
//

#include "AIMORouter.hpp"
#include "VirtualMidiController.hpp"


AIMORouter* AIMORouter::pInstance = 0;


AIMORouter::AIMORouter()
{
    
}

AIMORouter::~AIMORouter()
{
    
}

AIMORouter* AIMORouter::Instance()
{
    if (pInstance == 0) //is this is the first call ?
    {
        //create sole instance
        pInstance = new AIMORouter();
    }
    //address of sole instance
    return pInstance;
}



bool AIMORouter::route(const String message)
{
    for (int i = 0; i < registeredDestinations.size(); ++i)
    {
        if (message.contains(registeredDestinations[i]->getID()))
        {
            registeredDestinations[i]->route(message);
            return true;
        }
    }
    
    return false;
}

void AIMORouter::addDestination(VirtualMidiController* destination)
{
    if(destination)
    {
        registeredDestinations.addIfNotAlreadyThere(destination);

    }
}
