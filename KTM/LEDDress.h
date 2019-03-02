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
    void setDressIP(String dressIP);
    void setSceneCommand(int sceneNumber, String address, int arg1, int arg2);
private:
    typedef struct
    {
        String address;
        int sceneMaj;
        int sceneMin;
    } SceneCommand;
    
    OSCSender dressOutput;
    Array<SceneCommand> sceneCommands;
    
    bool connected;
    File preferencesFile;
};
