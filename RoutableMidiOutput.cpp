//
//  RoutableMidiOutput.cpp
//  AIMO
//
//  Created by Luke Saxton on 20/01/2017.
//
//

#include "RoutableMidiOutput.hpp"


RoutableMidiOutput::RoutableMidiOutput()
{

}

RoutableMidiOutput::~RoutableMidiOutput()
{
    
}


bool RoutableMidiOutput::connectToDevice(const String deviceName)
{
    StringArray devices = output->getDevices();
    for (int i = 0; i < devices.size(); ++i)
    {
        if (devices[i] == deviceName)
        {
            output = MidiOutput::openDevice(i);
            return true;
            
        }
    }

    return false;
    
}

bool RoutableMidiOutput:: createNewDevice(const String deviceName)
{
    
    output = MidiOutput::createNewDevice(deviceName);
    if (output != nullptr)
    {
        return true;
    }
    else
    {
        return false;
    }
    
}


bool RoutableMidiOutput::routeMidi (const String address, const MidiMessage message)
{
    if (address.contains(output->getName()))
    {
        if (output != nullptr)
        {
            output->sendMessageNow(message);
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
    
}
const String RoutableMidiOutput::getDeviceName()
{
    return output->getName();
}

