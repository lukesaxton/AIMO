//
//  AIMORouter.cpp
//  AIMO
//
//  Created by Luke Saxton on 19/01/2017.
//
//
//  AI:MO is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License, version 3,
//  as published by the Free Software Foundation.
//
//  AI:MO is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "AIMORouter.hpp"
#include "VirtualMidiController.hpp"
#include "RoutableMidiOutput.hpp"
#include "VMCModules.hpp"

AIMORouter* AIMORouter::pInstance = 0;


AIMORouter::AIMORouter()
{
    midiOutputs.add(new RoutableMidiOutput());
    midiOutputs.getLast()->createNewDevice("AIMO Out");
    
    midiOutputs.add(new RoutableMidiOutput());
    midiOutputs.getLast()->createNewDevice("AIMO Control");
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



bool AIMORouter::routeMidi (const String address, const MidiMessage message)
{
    String curSearch;
   
    for (int i = 0; i < midiOutputs.size(); i++)
    {
        curSearch = midiOutputs[i]->getDeviceName();
    
        if (address.contains(curSearch) && curSearch != "")
        {
            //DBG("Externally Routed " + address);

            midiOutputs[i]->routeMidi(address, message);
            return true;
        }
        
    }
    for (int i = 0; i < registeredDestinations.size(); i++)
    {
        curSearch = registeredDestinations[i]->getAddress();
        
        if (address.contains(curSearch) && curSearch != "")
        {
            //DBG("Internally Routed: " + address);
            
            registeredDestinations[i]->routeMidi(address, message);
            
            return true;
        }
        
    }
    
    DBG("Message not routed.. " + address);
    jassertfalse;
    
    return false;
}

bool AIMORouter::routeOSC (const OSCMessage message)
{
    String curSearch;
    
    for (int i = 0; i < registeredOSCDestinations.size(); i++)
    {
        curSearch = registeredOSCDestinations[i]->getOSCAddress();
        
        if (message.getAddressPattern().toString().contains(curSearch) && curSearch != "")
        {
            //DBG("Externally Routed " + address);
            
            registeredOSCDestinations[i]->routeOSC(message);
            return true;
        }
        
    }
    
    DBG("Message not routed.. " + message.getAddressPattern().toString());
    jassertfalse;
    
    return false;
}


void AIMORouter::addDestination(VMCModule* destination)
{
    if(destination)
    {
        registeredDestinations.addIfNotAlreadyThere(destination);

    }
}

void AIMORouter::addOSCDestination(VMC_OSCModule* destination)
{
    if(destination)
    {
        registeredOSCDestinations.addIfNotAlreadyThere(destination);
        
    }
}



bool mapMidi (const String address, const String source, const MidiMessage message)
{
    
    jassertfalse; // need to implemet this first..
    return false;
}
