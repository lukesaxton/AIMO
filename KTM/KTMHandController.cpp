//
//  KTMHandController.cpp
//  AIMO
//
//  Created by Luke Saxton on 08/02/2017.
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

#include "KTMHandController.hpp"
#include "AIMORouter.hpp"


#define KTM_CONTROLLER_RECEIVE 8010
#define KTM_CONTROLLER_SEND 9010


KTMHandController::KTMHandController()
{
    controllerReceive.addListener(this);
    connect();
    
    for (int i = 0; i < NUM_KTM_BUTTONS; i++)
    {
        stateGrid[i] = 0;
        setKeyMapping("/controllerOne/grid/key", i);
        setButtonLED(i, false);
    }
    
    for (int i = 0; i < NUM_RGB_LEDS; i++)
    {
        ledColours.add(new Colour(Colours::black));
    }
    
    refreshColourLEDs();
    
    startTimer(50);
}

KTMHandController::~KTMHandController()
{
    
}

bool KTMHandController::connect()
{
    if (controllerReceive.connect(KTM_CONTROLLER_RECEIVE) && controllerSend.connect("169.254.1.1", KTM_CONTROLLER_SEND))
    {
        DBG("Connected to KTMController ports");
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
    
    static String address;
    address = message.getAddressPattern().toString();
    
    
    if (address == "/inputs/digital")
    {
        OSCArgument* curArg = message.begin();;
        for (int i = 0; i < NUM_KTM_BUTTONS; i++)
        {
            if (curArg->isInt32())
            {
                if (stateGrid[i] != curArg->getInt32())
                {
                    stateGrid[i] = curArg->getInt32();
                    
                    setButtonLED(i, stateGrid[i]);
                    
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

void KTMHandController::timerCallback() //garbage collection for when a button release callback isn't received
{
    static OSCMessage poll("/inputs/digital/read");
    
    controllerSend.send(poll);
}

void KTMHandController::setButtonLED(const int forButton, const bool state)
{
    if (forButton > -1 && forButton < NUM_KTM_BUTTONS)
    {
        OSCMessage message("/outputs/digital/" + String(forButton+2));
        message.addInt32(state);
        controllerSend.send(message);
    }
}

void KTMHandController::setLEDColour(const int led, const uint8 r, const uint8 g, const uint8 b)
{
    if (led > -1 && led < NUM_RGB_LEDS)
    {
        *ledColours[led] = Colour::fromRGB(r, g, b);
        refreshColourLEDs();
    }
}

void KTMHandController::refreshColourLEDs()
{
    rgbBlob.reset();
    
    int cR, cG, cB;
    for (int i = 0; i < ledColours.size(); i++)
    {
        cR = ledColours[i]->getRed();
        cG = ledColours[i]->getGreen();
        cB = ledColours[i]->getBlue();
        
        rgbBlob.append(&cR, 1);
        rgbBlob.append(&cG, 1);
        rgbBlob.append(&cB, 1);
    }
    
    OSCMessage rgbMessage("/outputs/rgb/1");
    rgbMessage.addBlob(rgbBlob);
    controllerSend.send(rgbMessage);
}


