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


MidiButtonModule::MidiButtonModule(VMCModule& parent) : parentModule(parent)
{
    onColour = Colours::snow;
    offColour = Colours::grey;
    
    buttonMode = Standard;
    
    looperButtonState = Clear;
    
    
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
        static uint32 timeOfLastClick = 0;

        if (message->isNoteOn())
        {
            timeSinceLastClick = Time::getMillisecondCounter() - timeOfLastClick;
            timeOfLastClick = Time::getMillisecondCounter();

        }
        
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
        else if (buttonMode == MultiPress)
        {
            if (message->isNoteOnOrOff())
            {
               
                if (message->isNoteOff())
                {
                    if (!ignoreNextRelease)
                    {
                        buttonState = !buttonState;
                        if (buttonState)
                        {
                            *message = MidiMessage::controllerEvent(message->getChannel(), message->getNoteNumber(), 127);
                        }
                        else
                        {
                            *message = MidiMessage::controllerEvent(message->getChannel(), message->getNoteNumber(), 0);
                        }
                    }
                    else
                    {
                        ignoreNextRelease = false;
                    }
                
                triggerAsyncUpdate();
                }
            }
            else if (message->isController()) // secondary function
            {
                if (message->getControllerValue() == 127)
                {
                    *message = MidiMessage::noteOn(message->getChannel(), message->getControllerNumber()+32, uint8(127));
                    ignoreNextRelease = true;
                }
                else if (message->getControllerValue() == 0)
                {
                    *message = MidiMessage::noteOn(message->getChannel(), message->getControllerNumber()+64, uint8(127));
                    ignoreNextRelease = true;
                }
            }
        }
        else if (buttonMode == IncCC)
        {
            if (message->isNoteOnOrOff())
            {
                
                if (message->isNoteOff())
                {
                    if (!ignoreNextRelease)
                    {
                        buttonState = !buttonState;
                        if (buttonState)
                        {
                            *message = MidiMessage::controllerEvent(message->getChannel(), message->getNoteNumber(), 127);
                        }
                        else
                        {
                            *message = MidiMessage::controllerEvent(message->getChannel(), message->getNoteNumber(), 0);
                        }
                    }
                    else
                    {
                        ignoreNextRelease = false;
                    }
                    
                    triggerAsyncUpdate();
                }
            }
            else if (message->isController()) // secondary function
            {
                if (message->getControllerValue() == 127)
                {
                    incCurrentCC(true);
                    
                    *message = MidiMessage::controllerEvent(message->getChannel(), message->getControllerNumber()+32, uint8(currentCC));
                    ignoreNextRelease = true;
                }
                else if (message->getControllerValue() == 0)
                {
                    incCurrentCC(false);
                    
                    *message = MidiMessage::controllerEvent(message->getChannel(), message->getControllerNumber()+24, uint8(currentCC));
                    ignoreNextRelease = true;
                }
            }

        }
        else if (buttonMode == List)
        {
            //Not implemented yet
            jassertfalse;
        }
        else if (buttonMode == LiveLooper)
        {
            if (message->isNoteOn())
            {
                buttonState = true;
                
                
                startTimer(2000);
                
                if (timeSinceLastClick < DOUBLE_CLICK_TIME) // stop looper
                {
                    if (looperButtonState != Clear)
                    {
                        *message = MidiMessage::noteOn(looperNumber, 123, uint8(127)); // stop
                        looperButtonState = Stop;
                    }
                    else
                    {
                        *message = MidiMessage::noteOn(looperNumber, 124, uint8(127)); // clear
                    }
                }
                else
                {
                    if (looperButtonState == Clear)
                    {
                        *message = MidiMessage::noteOn(looperNumber, 120, uint8(127)); // record
                        looperButtonState = Record;

                    }
                    else if (looperButtonState == Record)
                    {
                        *message = MidiMessage::noteOn(looperNumber, 121, uint8(127)); // play
                        looperButtonState = Play;
                    }
                    else if (looperButtonState == Play)
                    {
                        *message = MidiMessage::noteOn(looperNumber, 122, uint8(127)); // Overdub
                        looperButtonState = Overdub;
                    }
                    else if (looperButtonState == Overdub)
                    {
                        *message = MidiMessage::noteOn(looperNumber, 121, uint8(127)); // play
                        looperButtonState = Play;
                    }
                    else if (looperButtonState == Stop)
                    {
                        *message = MidiMessage::noteOn(looperNumber, 121, uint8(127)); // play
                        clearIfLongPress = true;
                        looperButtonState = Play;
                    }
                }

            }
            else if (message->isNoteOff())
            {
                buttonState = false;
            }
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
        
        if (newMode == LiveLooper && looperButtonState == NotALooper)
        {
            looperButtonState = Clear;
        }
        else if (newMode != LiveLooper)
        {
            looperButtonState = NotALooper;
        }
        
        return true;
    }
    return false;
}

int MidiButtonModule::getLooperButtonMode()
{
    return looperButtonState;
}


void MidiButtonModule::clearLooper()
{
    looperButtonState = Clear;
    parentModule.sendToAddress(MidiMessage::noteOn(looperNumber, 124, uint8(127))); // clear
}

void MidiButtonModule::looperUndo()
{
    parentModule.sendToAddress(MidiMessage::noteOn(looperNumber, 125, uint8(127))); // undo
}

void MidiButtonModule::looperStop()
{
    parentModule.sendToAddress(MidiMessage::noteOn(looperNumber, 123, uint8(127)));
    looperButtonState = Stop;
}

void MidiButtonModule::looperStart()
{
    parentModule.sendToAddress(MidiMessage::noteOn(looperNumber, 121, uint8(127)));
    looperButtonState = Play;
}

void MidiButtonModule::timerCallback()
{
    if (buttonState) // button is still down after 2 seconds
    {
        if (clearIfLongPress)
        {
            looperButtonState = Clear;
            parentModule.sendToAddress(MidiMessage::noteOn(looperNumber, 124, uint8(127))); // clear
        }
        else
        {
            parentModule.sendToAddress(MidiMessage::noteOn(looperNumber, 125, uint8(127))); // undo
        }
    }
    clearIfLongPress = false;
    stopTimer();
}


void MidiButtonModule::incCurrentCC(bool inc)
{
    if (inc)
    {
        ccInc = jlimit(0, 4, ccInc+1);
    }
    else
    {
        ccInc = jlimit(0, 4, ccInc-1);
    }
    
    currentCC = incModeValues[ccInc];
}

