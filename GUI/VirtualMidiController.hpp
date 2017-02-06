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
#include "GridModule.hpp"

//Main functionality class of app, takes various input in the form of addressed midi/OSC data

class VirtualMidiController : public Component
{
public:
    
    VirtualMidiController(String _ID);
    ~VirtualMidiController();
    
    bool routeMidi (const String address, const MidiMessage message);
    
    void paint(Graphics& g) override;
    void resized() override;
    
    const String getID();
private:
    ScopedPointer<GridModule> grid = nullptr;
    ScopedPointer<KeyboardModule> keyboard;
    
    String ID;
};


#endif /* VirtualMidiController_hpp */
