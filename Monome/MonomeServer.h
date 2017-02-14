//
//  MonomeServer.h
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

#ifndef __LED_Metronome__MonomeServer__
#define __LED_Metronome__MonomeServer__

#define RECEIVER_PORT 6543
#define LOCALHOST "127.0.0.1"
#define SERIALOSC_PORT 12002

#include "../JuceLibraryCode/JuceHeader.h"
#include "Monome.hpp"

class MonomeServer : public Timer,
                     public OSCReceiver::Listener<>
{
public:
    MonomeServer()
    {
        // listen on port
        if(receiver.connect(RECEIVER_PORT))
        {
            DBG("OSC Receiver connected, Port: " + String(RECEIVER_PORT));
        }
        else
        {
            jassertfalse;
        }
        
        // start listening
        receiver.addListener(this);
        
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
        
        
        discover();
        startTimer(5000);
    }
    ~MonomeServer()
    {

    }
    
    bool discover()
    {
        OSCAddressPattern address("/serialosc/list");
        
        OSCMessage message(address);
        message.addString("127.0.0.1");
        message.addInt32(RECEIVER_PORT);
    
        serialOSCSender.send(message);
        
        
        return false;
    }
    
    // called by osc server on incomming osc messsages
    void oscMessageReceived (const OSCMessage& message)
    {
        String console = "Message received ";
        
        
        if (message.getAddressPattern().toString() == "/serialosc/device") //valid device to connect to
        {
            if (message.size() > 0)
            {
                //monomeData.
                stopTimer();
                
                OSCArgument* curArg = message.begin();
                Monome::MonomeData data;

                data.deviceName = curArg->getString();
                curArg++;
                data.ID = curArg->getString();
                curArg++;
                data.port = curArg->getInt32();
                
                
                console += data.deviceName + " : " + data.ID + " : " + String(data.port);
                
                DBG(console);
                
                monome.connectToDevice(data);
                
                //monomeSender.connect(<#const juce::String &targetHostName#>, <#int targetPortNumber#>)
                
            }
        }
        
       
        
    }
    
    
    // send out a osc message
    void sendOscMessage ()
    {

        
//        osc::OutboundPacketStream oscMessage (buffer, bufferSize);
//        oscMessage << osc::BeginMessage (address.toRawUTF8 ()) << 0.0 <<osc::EndMessage;
//        oscServer->sendMessage (oscMessage);
    }
    
    //=====================================================================================
    
    void timerCallback()
    {
        discover();
    }
    
private:
    OSCSender serialOSCSender;
    OSCReceiver receiver;
    
    Monome monome;
    
    bool monomeConnected = false;
};

#endif /* defined(__LED_Metronome__MonomeServer__) */

