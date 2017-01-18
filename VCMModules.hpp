//
//  VCMModules.hpp
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
    virtual void setMapIn (const String newMapIn) = 0;
    virtual void setMapOut (const String newMapOut) = 0;
    virtual void addressedMidiCallback(const String address, MidiMessage message) = 0;
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
                       public Component
{
public:
    KeyboardModule();
    ~KeyboardModule();
    
    void setMapIn (const String newMapIn) override;
    void setMapOut (const String newMapOut) override;
    void addressedMidiCallback(const String address, MidiMessage message) override;
    
    void paint(Graphics& g) override;
    void resized() override;
private:
};

class GridModule : public VMCModule,
                   public Component
{
public:
    class GridButton : public VMCMappableControl
    {
    public:
        GridButton(){}
        ~GridButton(){}
        void paint(Graphics& g) override
        {
            g.setColour(Colours::darkgrey);
            g.drawRect(3, 3, getWidth()-4, getHeight() -4);
            if (buttonState)
            {
                g.setColour(Colours::whitesmoke);
            }
            else
            {
                g.setColour(Colours::slategrey);
            }
            g.fillRect(3, 3, getWidth()-6, getHeight() -6);
            
            
        }
        void resized() override{}
        
        void setButtonState(const bool newState)
        {
            buttonState = newState;
            triggerAsyncUpdate();
        }
        

        
    private:
        bool buttonState = false;
        String outAddress = "";
    };
    
    GridModule(const int gridSize);
    ~GridModule();
    
    void setMapIn (const String newMapIn) override;
    void setMapOut (const String newMapOut) override;
    void addressedMidiCallback(const String address, MidiMessage message) override;
    
    const int getGridSize();
    
    void paint(Graphics& g) override;
    void resized() override;
private:
    OwnedArray<GridButton> buttons;
};


class ControlModule : public VMCModule,
                      public Component
{
public:
    ControlModule();
    ~ControlModule();
    
    void setMapIn (const String newMapIn) override;
    void setMapOut (const String newMapOut) override;
    void addressedMidiCallback(const String address, MidiMessage message) override;
    
    void paint(Graphics& g) override;
    void resized() override;
private:
};


#endif /* VCMModules_hpp */
