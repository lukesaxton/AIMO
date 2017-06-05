//
//  VMCModules.hpp
//  AIMO
//
//  Created by Luke Saxton on 18/01/2017.
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

#ifndef VCMModules_hpp
#define VCMModules_hpp

#include "../JuceLibraryCode/JuceHeader.h"


class AIMOInternalMessage
{
public:
    AIMOInternalMessage() {}
    ~AIMOInternalMessage() {}
//    void addAddressToChain(String address) {addressChain.add(address);}
//    String popNextAddress()
//    {
//        if (addressChain.size() > 0)
//        {
//            String retVal = addressChain[0];
//            addressChain.remove(0);
//            return retVal;
//        }
//        else
//        {
//            return "";
//        }
//    }
    MidiMessage message;
    StringArray addressChain;
};

// Base Class for modules in the virtual midi controller
class VMCModule{
public:
    virtual ~VMCModule() {};
    void setMapOut (const String newMapOut) {mapOut = newMapOut;};
    virtual bool routeMidi (const String address, const MidiMessage message) = 0;
    virtual void sendToAddress(const MidiMessage message)
    {
        jassertfalse; //bit hacky for now, nee to find a better way for midi processors to bounce asyc midi back up to parent controllers
    }
    String getAddress() {return address;}
    void setAddress(const String newAddress) { address = newAddress; }
private:
    String address;
    String mapOut;
};

class VMC_OSCModule{
public:
    virtual ~VMC_OSCModule() {};
    virtual void setOSCMapOut() = 0;
    virtual bool routeOSC (const OSCMessage) = 0;
    virtual String getOSCAddress() {return address;}
    virtual void setOSCAddress(const String newAddress) { address = newAddress; }
private:
    String address;
};

class VMCAsyncControl : public Component,
                        public AsyncUpdater
{
public:
    ~VMCAsyncControl(){}
    void paint (Graphics& g) override{}
    void resized () override{}
    void handleAsyncUpdate() override
    {
        repaint();
    }
private:
};

class VMCMidiProcessor{
public:
    virtual ~VMCMidiProcessor() {};
    virtual void processMidi (MidiMessage* message) = 0;
private:
};

//standard
class KeyboardModule : public VMCModule,
                       public Component,
                       public MidiKeyboardStateListener
{
public:
    KeyboardModule();
    ~KeyboardModule();
    
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
    
    bool routeMidi (const String address, const MidiMessage message) override;
    
    void paint(Graphics& g) override;
    void resized() override;
private:
};


#endif /* VMCModules_hpp */
