//
//  AIMORouter.hpp
//  AIMO
//
//  Created by Luke Saxton on 19/01/2017.
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

#ifndef AIMORouter_hpp
#define AIMORouter_hpp

#include "../JuceLibraryCode/JuceHeader.h"

class VirtualMidiController;
class RoutableMidiOutput;
class VMCModule;

class AIMORouter : public DeletedAtShutdown
{
public:
    
    static AIMORouter* Instance();
    ~AIMORouter();
    
    //bool route(const String message);
    
    bool routeMidi (const String address, const MidiMessage message);
    
    void addDestination(VMCModule* destination);

    bool mapMidi (const String address, const String source, const MidiMessage message);
    
    
private:
    AIMORouter();
    static AIMORouter* pInstance;
    
    Array<VMCModule*> registeredDestinations;
    OwnedArray<RoutableMidiOutput> midiOutputs;
    
};

#endif /* AIMORouter_hpp */
