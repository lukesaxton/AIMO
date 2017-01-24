//
//  RoutableMidiOutput.hpp
//  AIMO
//
//  Created by Luke Saxton on 20/01/2017.
//
//

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
