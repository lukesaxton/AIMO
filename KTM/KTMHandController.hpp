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
                          public Timer,
                          public Component
{
public:
    class ColouredBox : public Component
    {
    public:
        ColouredBox(){ boxColour = Colours::black; }
        ~ColouredBox(){}
        void paint(Graphics& g) override
        {
            g.setColour(boxColour);
            g.fillRect(float(getWidth()*0.05), getHeight()*0.05, getWidth()*0.9, getHeight()*0.9);
        }
        void resized() override
        {
            
        }
        void setColour(const Colour newColour)
        {
            boxColour = newColour;
            repaint();
        }
    private:
        Colour boxColour;
    };
    
    
    KTMHandController();
    ~KTMHandController();
    void oscMessageReceived (const OSCMessage& message) override;
    
    bool connect();
    
    void timerCallback() override;
    
    void setButtonLED(const int forButton, const bool state);
    
    void setLEDColour(const int led, const uint8 r, const uint8 g, const uint8 b);
    
    void refreshColourLEDs();
    
    void setSceneLEDs(const Colour newColour);
    
    void resized() override;
    void paint(Graphics& g) override;
    void mouseDown (const MouseEvent& event) override;
    
private:
    OSCReceiver controllerReceive;
    
    OSCSender controllerSend;
    int stateGrid[NUM_KTM_BUTTONS];
    
    MemoryBlock rgbBlob;
    
    OwnedArray<Colour> ledColours;
    OwnedArray<ColouredBox> ledDisplayBoxes;
    OwnedArray<ColouredBox> buttonDisplayBoxes;

    
    Rectangle<int> mainBox;
    Rectangle<int> buttonRows[4];
    
    
    
};

#endif /* KTMHandController_hpp */
