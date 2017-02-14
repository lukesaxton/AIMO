//
//  RoutableMidiOutput.cpp
//  AIMO
//
//  Created by Luke Saxton on 20/01/2017.
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

