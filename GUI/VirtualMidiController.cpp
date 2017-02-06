//
//  VirtualMidiController.cpp
//  AI:MO
//
//  Created by Luke Saxton on 17/01/2017.
//
//

#include "VirtualMidiController.hpp"
#include "AIMORouter.hpp"

VirtualMidiController::VirtualMidiController(String _ID)
{
    grid = new GridModule(64);
    addAndMakeVisible(grid);
    
    ID = _ID;
    
    //keyboard = new KeyboardModule();
    //addAndMakeVisible(keyboard);
    
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
    g.fillAll(Colours::black);
    
    

}

void VirtualMidiController::resized()
{
    grid->setBounds(getLocalBounds());
    
    //keyboard->setBounds(0, y, x, y);
}

const String VirtualMidiController::getID()
{
    return ID;
}
