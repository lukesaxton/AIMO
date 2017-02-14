//
//  VirtualMidiController.hpp
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

#ifndef VirtualMidiController_hpp
#define VirtualMidiController_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "GridModule.hpp"

//Main functionality class of app, takes various input in the form of addressed midi/OSC data

class VirtualMidiController : public Component
{
public:
    
    VirtualMidiController(String _ID);
    ~VirtualMidiController();
    
    bool routeMidi (const String address, const MidiMessage message);
    bool mapMidi (const String address, const String source, const MidiMessage message);
    
    void paint(Graphics& g) override;
    void resized() override;
    
    const String getID();
private:
    ScopedPointer<GridModule> grid = nullptr;
    ScopedPointer<KeyboardModule> keyboard;
    
    String ID;
};


#endif /* VirtualMidiController_hpp */
