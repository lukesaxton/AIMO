//
//  VirtualMidiController.cpp
//  AI:MO
//
//  Created by Luke Saxton on 17/01/2017.
//
//

#include "VirtualMidiController.hpp"
#include "AIMORouter.hpp"

VirtualMidiController::VirtualMidiController(String ID)
{
    grid = new GridModule(64);
    addAndMakeVisible(grid);
    
    keyboard = new KeyboardModule();
    addAndMakeVisible(keyboard);
    
    AIMORouter::Instance()->addDestination(this);
}

VirtualMidiController::~VirtualMidiController()
{
    
}

bool VirtualMidiController::routeMidi (const String address, const MidiMessage message)

{
    if (address.contains("grid"))
    {
        grid->routeMidi(address.fromFirstOccurrenceOf("grid", false, true), message);
        return true;
    }
    
    return false;
}


void VirtualMidiController::paint(Graphics& g)
{
    g.fillAll(Colours::darkslategrey);
}

void VirtualMidiController::resized()
{
    float x = getWidth();
    float y = getHeight()/2.0;
    
    if (grid)
    {
        if (x<=y)
        {
            grid->setBounds(0,0, x-10, x-10);
        }
        else
        {
            grid->setBounds(0,0, y-10, y-10);

        }
        
        grid->setCentrePosition(x/2.0, y/2.0);
    }
    
    keyboard->setBounds(0, y, x, y);
}

const String VirtualMidiController::getID()
{
    return ID;
}
