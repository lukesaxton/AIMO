//
//  VirtualMidiController.hpp
//  AI:MO
//
//  Created by Luke Saxton on 17/01/2017.
//
//

#ifndef VirtualMidiController_hpp
#define VirtualMidiController_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "VCMModules.hpp"

//Main functionality class of app, takes various input in the form of addressed midi/OSC data

class VirtualMidiController : public Component
{
public:
    
    VirtualMidiController();
    ~VirtualMidiController();
    
    void paint(Graphics& g) override;
    void resized() override;
private:
    ScopedPointer<GridModule> grid = nullptr;
};


#endif /* VirtualMidiController_hpp */
