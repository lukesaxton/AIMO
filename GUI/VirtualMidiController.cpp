//
//  VirtualMidiController.cpp
//  AI:MO
//
//  Created by Luke Saxton on 17/01/2017.
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
