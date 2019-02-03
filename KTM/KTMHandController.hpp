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
#include "VMCModules.hpp"
#include "MidiButtonModule.hpp"
#include "SequentialMidiButtonModule.hpp"
#include "LEDDress.h"

#define NUM_KTM_BUTTONS 12
#define NUM_RGB_LEDS 15
#define MAX_SCENES 32
#define KTM_NUM_PAGES 4

/* 
    Class for communicating with the KTM Hand Controller
 
    Addresses/commands:
    MIDI:
    'key' + note on/off message with note number 0-11 
        - simulates a button press
    
    OSC:
    'lights' + OSCMessage with 2 int args : LED Number, ARGB values
        - sets the colour of that LED to RGB Value supplied.
 
 */


class KTMHandController : public OSCReceiver::Listener<>,
                          public AIMOInput,
                          public MultiTimer,
                          public Component,
                          public VMCModule,
                          public VMC_OSCModule,
                          public Button::Listener,
                          public TextEditor::Listener
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
    
    void setMapOut (const String newMapOut) override;
    bool routeMidi (const String address, const MidiMessage message) override;
    void sendToAddress(const MidiMessage message) override;
    
    void setOSCMapOut() override;
    bool routeOSC (const OSCMessage) override;
    
    
    bool connect(String sendAddress);
    
    void timerCallback (int timerID) override;
    void setButtonLED(const int forButton, const bool state);
    
    void setLEDColour(const int led, const uint8 r, const uint8 g, const uint8 b);
    void setLEDColour(const int led, const int ARGB);
    
    void refreshColourLEDs();
    
    void setScene(const int newScene);
    void setSceneLEDs(const Colour newColour);
    void setPage(const int page);
    
    
    void resized() override;
    void paint(Graphics& g) override;
    void mouseDown (const MouseEvent& event) override;
    
    void refreshLooperState(const int forButton);
    
    void buttonClicked(Button* button) override;
    
    void textEditorReturnKeyPressed (TextEditor&) override;
    
    void pollForControllerAtAddress(String controllerAddress);
    
    void loadPreferencesFromFile();
private:
    
    enum timerIDs {
        INPUT_POLL,
        CONNECTION_CHECK
    };
    
    DatagramSocket oscInputSocket;
    OSCReceiver controllerReceive;
    
    OSCSender controllerSend;
    int stateGrid[NUM_KTM_BUTTONS];
    
    LEDDress dress;
    
    MemoryBlock rgbBlob;
    
    OwnedArray<Colour> ledColours[KTM_NUM_PAGES];
    Colour sceneLEDColour;
    
    OwnedArray<ColouredBox> ledDisplayBoxes[KTM_NUM_PAGES];
    OwnedArray<MidiButtonModule> buttonModules[KTM_NUM_PAGES];
    TextButton sendClearButton;
    Slider clearButtonChannel;
    Label sceneNumberLabel;
    TextEditor controllerIpEditor;
    Label controllerIpLabel;
    
    OwnedArray<Colour> sceneColours;
    
    int currentScene = 0;
    int currentPage = 0;
    
    uint32 timeOfLastPing = 0;
    
    bool controllerConnected = false;
    String controllerIpString = "192.168.0.103";
    
    Rectangle<int> mainBox;
    Rectangle<int> buttonRows[4];
    
    File preferencesFile;
    ScopedPointer<XmlElement> prefsXML;
};

#endif /* KTMHandController_hpp */
