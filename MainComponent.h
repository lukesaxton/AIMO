/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "VirtualMidiController.hpp"
#include "KTMHandController.hpp"
#include "RoutableMidiInput.hpp"
#include "AIMOInputManager.hpp"
#include "LoggerComponent.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
//==============================================================================
/*
 This component lives inside our window, and this is where you should put all
 your controls and content.
 */
class MainContentComponent   : public AudioAppComponent
{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();
    
    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    
    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;
    
    
private:
    //==============================================================================
    LoggerComponent logger;
    
    VirtualMidiController controllerOne;
    KTMHandController kTMController;
    
    AIMOInputManager inputManager;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};

// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }
