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
#include "VCMModules.hpp"

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
        
        void setButtonState(const int newState)
        {
            buttonState = newState;
            triggerAsyncUpdate();
        }
    private:
        int buttonState = 0;
        String outAddress = "";
    };
    
    class ConfigComponent : public Component{
        ConfigComponent()
        {
            
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
    
    void setMapIn (const String newMapIn) override;
    void setMapOut (const String newMapOut) override;
    void route(const String message) override;
    
    const int getGridSize();
    
    void paint(Graphics& g) override;
    void resized() override;
    void mouseDown(const MouseEvent& event) override;
    
private:
    OwnedArray<GridButton> buttons;
};


#endif /* GridModule_hpp */