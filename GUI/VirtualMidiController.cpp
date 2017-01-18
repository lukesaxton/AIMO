//
//  VirtualMidiController.cpp
//  AI:MO
//
//  Created by Luke Saxton on 17/01/2017.
//
//

#include "VirtualMidiController.hpp"

VirtualMidiController::VirtualMidiController()
{
    grid = new GridModule(64);
    addAndMakeVisible(grid);
}

VirtualMidiController::~VirtualMidiController()
{
    
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
