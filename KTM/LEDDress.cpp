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
    //dressOutput.connect("192.168.0.1", 5567);
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
    if (connected)
    {
        if (sceneNumber < sceneCommands.size())
        {
            OSCMessage outMessage(sceneCommands[sceneNumber].address);
            outMessage.addInt32(sceneCommands[sceneNumber].sceneMaj);
            outMessage.addInt32(sceneCommands[sceneNumber].sceneMin);
            dressOutput.send(outMessage);
        }
    }
   
}

void LEDDress::setDressIP(String dressIP)
{
    connected = dressOutput.connect(dressIP, 5567);
    if (connected)
    {
        Logger::writeToLog("LED dress configured @ " + dressIP + ":" + String(LED_DRESS_PORT));
    }
    else
    {
        Logger::writeToLog("ERROR - LED dress config failed @ " + dressIP + ":" + String(LED_DRESS_PORT));
    }
}

void LEDDress::setSceneCommand(int sceneNumber, String address, int arg1, int arg2)
{
    SceneCommand dummyCommand;
    dummyCommand.address = address;
    dummyCommand.sceneMaj = arg1;
    dummyCommand.sceneMin = arg2;

    sceneCommands.set(sceneNumber, dummyCommand);
}
