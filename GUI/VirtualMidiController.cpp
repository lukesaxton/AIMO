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
    
    AIMORouter::Instance()->addDestination(this);
}

VirtualMidiController::~VirtualMidiController()
{
    
}

void VirtualMidiController::route(const String message)
{
    if (message.contains("grid"))
    {
        int padNum = message.fromFirstOccurrenceOf(" ", false, true).getIntValue();
        
    }
}


void VirtualMidiController::paint(Graphics& g)
{
    g.fillAll(Colours::darkslategrey);
}

void VirtualMidiController::resized()
{
    float x = getWidth();
    float y = getHeight();
    
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
}

const String VirtualMidiController::getID()
{
    return ID;
}
