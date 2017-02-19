//
//  MonomeServer.hpp
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

#ifndef MonomeInput_hpp
#define MonomeInput_hpp

#define RECEIVER_PORT 6543
#define LOCALHOST "127.0.0.1"
#define SERIALOSC_PORT 12002

#include "../JuceLibraryCode/JuceHeader.h"
#include "AIMOInput.hpp"

class MonomeInput : public Timer,
                    public OSCReceiver::Listener<>,
                    public AIMOInput
{
public:
    struct MonomeData {
        String ID = "\0";
        String deviceName = "\0";
        int size[2] = {8,8};
        String host = "127.0.0.1";
        int port = 0;
        String prefix = "\0";
        int rotation = 0;
    };
    
    struct Grid{
        bool s = false;
        bool momentaryS = false;
        uint32 lastPress;
    };
    
    MonomeInput();
    ~MonomeInput();
    
    bool discover();
    
    // called by osc server on incomming osc messsages
    void oscMessageReceived (const OSCMessage& message) override;
    bool connectToDevice(MonomeData deviceInfo);
    void sendOscMessage ();
    void timerCallback() override;
    
private:
    OSCSender serialOSCSender;
    OSCReceiver serialOSCReceiver;
    
    OSCReceiver monomeReceive;
    OSCSender monomeSend;
    
    Grid grid[8][8];
    
    MonomeData monomeData;
    
    String connectedDeviceName;
    
    Time time;
    
    bool monomeConnected = false;
};

#endif /* defined(MonomeInput_hpp) */

