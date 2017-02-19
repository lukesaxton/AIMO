//
//  AIMOInputManager.cpp
//  AIMO
//
//  Created by Luke Saxton on 14/02/2017.
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

#include "AIMOInputManager.hpp"


AIMOInputManager::AIMOInputManager()
{
    
    StringArray midiDevices = MidiInput::getDevices();
    
    for (int i = 0; i < midiDevices.size(); i++)
    {
        if (!midiDevices[i].contains("AIMO"))
        {
            midiInputs.add(new RoutableMidiInput(midiDevices[i]));
            addAndMakeVisible(midiInputs.getLast());
        }
       
    }
    
    
}

AIMOInputManager::~AIMOInputManager()
{
    
}

void AIMOInputManager::paint (Graphics& g)
{
    g.fillAll(Colours::darkgrey);
}

void AIMOInputManager::resized()
{
    if (midiInputs.size() > 0)
    {
        midiInputs[0]->setBounds(0, 0, getWidth(), getWidth());
        for (int i = 1; i < midiInputs.size(); i++)
        {
            midiInputs[i]->setBounds(midiInputs[i-1]->getBounds().translated(0, getWidth()+2));
        }
    }
}
