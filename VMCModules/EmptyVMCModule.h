/*
  ==============================================================================

    EmptyVMCModule.h
    Created: 5 Jun 2017 11:47:25pm
    Author:  Luke Saxton

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "VMCModules.hpp"
#include "AIMORouter.hpp"

class EmptyVMCModule : public VMCModule,
                       public Component,
                       public Timer
{
public:
    EmptyVMCModule();
    ~EmptyVMCModule();
    
    // VMCModule Callbacks
    bool routeMidi (const String address, const MidiMessage message) override;
    void sendToAddress(const MidiMessage message) override;
    
    // Component Callbacks
    void paint(Graphics& g) override;
    void resized() override;
    
    // TImer Callback
    void timerCallback() override;
    
    bool highlightBackground = false;
//    // Config Component
//    class ConfigComponent : public Component,
//    public Slider::Listener,
//    public ComboBox::Listener
//    {
//    public:
//        ConfigComponent(EmptyVMCModule& _module) : module(_module)
//        {
//            setSize(200, 300);
//            
//            addressInLabel.setText("Enter Address To Listen To", <#juce::NotificationType notification#>)
//            
//            addAndMakeVisible(modeBox);
//        }
//        ~ConfigComponent()
//        {
//            
//        }
//        void paint(Graphics& g) override
//        {
//            g.fillAll(Colours::darkgrey);
//        }
//        void resized() override
//        {
//            modeBox.setBounds(0, 0, getWidth(), 30);
//            
//        }
//        
//        void sliderValueChanged (Slider* slider) override
//        {
//            
//        }
//        
//        void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override
//        {
//            if (comboBoxThatHasChanged == &modeBox)
//            {
//                //midiButton->setButtonMode(modeBox.getSelectedId());
//            }
//            
//        }
//        
//    private:
//        EmptyVMCModule &module;
//        Label addressInLabel,addressOutLabel;
//    };
    
};
