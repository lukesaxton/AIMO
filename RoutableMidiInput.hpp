//
//  RoutableMidiInput.hpp
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

#ifndef RoutableMidiInput_hpp
#define RoutableMidiInput_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "AIMOInput.hpp"

class RoutableMidiInput : public AIMOInput,
                          public MidiInputCallback,
                          public Component,
                          public Timer
{
public:
    enum MidiInputType{
        Keyboard = 0,
        Grid,
    };
    
    class ConfigComponent : public Component
    {
    public:
        ConfigComponent(RoutableMidiInput* parent)
        {
            midiInput = parent;
            
            deviceName.setColour(Label::textColourId, Colours::whitesmoke);
            addAndMakeVisible(deviceName);
            update();
            setSize(200, 100);
        }
        ~ConfigComponent()
        {
            
        }
        void paint(Graphics& g) override
        {
            
        }
        void resized() override
        {
            deviceName.setBounds(0, 0, getWidth(), 30);
        }
        void update()
        {
            deviceName.setText(midiInput->getDeviceToConnect(), dontSendNotification);
        }
    private:
        RoutableMidiInput* midiInput;
        Label deviceName;
    };
    
    RoutableMidiInput(const String deviceToConnectTo);
    ~RoutableMidiInput();
    
    void refreshDeviceList();
    
    void handleIncomingMidiMessage (MidiInput* source,
                                    const MidiMessage& message) override;
    
    void paint(Graphics& g) override;
    void resized() override;
    void mouseDown(const MouseEvent& event) override;
    
    
    void timerCallback() override;
    
    const String getDeviceToConnect();

private:
    
    
    std::unique_ptr<MidiInput> input = nullptr;
    StringArray deviceList;
    String deviceToConnect;
    bool connected = false;
};

#endif /* RoutableMidiInput_hpp */
