//
//  MidiButtonModule.hpp
//  AIMO
//
//  Created by Luke Saxton on 27/02/2017.
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

#ifndef MidiButtonModule_hpp
#define MidiButtonModule_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "VMCModules.hpp"

#define DOUBLE_CLICK_TIME 200
#define LONG_PRESS_TIME 1500

class MidiButtonModule : public VMCAsyncControl,
                         public VMCMidiProcessor,
                         public Timer
{
public:
    enum ButtonMode{
        Standard = 1,
        Toggle,
        ToggleCC,
        IncCC,
        List,
        MultiPress,
        LiveLooper,
        FINAL_BUTTONMODE
    };
    
    enum LooperButtonState{
        NotALooper = 0,
        Play,
        Stop,
        Record,
        Overdub,
        Clear,
        FINAL_LOOPERBUTTONSTATE
    };
    
    class ConfigComponent : public Component,
    public Slider::Listener,
    public ComboBox::Listener
    {
    public:
        ConfigComponent(MidiButtonModule* _button)
        {
            
            midiButton = _button;
            setSize(200, 300);
            
            modeBox.addItem("Standard", ButtonMode::Standard);
            modeBox.addItem("Toggle", ButtonMode::Toggle);
            modeBox.addItem("ToggleCC", ButtonMode::ToggleCC);
            modeBox.addItem("List", ButtonMode::List);
            modeBox.addItem("Live Looper", ButtonMode::LiveLooper);


            modeBox.setSelectedId(midiButton->getButtonMode(), dontSendNotification);
            modeBox.addListener(this);
            addAndMakeVisible(modeBox);
            
            
            
        }
        ~ConfigComponent()
        {
            
        }
        void paint(Graphics& g) override
        {
            //g.fillAll(Colours::darkgrey);
        }
        void resized() override
        {
            modeBox.setBounds(0, 0, getWidth(), 30);
            
        }
        
        void sliderValueChanged (Slider* slider) override
        {

        }
        
        void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override
        {
            if (comboBoxThatHasChanged == &modeBox)
            {
                midiButton->setButtonMode(modeBox.getSelectedId());
            }
        }
        
    private:
        MidiButtonModule* midiButton;
        ComboBox modeBox;
    };
    
    
    MidiButtonModule(VMCModule& parent);
    ~MidiButtonModule();
    void paint(Graphics& g) override;
    void resized() override;
    void setButtonState(const bool newState);
    void setColour(const Colour newColour);
    const bool getButtonState();
    void processMidi (MidiMessage* message) override;
    
    const int getButtonMode();
    bool setButtonMode(const int newMode);
    
    int getLooperButtonMode();
    void clearLooper();
    void looperUndo();
    void setLooperNumber(int newNumber) {looperNumber = newNumber;};
    
    uint32 getTimeSinceLastClick() {
        DBG(String(timeSinceLastClick));
        return timeSinceLastClick;};
    
    void timerCallback() override;
    
private:
    void incCurrentCC(bool inc);
    
    bool buttonState = 0;
    
    int looperButtonState = 0;
    
    bool clearIfLongPress = 0;
    bool offOnRelease = 0;
    int multiPress = 0;
    bool ignoreNextRelease = 0;
    int currentCC = 0;
    int looperNumber = 0;
    
    uint32 timeSinceLastClick = 0;
    
    VMCModule& parentModule;
    
    Colour onColour, offColour;
    String mapOut;
    ButtonMode buttonMode = Standard;
};

#endif /* MidiButtonModule_hpp */
