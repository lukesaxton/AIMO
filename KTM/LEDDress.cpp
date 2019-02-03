/*
  ==============================================================================

    LEDDress.cpp
    Created: 1 Aug 2017 6:15:45pm
    Author:  Luke Saxton

  ==============================================================================
*/

#include "LEDDress.h"


LEDDress::LEDDress()
{
    dressOutput.connect("192.168.0.1", 5567);
    //changeScene(4);
    
    
}
LEDDress::~LEDDress()
{
    
}

void LEDDress::paint (Graphics& g)
{
    
}

void LEDDress::resized()
{
    
}

void LEDDress::changeScene(int sceneNumber)
{
    if (sceneNumber < sceneCommands.size())
    {
        OSCMessage outMessage(OSCAddressPattern("/scene"));
        outMessage.addString(sceneCommands[sceneNumber]);
        dressOutput.send(outMessage);
    }
}

void LEDDress::setSceneCommand(int sceneNumber, String command)
{
    sceneCommands.set(sceneNumber, command);
}
