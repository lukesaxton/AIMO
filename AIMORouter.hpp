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

class AIMORouter : public DeletedAtShutdown
{
public:
    
    static AIMORouter* Instance();
    ~AIMORouter();
    
    bool route(const String message);
    
    void addDestination(VirtualMidiController* destination);

    
private:
    AIMORouter();
    static AIMORouter* pInstance;
    
    Array<VirtualMidiController*> registeredDestinations;
};

#endif /* AIMORouter_hpp */
