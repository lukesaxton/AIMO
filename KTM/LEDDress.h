/*
  ==============================================================================

    LEDDress.h
    Created: 1 Aug 2017 6:16:16pm
    Author:  Luke Saxton

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"

#define LED_DRESS_PORT 5567

#pragma once

class LEDDress : public Component
{
public:
    LEDDress();
    ~LEDDress();
    void paint (Graphics& g) override;
    void resized() override;
    void changeScene(int sceneNumber);
    void setSceneCommand(int sceneNumber, String command);
private:
    OSCSender dressOutput;
    StringArray sceneCommands;
    
    File preferencesFile;
};
