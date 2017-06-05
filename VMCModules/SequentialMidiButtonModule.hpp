//
//  SequentialMidiButtonModule.hpp
//  AIMO
//
//  Created by Luke Saxton on 03/03/2017.
//
//

#ifndef SequentialMidiButtonModule_hpp
#define SequentialMidiButtonModule_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "GridModule.hpp"

class SequentialMidiButtonModule : public VMCAsyncControl,
                                   public VMCMidiProcessor
{
public:
    
    enum ButtonFunctions{
        UpSeq = 0,
        DownSeq
    };
    
    class ConfigComponent : public Component,
    public Slider::Listener,
    public ComboBox::Listener
    {
    public:
        ConfigComponent(SequentialMidiButtonModule* _button)
        {
            
//            midiButton = _button;
//            setSize(200, 300);
//            
//            modeBox.addItem("Standard", ButtonMode::Standard);
//            modeBox.addItem("Toggle", ButtonMode::Toggle);
//            modeBox.addItem("ToggleCC", ButtonMode::ToggleCC);
//            modeBox.addItem("List", ButtonMode::List);
//            
//            modeBox.setSelectedId(midiButton->getButtonMode(), dontSendNotification);
//            modeBox.addListener(this);
//            addAndMakeVisible(modeBox);
            
            
            
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
        SequentialMidiButtonModule* midiButton;
        ComboBox modeBox;
    };
    
    
    SequentialMidiButtonModule(const int numButtons);
    ~SequentialMidiButtonModule();
    void paint(Graphics& g) override;
    void resized() override;
    void setButtonState(const bool newState);
    void setColour(const Colour newColour);
    const int getSequencePosition();
    void processMidi (MidiMessage* message) override;
    
    const int getButtonMode();
    bool setButtonMode(const int newMode);
    
    void setSequencePosition(const int newPosition);
private:
    int sequencePosition = 0;
    Colour onColour, offColour;
    String mapOut;
    
    ScopedPointer<GridModule> grid;
};

#endif /* SequentialMidiButtonModule_hpp */
