//
//  SequentialMidiButtonModule.cpp
//  AIMO
//
//  Created by Luke Saxton on 03/03/2017.
//
//

#include "SequentialMidiButtonModule.hpp"


SequentialMidiButtonModule::SequentialMidiButtonModule(const int numButtons)
{
    grid = new GridModule(numButtons);
    grid->setInputType(GridModule::InputType::AllKeys);
    grid->setScale(GridModule::Scale::Chromatic);
    addAndMakeVisible(grid);
}

SequentialMidiButtonModule::~SequentialMidiButtonModule()
{
    
}

void SequentialMidiButtonModule::paint(Graphics& g)
{
    
}

void SequentialMidiButtonModule::resized()
{
    grid->setBounds(getLocalBounds());
}

void SequentialMidiButtonModule::setButtonState(const bool newState)
{
    
}

void SequentialMidiButtonModule::setColour(const Colour newColour)
{
    
}
const int SequentialMidiButtonModule::getSequencePosition()
{
    return sequencePosition;
}

void SequentialMidiButtonModule::processMidi (MidiMessage* message)
{
    if (message->isController())
    {
        if (message->getControllerValue() > 64)
        {
            setSequencePosition(sequencePosition+1);
        }
        else
        {
            setSequencePosition(sequencePosition-1);

        }
    }
}

const int SequentialMidiButtonModule::getButtonMode()
{
    return buttonMode;
}

bool SequentialMidiButtonModule::setButtonMode(const int newMode)
{
    buttonMode = newMode;
    return true;
}

void SequentialMidiButtonModule::setSequencePosition(const int newPosition)
{
    if (newPosition > -1 && newPosition < grid->getGridSize())
    {
        grid->routeMidi("key", MidiMessage::noteOn(16, sequencePosition, uint8(110)));
        grid->routeMidi("key", MidiMessage::noteOff(16, sequencePosition));

    }
    else if (newPosition < 0)
    {
        sequencePosition = 0;
    }
    else if (newPosition >= grid->getGridSize())
    {
        sequencePosition = grid->getGridSize()-1;
    }
}



