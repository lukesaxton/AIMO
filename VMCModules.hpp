//
//  VMCModules.hpp
//  AIMO
//
//  Created by Luke Saxton on 18/01/2017.
//
//

#ifndef VCMModules_hpp
#define VCMModules_hpp

#include "../JuceLibraryCode/JuceHeader.h"


// Base Class for modules in the virtual midi controller
class VMCModule{
public:
    virtual ~VMCModule() {};
    virtual void setMapOut (const String newMapOut) = 0;
    virtual bool routeMidi (const String address, const MidiMessage message) = 0;

private:
};

class VMCMappableControl : public Component,
                           public AsyncUpdater
{
public:
    ~VMCMappableControl(){}
    void setOutAddress (const String newAddress)
    {
        outAddress = newAddress;
    }
    const String getOutAddress()
    {
        return outAddress;

    }
    void paint (Graphics& g) override{}
    void resized () override{}
    void handleAsyncUpdate() override
    {
        repaint();
    }
    
private:
    String outAddress;
};


//standard
class KeyboardModule : public VMCModule,
                       public Component,
                       public MidiKeyboardStateListener
{
public:
    KeyboardModule();
    ~KeyboardModule();
    
    void setMapOut (const String newMapOut) override;
    bool routeMidi (const String address, const MidiMessage message) override;

    
    void paint(Graphics& g) override;
    void resized() override;
    
    // Keyboard State Listener
    void handleNoteOn (MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff (MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity) override;
    
private:
    ScopedPointer<MidiKeyboardComponent> keyComponent;
    MidiKeyboardState keyboardState;
    
    String outMapping = "";
};



class ControlModule : public VMCModule,
                      public Component
{
public:
    ControlModule();
    ~ControlModule();
    
    void setMapOut (const String newMapOut) override;
    bool routeMidi (const String address, const MidiMessage message) override;
    
    void paint(Graphics& g) override;
    void resized() override;
private:
};


#endif /* VMCModules_hpp */
