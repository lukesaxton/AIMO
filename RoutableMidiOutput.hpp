//
//  RoutableMidiOutput.hpp
//  AIMO
//
//  Created by Luke Saxton on 20/01/2017.
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

#ifndef RoutableMidiOutput_hpp
#define RoutableMidiOutput_hpp

#include "../JuceLibraryCode/JuceHeader.h"

class RoutableMidiOutput
{
public:
    RoutableMidiOutput();
    ~RoutableMidiOutput();
    
    bool connectToDevice(const String deviceName);
    bool createNewDevice(const String deviceName);

    
    bool routeMidi (const String address, const MidiMessage message);
    
    const String getDeviceName();
    
private:
    ScopedPointer<MidiOutput> output;
};

#endif /* RoutableMidiOutput_hpp */
