//
//  MonomeInput.cpp
//  AI:MO
//
//  Created by Luke Saxton on 30/08/2015.
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

#include "MonomeInput.hpp"
#include "AIMORouter.hpp"

#define MONOME_RECEIVE 7865


MonomeInput::MonomeInput()
{
    // listen on port
    if(serialOSCReceiver.connect(RECEIVER_PORT))
    {
        DBG("OSC Receiver connected, Port: " + String(RECEIVER_PORT));
    }
    else
    {
        jassertfalse;
    }
    
    // start listening
    serialOSCReceiver.addListener(this);
    
    // Connect to SerialOSC
    ;
    if(serialOSCSender.connect("127.0.0.1", SERIALOSC_PORT))
    {
        DBG("OSC Sender connected, Port: 12002");
    }
    else
    {
        jassertfalse;
    }
    
    for (int i = 0; i < 128; i++)
    {
        setKeyMapping("/KTM/key", i);
    }
    
    monomeReceive.addListener(this);
    
    discover();
    startTimer(5000);
}
MonomeInput::~MonomeInput()
{
    
}

bool MonomeInput::discover()
{
    OSCAddressPattern address("/serialosc/list");
    
    OSCMessage message(address);
    message.addString("127.0.0.1");
    message.addInt32(RECEIVER_PORT);
    
    serialOSCSender.send(message);
    
    
    return false;
}

// called by osc server on incomming osc messsages
void MonomeInput::oscMessageReceived (const OSCMessage& message)
{
    String console = "Message received ";
    
    
    if (message.getAddressPattern().toString() == "/serialosc/device") //valid device to connect to
    {
        if (message.size() > 0)
        {
            //monomeData.
            stopTimer();
            
            OSCArgument* curArg = message.begin();
            MonomeInput::MonomeData data;
            
            data.deviceName = curArg->getString();
            curArg++;
            data.ID = curArg->getString();
            curArg++;
            data.port = curArg->getInt32();
            
            
            console += data.deviceName + " : " + data.ID + " : " + String(data.port);
            
            DBG(console);
            
            connectToDevice(data);
            
            //monomeSender.connect(<#const juce::String &targetHostName#>, <#int targetPortNumber#>)
            
        }
    }
    else if (message.getAddressPattern().toString() == (monomeData.prefix + "/grid/key"))
    {
        
        //stopIdle();
        OSCArgument* cur = message.begin();
        
        OSCArgument xArg(*cur);
        OSCArgument yArg(*++cur);
        OSCArgument sArg(*++cur);
        
        int x, y, s;
        x = xArg.getInt32();
        y = yArg.getInt32();
        s = sArg.getInt32();
        
        if (x < monomeData.size[0] && y < monomeData.size[1])
        {
            grid[x][y].lastPress = time.getMillisecondCounter();
            grid[x][y].s = s;
            //setLight(x, y, s);
            
            int padNum = x + (y*8);
            
            String mappingString = getKeyMapping(padNum);
            
            MidiMessage midiMessage;
            
            if (s)
            {
                midiMessage = MidiMessage::noteOn(1, padNum, uint8(120));
                
            }
            else
            {
                midiMessage = MidiMessage::noteOff(1, padNum);
            }
            midiMessage.setChannel(1);
            
            
            AIMORouter::Instance()->routeMidi(mappingString, midiMessage);
            
            
//            if (numberMode && keysPressed == 0)
//            {
//                displayNumber(((y*8)+x)+1);
//            }
//            
//            if (s)
//            {
//                keysPressed++;
//            }
//            else{
//                keysPressed--;
//                
//            }
//            
//            //DBG("Keys Pressed: " + String(keysPressed));
//            
//            
//            
//            if (keysPressed == 2) {
//                if (grid[0][0].s && grid[1][0].s)
//                {
//                    if (Timer::isTimerRunning())
//                    {
//                        stopIdle();
//                    }
//                    else
//                    {
//                        Timer::startTimer(100);
//                    }
//                }
//                else if (grid[6][7].s && grid[7][7].s) //kill all lights
//                {
//                    Timer::stopTimer();
//                    lightsOff();
//                }
//                else if (grid[7][6].s && grid[7][7].s) //show all Lights
//                {
//                    Timer::stopTimer();
//                    lightsOn();
//                }
//                else if (grid[7][0].s && grid[7][1].s) //enable number mode
//                {
//                    numberMode = true;
//                }
//                else if (grid[7][0].s && grid[6][0].s) //disable number mode
//                {
//                    numberMode = false;
//                    lightsOff();
//                }
//            }
        }
        else
        {
            DBG(message.getAddressPattern().toString());
        }
        
    }
    
}

bool MonomeInput::connectToDevice(MonomeData deviceInfo)
{
    
    monomeData = deviceInfo;
    
    if (monomeReceive.connect(MONOME_RECEIVE) && monomeSend.connect(monomeData.host, monomeData.port))
    {
        monomeConnected = true;
        
        OSCMessage setPrefix("/sys/prefix");
        monomeData.prefix = "/aimo";
        setPrefix.addString(monomeData.prefix);
        monomeSend.send(setPrefix);
        
        OSCMessage setPort("/sys/port");
        setPort.addInt32(MONOME_RECEIVE);
        monomeSend.send(setPort);
        
        //setIntensity(15);
        
        //Timer::startTimer(100);
        
        //setMask(CircleMasks::ring4);
        
        //HighResolutionTimer::startTimer(20);
        
        
        //setMask(LetterMasks::Z);
        
    }
    else{
        jassertfalse;
        return false;
    }
    
    return true;
    
    //pollConnectedMonome();
    
    //startIdle();
    
    
}

// send out a osc message
void MonomeInput::sendOscMessage ()
{
    
    
}

//=====================================================================================

void MonomeInput::timerCallback()
{
    discover();
    
    
}
