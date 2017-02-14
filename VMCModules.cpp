//
//  VMCModules.cpp
//  AIMO
//
//  Created by Luke Saxton on 18/01/2017.
//
//
//  AI:MO is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License, version 3,
//  as published by the Free Software Foundation.
//
//  AI::MO is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

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


void KeyboardModule::setMapOut (const String newMapOut)
{
    
}

bool KeyboardModule::routeMidi (const String address, const MidiMessage message)
{
    keyboardState.processNextMidiEvent(message);
    return true;
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
    //AIMORouter::Instance()->routeMidi;
}
void KeyboardModule::handleNoteOff (MidiKeyboardState *source, int midiChannel,
                                    int midiNoteNumber, float velocity)
{
    
}
