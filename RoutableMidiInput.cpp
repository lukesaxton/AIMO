//
//  RoutableMidiInput.cpp
//  AIMO
//
//  Created by Luke Saxton on 14/02/2017.
//
//
//  AI:MO is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License, version 3,
//  as published by the Free Software Foundation.
//
//  AI::MO is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "RoutableMidiInput.hpp"
#include "AIMORouter.hpp"

RoutableMidiInput::RoutableMidiInput(const String deviceToConnetTo)
{
    refreshDeviceList();
    
    for (int i = 0; i < deviceList.size(); i++)
    {
        DBG(deviceList[i]);
        if (deviceList[i] == deviceToConnetTo)
        {
            input = MidiInput::openDevice(i, this);
            input->start();
        }
    }
    
    for (int i = 0; i < 127; i++)
    {
        setKeyMapping("/controllerOne/grid/key/", i);
    }
}

RoutableMidiInput::~RoutableMidiInput()
{
    input->stop();
}


void RoutableMidiInput::refreshDeviceList()
{
    deviceList = MidiInput::getDevices();
}


void RoutableMidiInput::handleIncomingMidiMessage (MidiInput* source, const MidiMessage& message)
{
    if (source == input)
    {
        //DBG("Midi Input Message Received");
        DBG(message.getDescription());
        
        
        AIMORouter::Instance()->routeMidi("/controllerOne/grid/key/", message);
        
    }
}
