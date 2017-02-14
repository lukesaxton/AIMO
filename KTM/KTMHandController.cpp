//
//  KTMHandController.cpp
//  AIMO
//
//  Created by Luke Saxton on 08/02/2017.
//
//

#include "KTMHandController.hpp"
#include "AIMORouter.hpp"


#define KTM_CONTROLLER_PORT 8010

KTMHandController::KTMHandController()
{
    controllerReceive.addListener(this);
    connect();
    
    for (int i = 0; i < NUM_KTM_BUTTONS; i++)
    {
        stateGrid[i] = 0;
        setKeyMapping("/controllerOne/grid/key", i);
    }
}

KTMHandController::~KTMHandController()
{
    
}

bool KTMHandController::connect()
{
    if (controllerReceive.connect(KTM_CONTROLLER_PORT))
    {
        DBG("Connected to KTMController receive port");
        return true;
    }
    else
    {
        return false;
    }
    
   
}

void KTMHandController::oscMessageReceived (const OSCMessage& message)
{
    //DBG(message.getAddressPattern().toString() + " ARGS: " + String(message.size()));
    
    String address = message.getAddressPattern().toString();
    
    if (address == "/inputs/digital")
    {
        OSCArgument* curArg = message.begin();;
        for (int i = 0; i < message.size(); i++)
        {
            if (curArg->isInt32())
            {
                if (stateGrid[i] != curArg->getInt32())
                {
                    stateGrid[i] = curArg->getInt32();
                    
                    if (stateGrid[i] == 1)
                    {
                        DBG("KTMHC Button On: " + String(i));
                        AIMORouter::Instance()->routeMidi(getKeyMapping(i), MidiMessage::noteOn(1, i, uint8(110)));
                    }
                    else if (stateGrid[i] == 0)
                    {
                        DBG("KTMHC Button Off: " + String(i));
                        AIMORouter::Instance()->routeMidi(getKeyMapping(i), MidiMessage::noteOff(1, i));
                    }
                }
            }
            curArg++;
            
            
        }
    }
}

