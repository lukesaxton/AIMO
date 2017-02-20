//
//  KTMHandController.hpp
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

#ifndef KTMHandController_hpp
#define KTMHandController_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "AIMOInput.hpp"

#define NUM_KTM_BUTTONS 12
#define NUM_RGB_LEDS 15


class KTMHandController : public OSCReceiver::Listener<>,
                          public AIMOInput,
                          public Timer
{
public:
    KTMHandController();
    ~KTMHandController();
    void oscMessageReceived (const OSCMessage& message) override;
    
    bool connect();
    
    void timerCallback() override;
    
    void setButtonLED(const int forButton, const bool state);
    
    void setLEDColour(const int led, const uint8 r, const uint8 g, const uint8 b);
    
    void refreshColourLEDs();
private:
    OSCReceiver controllerReceive;
    
    OSCSender controllerSend;
    int stateGrid[NUM_KTM_BUTTONS];
    
    MemoryBlock rgbBlob;
    
    OwnedArray<Colour> ledColours;
};

#endif /* KTMHandController_hpp */
