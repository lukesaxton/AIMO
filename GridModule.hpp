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
        MelodicMinor,
        HarmonicMajor,
        Blues,
        MajorPentatonic,
        MinorPentatonic,
        Chromatic,
        Hungarian,
        Aeolian,
        Dorian,
        Ionian,
        Locrian,
        Lydian,
        Mixolydian,
        Phrygian,
        PhrygianDominant,
        Acoustic,
        Algerian,
        Augmented,
        BebopDominant,
        DoubleHarmonic,
        Enigmatic,
        Gypsy,
        HalfDiminished,
        Hirajoshi,
        MajorBebop,
        MajorLocran,
        NeopolitanMajor,
        NeopolitanMinor,
        Persian,
        Tritone,
        WholeTone,
        
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
    

            if (buttonState == On)
            {
                buttonColour = Colours::whitesmoke;
            }
            else
            {
                buttonColour = Colours::slategrey;

            }
            
            if (buttonHighlight)
            {
                buttonColour = buttonColour.brighter();
            }
            
            g.setColour(buttonColour);
            g.fillRect(padding, padding, getWidth()- 2*padding, getHeight() - 2*padding);
            
            
        }
        void resized() override{}
        
        void setButtonState(const bool newState)
        {
            buttonState = newState;
            triggerAsyncUpdate();
        }
        
        void setButtonHighlight(const bool isHighlighted)
        {
            if (isHighlighted != buttonHighlight)
            {
                buttonHighlight = isHighlighted;
                triggerAsyncUpdate();
            }
        }
        
       
    private:
        bool buttonState = 0;
        bool buttonHighlight = 0;
        Colour buttonColour;
    };
    
    class ConfigComponent : public Component,
                            public Slider::Listener,
                            public ComboBox::Listener
    {
    public:
        ConfigComponent(GridModule* _grid)
        {
            
            grid = _grid;
            setSize(200, 300);
            
            rootNoteSlider.setRange(0, 127, 1);
            rootNoteSlider.setValue(grid->getRootNote(), dontSendNotification);
            rootNoteSlider.addListener(this);
            addAndMakeVisible(&rootNoteSlider);
            
            scaleBox.addItem("Major", Scale::Major + 1);
            scaleBox.addItem("Minor", Scale::NaturalMinor + 1);
            scaleBox.addItem("Harmonic Minor", Scale::HarmonicMinor + 1);
            scaleBox.addItem("Melodic Minor", Scale::MelodicMinor + 1);
            scaleBox.addItem("Blues", Scale::Blues + 1);
            scaleBox.addItem("Major Pentatonic", Scale::MajorPentatonic + 1);
            scaleBox.addItem("Minor Pentatonic", Scale::MinorPentatonic + 1);
            scaleBox.addItem("Chromatic", Scale::Chromatic + 1);
            scaleBox.addItem("Hungarian", Scale::Hungarian + 1);
            scaleBox.addItem("Aeolian", Scale::Aeolian + 1);
            scaleBox.addItem("Dorian", Scale::Dorian + 1);
            scaleBox.addItem("Ionian", Scale::Ionian + 1);
            scaleBox.addItem("Locrian", Scale::Locrian + 1);
            scaleBox.addItem("Lydian", Scale::Lydian + 1);
            scaleBox.addItem("Mixolydian", Scale::Mixolydian + 1);
            scaleBox.addItem("Phrygian", Scale::Phrygian + 1);
            scaleBox.addItem("Phrygian Dominant", Scale::PhrygianDominant + 1);
            scaleBox.addItem("Acoustic", Scale::Acoustic + 1);
            scaleBox.addItem("Algerian", Scale::Augmented + 1);
            scaleBox.addItem("Bebop Dominant", Scale::BebopDominant + 1);
            scaleBox.addItem("Double Harmonic", Scale::DoubleHarmonic + 1);
            scaleBox.addItem("Enigmatic", Scale::Enigmatic + 1);
            scaleBox.addItem("Gypsy", Scale::Gypsy + 1);
            scaleBox.addItem("Half Diminished", Scale::HalfDiminished + 1);
            scaleBox.addItem("Hirajoshi", Scale::Hirajoshi + 1);
            scaleBox.addItem("Major Bebop", Scale::MajorBebop + 1);
            scaleBox.addItem("Major Locran", Scale::MajorLocran + 1);
            scaleBox.addItem("Neopolitan Major", Scale::NeopolitanMajor + 1);
            scaleBox.addItem("Neopolitan Minor", Scale::NeopolitanMinor + 1);
            scaleBox.addItem("Double Harmonic", Scale::DoubleHarmonic + 1);
            scaleBox.addItem("Persian", Scale::Persian + 1);
            scaleBox.addItem("Tritone", Scale::Tritone + 1);
            scaleBox.addItem("Whole Tone", Scale::WholeTone + 1);
            
            scaleBox.setSelectedId(grid->getScale()+1, dontSendNotification);
            scaleBox.addListener(this);
            addAndMakeVisible(&scaleBox);
        }
        ~ConfigComponent()
        {
            
        }
        void paint(Graphics& g) override
        {
            //g.fillAll(Colours::darkgrey);
        }
        void resized() override
        {
            rootNoteSlider.setBounds(0, 0, getWidth(), 30);
            scaleBox.setBounds(rootNoteSlider.getBounds().translated(0, 30));
        }
        
        void sliderValueChanged (Slider* slider) override
        {
            grid->setRootNote(slider->getValue());
        }
        
        void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override
        {
            grid->setScale(comboBoxThatHasChanged->getSelectedId()-1);
        }
        
    private:
        GridModule* grid;
        Slider rootNoteSlider;
        ComboBox scaleBox;
    };
    
    GridModule(const int gridSize);
    ~GridModule();
    
    void setMapOut (const String newMapOut) override;
    bool routeMidi (const String address, const MidiMessage message) override;
    
    const int getGridSize();
    
    void paint(Graphics& g) override;
    void resized() override;
    void mouseDown(const MouseEvent &event) override;
    void mouseUp(const MouseEvent &event) override;

    
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
