/*
 ==============================================================================
 
 LoggerComponent.h
 Created: 7 Sep 2018 9:02:06pm
 Author:  Luke Saxton
 
 ==============================================================================
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"


class LoggerComponent : public Component,
public Logger,
public Button::Listener,
public AsyncUpdater
{
public:
    LoggerComponent() : log(debugLog, nullptr)
    {
        addAndMakeVisible(log);
        
        bypassButton.setClickingTogglesState(true);
        bypassButton.setToggleState(true, dontSendNotification);
        bypassButton.setButtonText("Bypass");
        addAndMakeVisible(bypassButton);
    }
    
    ~LoggerComponent()
    {
        Logger::setCurrentLogger(nullptr);
    }
    
    void paint(Graphics& g) override
    {
        g.fillAll(Colours::hotpink);
    }
    
    void resized() override
    {
        int w = getWidth();
        int h = getHeight();
        
        log.setBounds(getLocalBounds());
        bypassButton.setBounds(w-100, h-30, 100, 30);
    }
    
    void logMessage (const String& message) override
    {
        if(bypassButton.getToggleState())
        {
            asyncString += message + "\n";
            triggerAsyncUpdate();
        }
        DBG(message);
    }
    
    void buttonClicked (Button* button) override
    {
        if (button == &bypassButton)
        {
            
        }
    }
    
    void handleAsyncUpdate() override
    {
        log.insertTextAtCaret(asyncString);
        asyncString.clear();
    }
private:
    CodeDocument debugLog;
    CodeEditorComponent log;
    TextButton bypassButton;
    String asyncString;
    bool bypass = false;
};
