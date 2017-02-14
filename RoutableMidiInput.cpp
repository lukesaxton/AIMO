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
//  AI:MO is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "RoutableMidiInput.hpp"
#include "AIMORouter.hpp"

RoutableMidiInput::RoutableMidiInput(const String deviceToConnectTo)
{
    refreshDeviceList();
    
    deviceToConnect = deviceToConnectTo;
    
    for (int i = 0; i < deviceList.size(); i++)
    {
        DBG(deviceList[i]);
        if (deviceList[i] == deviceToConnect)
        {
            input = MidiInput::openDevice(i, this);
            input->start();
            connected = true;
        }
    }
    
    for (int i = 0; i < 127; i++)
    {
        setKeyMapping("/controllerOne/grid/key/", i);
    }
    
    startTimer(1000);
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
        if (!message.isMidiClock())
        {
            DBG(message.getDescription());
        }
        
        
        AIMORouter::Instance()->routeMidi("/controllerOne/grid/key/", message);
        
    }
}


void RoutableMidiInput::paint(Graphics& g)
{
    g.fillAll(Colours::black);
    
    if (connected)
    {
        g.setColour(Colours::darkcyan);
    }
    else
    {
        g.setColour(Colours::darkgrey);
    }
    
    
    g.fillRect(3, 3, getWidth()-6, getHeight()-6);
    
    g.setColour(Colours::white);
    g.drawText(deviceToConnect.substring(0, 8), getLocalBounds(), Justification::centred);
}

void RoutableMidiInput::resized()
{
    
}

void RoutableMidiInput::mouseDown(const MouseEvent& event)
{
    if (event.mods.isRightButtonDown())
    {
        CallOutBox::launchAsynchronously(new ConfigComponent(this), getScreenBounds(), nullptr);
    }
}

void RoutableMidiInput::timerCallback()
{
    refreshDeviceList();
    bool foundDevice;
    for (int i = 0; i < deviceList.size(); i++)
    {
        if (deviceList[i] == deviceToConnect)
        {
            foundDevice = true;
        }
    }
    
    if (!foundDevice)
    {
        if (input != nullptr)
        {
            input->stop();
        }
        connected = false;
    }
    else
    {
        if (!connected)
        {
            for (int i = 0; i < deviceList.size(); i++)
            {
                DBG(deviceList[i]);
                if (deviceList[i] == deviceToConnect)
                {
                    input = MidiInput::openDevice(i, this);
                    input->start();
                    connected = true;
                }
            }
        }
    }
    
    repaint();
}

const String RoutableMidiInput::getDeviceToConnect()
{
    return deviceToConnect;
}

