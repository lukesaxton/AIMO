//
//  VMCModules.cpp
//  AIMO
//
//  Created by Luke Saxton on 18/01/2017.
//
//

#include "VMCModules.hpp"
#include "AIMORouter.hpp"

KeyboardModule::KeyboardModule()
{
    keyComponent = new MidiKeyboardComponent(keyboardState, MidiKeyboardComponent::horizontalKeyboard);
    addAndMakeVisible(keyComponent);
    
}

KeyboardModule::~KeyboardModule()
{
    
}

void KeyboardModule::setMapIn (const String newMapIn)
{
    
}

void KeyboardModule::setMapOut (const String newMapOut)
{
    
}

void KeyboardModule::route(const String message)
{
    
}

void KeyboardModule::paint(Graphics& g)
{
    g.fillAll(Colours::chocolate);
}

void KeyboardModule::resized()
{
    int x = getWidth();
    int y = getHeight();
    
    keyComponent->setBounds(0,y/2.0,x, y/2.0);
}


void KeyboardModule::handleNoteOn (MidiKeyboardState *source, int midiChannel,
                                   int midiNoteNumber, float velocity)
{
    AIMORouter::Instance()->route(outMapping + " " + );
}
void KeyboardModule::handleNoteOff (MidiKeyboardState *source, int midiChannel,
                                    int midiNoteNumber, float velocity)
{
    
}