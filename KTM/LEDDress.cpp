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
    
    sceneNames.add("~/Beat.xml");
    sceneNames.add("~/Sea.xml");
    sceneNames.add("~/Feet.xml");
    sceneNames.add("~/DoubleBass.xml");
    sceneNames.add("~/Money.xml");
    sceneNames.add("~/Samurai.xml");
    sceneNames.add("~/Tabla.xml");
    
    changeScene(4);
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
    if (sceneNumber < sceneNames.size())
    {
        OSCMessage outMessage(OSCAddressPattern("/scene"));
        outMessage.addString(sceneNames[sceneNumber]);
        dressOutput.send(outMessage);
    }
}
