/*
  ==============================================================================

    EmptyVMCModule.cpp
    Created: 5 Jun 2017 11:47:25pm
    Author:  Luke Saxton

  ==============================================================================
*/

#include "EmptyVMCModule.h"

EmptyVMCModule::EmptyVMCModule()
{
    
}

EmptyVMCModule::~EmptyVMCModule()
{
    
}

bool EmptyVMCModule::routeMidi (const String address, const MidiMessage message)
{
    if (address == getAddress())
    {
        highlightBackground = true;
        repaint();
        startTimer(500);
    }
}

void EmptyVMCModule::sendToAddress(const MidiMessage message)
{
    
}

void EmptyVMCModule::paint(Graphics& g)
{
    if (highlightBackground)
    {
        g.fillAll(Colours::slategrey.brighter());
    }
    else
    {
        g.fillAll(Colours::slategrey);
    }
    
}

void EmptyVMCModule::resized()
{
    
}

void EmptyVMCModule::timerCallback()
{
    
}
