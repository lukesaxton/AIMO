//
//  MidiButtonModule.cpp
//  AIMO
//
//  Created by Luke Saxton on 27/02/2017.
//
//
//  AI:MO is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License, version 3,
//  as published by the Free Software Foundation.
//
//  AI:MO is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "MidiButtonModule.hpp"


MidiButtonModule::MidiButtonModule()
{
    onColour = Colours::snow;
    offColour = Colours::grey;
}

MidiButtonModule::~MidiButtonModule()
{
    
}

void MidiButtonModule::paint(Graphics& g)
{
    float padding = getWidth()*0.0375;
    g.setColour(Colours::darkgrey);
    g.drawRect(padding, padding, getWidth()-1.5*padding, getHeight() -1.5*padding);
    
    
    if (buttonState == 1)
    {
        g.setColour(onColour);
    }
    else
    {
        g.setColour(offColour);
    }
    
    g.fillRect(padding, padding, getWidth()- 2*padding, getHeight() - 2*padding);
    
    
}
void MidiButtonModule::resized()
{
    
}

void MidiButtonModule::setButtonState(const bool newState)
{
    buttonState = newState;
    triggerAsyncUpdate();
}

void MidiButtonModule::setColour(const Colour newColour)
{
    onColour = newColour;
    offColour = newColour.darker().darker();
    triggerAsyncUpdate();
}

const bool MidiButtonModule::getButtonState()
{
    return buttonState;
}


void MidiButtonModule::processMidi (MidiMessage* message)
{
    if (message)
    {
        if (buttonMode == Standard)
        {
            buttonState = message->isNoteOn();
            triggerAsyncUpdate();
        }
        else if (buttonMode == Toggle)
        {
            if (message->isNoteOnOrOff())
            {
                if (message->isNoteOn())
                {
                    buttonState = !buttonState;
                    if (!buttonState)
                    {
                        message->setVelocity(0);
                    }
                    triggerAsyncUpdate();
                    
                }
            }
        }
        else if (buttonMode == ToggleCC)
        {
            if (message->isNoteOnOrOff())
            {
                if (offOnRelease)
                {
                    if (message->isNoteOff())
                    {
                        *message = MidiMessage::controllerEvent(message->getChannel(), message->getNoteNumber(), 0);
                        offOnRelease = false;
                        buttonState = false;
                    }
                }
                else if (message->isNoteOn())
                {
                    buttonState = !buttonState;
                    if (buttonState)
                    {
                        *message = MidiMessage::controllerEvent(message->getChannel(), message->getNoteNumber(), 127);
                    }
                    else
                    {
                        buttonState = true;
                        offOnRelease = true;
                    }
                }
                
                triggerAsyncUpdate();
            }
        }
//        else if (buttonMode == MultiPress)
//        {
//            if (message->isNoteOnOrOff())
//            {
//                if (message->isNoteOn())
//                {
//                    multiPress++;
//                }
//                else
//                {
//                    multiPress--;
//                    if (multi) {
//                        <#statements#>
//                    }
//                }
//            }
//        }
        
        else if (buttonMode == List)
        {
            //Not implemented yet
            jassertfalse;
        }
    }
}

const int MidiButtonModule::getButtonMode()
{
    return buttonMode;
}

bool MidiButtonModule::setButtonMode(const int newMode)
{
    if (newMode > 0 && newMode < FINAL_BUTTONMODE)
    {
        buttonMode = ButtonMode(newMode);
        return true;
    }
    return false;
}
