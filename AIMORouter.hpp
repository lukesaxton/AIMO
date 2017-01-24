//
//  AIMORouter.hpp
//  AIMO
//
//  Created by Luke Saxton on 19/01/2017.
//
//

#ifndef AIMORouter_hpp
#define AIMORouter_hpp

#include "../JuceLibraryCode/JuceHeader.h"


class VirtualMidiController;
class RoutableMidiOutput;

class AIMORouter : public DeletedAtShutdown
{
public:
    
    static AIMORouter* Instance();
    ~AIMORouter();
    
    //bool route(const String message);
    
    bool routeMidi (const String address, const MidiMessage message);
    
    void addDestination(VirtualMidiController* destination);

    
private:
    AIMORouter();
    static AIMORouter* pInstance;
    
    Array<VirtualMidiController*> registeredDestinations;
    OwnedArray<RoutableMidiOutput> midiOutputs;
    
};

#endif /* AIMORouter_hpp */
