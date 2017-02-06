//
//  GridModule.hpp
//  AIMO
//
//  Created by Luke Saxton on 19/01/2017.
//
//

#ifndef GridModule_hpp
#define GridModule_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "VMCModules.hpp"


class GridModule : public VMCModule,
                   public Component
{
public:
    enum Scale{
        Major = 0,
        NaturalMinor,
        HarmonicMinor,
        Hungarian,
        FINAL_SCALE
    };
    
    class GridButton : public VMCMappableControl
    {
    public:
        enum ButtonStates{
            Off = 0,
            On,
            HighlightOff,
            HighlightOn
        };
        GridButton(){}
        ~GridButton(){}
        void paint(Graphics& g) override
        {
            float padding = getWidth()*0.0375;
            g.setColour(Colours::darkgrey);
            g.drawRect(padding, padding, getWidth()-1.5*padding, getHeight() -1.5*padding);
            
            switch (buttonState)
            {
                case Off:
                    g.setColour(Colours::slategrey);
                    break;
                case On:
                    g.setColour(Colours::whitesmoke);
                    break;
                case HighlightOn:
                    g.setColour(Colours::orange);
                    break;
                case HighlightOff:
                    g.setColour(Colours::chocolate);
                    break;
            }
            
            if (buttonState)
            {
                g.setColour(Colours::whitesmoke);
            }
            else
            {
            }
            g.fillRect(padding, padding, getWidth()- 2*padding, getHeight() - 2*padding);
            
            
        }
        void resized() override{}
        
        void setButtonState(const int newState)
        {
            buttonState = newState;
            triggerAsyncUpdate();
        }
        
       
    private:
        int buttonState = 0;
    };
    
    class ConfigComponent : public Component
    {
    public:
        ConfigComponent()
        {
            setSize(200, 300);
        }
        ~ConfigComponent()
        {
            
        }
        void paint(Graphics& g) override
        {
            g.fillAll(Colours::hotpink);
        }
        void resized() override
        {
            
        }
    };
    
    GridModule(const int gridSize);
    ~GridModule();
    
    void setMapOut (const String newMapOut) override;
    bool routeMidi (const String address, const MidiMessage message) override;
    
    const int getGridSize();
    
    void paint(Graphics& g) override;
    void resized() override;
    void mouseDown(const MouseEvent &event) override;
    
    void setRootNote(const int newNote);
    const int getRootNote();
    
    void setScale(const int newScale);
    const int getScale();
    
private:
    OwnedArray<GridButton> buttons;
    String mapOut;
    Rectangle<int> buttonBoundingBox;
    int64 timeSinceLastMessage = 0;
    int rootNote = 24;
    int currentScale;
    Array<int> noteMappings;
};


#endif /* GridModule_hpp */
