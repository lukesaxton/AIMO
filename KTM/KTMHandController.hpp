//
//  KTMHandController.hpp
//  AIMO
//
//  Created by Luke Saxton on 08/02/2017.
//
//

#ifndef KTMHandController_hpp
#define KTMHandController_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "AIMOInput.hpp"

#define NUM_KTM_BUTTONS 12



class KTMHandController : public OSCReceiver::Listener<>,
                          public AIMOInput
{
public:
    KTMHandController();
    ~KTMHandController();
    void oscMessageReceived (const OSCMessage& message);
    
    bool connect();
    
private:
    OSCReceiver controllerReceive;
    int stateGrid[NUM_KTM_BUTTONS];
};

#endif /* KTMHandController_hpp */
