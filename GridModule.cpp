//
//  GridModule.cpp
//  AIMO
//
//  Created by Luke Saxton on 19/01/2017.
//
//

#include "GridModule.hpp"
#include "AIMORouter.hpp"



namespace IntervalSpacings
{

    //MODES
    const int Aeolian[] = {0,2,3,4,7,8,10,-1};
    const int Dorian[] = {0,2,3,5,7,9,10,-1};
    const int Ionian[] = {0,2,4,5,7,9,11,-1};
    const int Locrian[] = {0,1,3,5,6,8,10,-1};
    const int Lydian[] = {0,1,4,6,7,9,11,-1};
    const int Phrygian[] = {0,1,3,5,7,8,10,-1};
    
    //SCALES
    const int Acoustic[] = {0,2,4,6,7,9,-1};
    const int NaturalMinor[] = {0,2,3,5,7,8,10,-1};
    const int Algerian[] = {0,2,3,6,7,8,11, -1};
    const int Augmented[] = {0,3,4,7,8,11,-1};
    const int BebopDominant[] = {0,2,4,5,7,9,10,11,-1};
    const int Blues[] = {0,3,5,6,7,10,-1};
    const int Chromatic[] = {0,1,2,3,4,5,6,7,8,9,10,11,-1};
    const int DoubleHarmonic[] = {0,1,4,5,7,8,11,-1};
    const int Enigmatic[] = {0,1,4,6,8,10,11,-1};
    const int Gypsy[] = {0,2,3,6,7,8,10,-1};
    const int HalfDiminished[] = {0,2,3,5,6,8,10,-1};
    const int HarmonicMajor[] = {0,2,4,5,7,8,11,-1};
    const int HarmonicMinor[] = {0,2,3,5,7,8,11,-1};
    const int Hirajoshi[] = {0,2,3,7,8,-1};
    const int HungarianMinor[] = {0,2,3,6,7,8,11,-1};
    const int Major[] = {0,2,4,5,7,9, 11,-1};
    const int MajorBebop[] = {0,2,4,5,7,8,9,11,-1};
    const int MajorLocran[] = {0,2,4,5,6,8,10,-1};
    const int MajorPentatonic[] = {0,2,4,7,9,-1};
    const int MelodicMinor[] = {0,2,3,5,7,8,9,11,-1};
    const int MinorPentatonic[] = {0,3,5,7,10,-1};
    const int Mixolydian[] = {0,2,4,5,7,9,10,-1};
    const int NeopolitanMajor[] = {0,1,3,5,7,9,11,-1};
    const int NeopolitanMinor[] = {0,1,3,5,7,8,11,-1};
    const int Persian[] = {0,1,4,5,6,8,11,-1};
    const int PhrygianDominant[] = {0,1,4,5,7,8,10,-1};
    const int Tritone[] = {0,1,4,6,7,10};
    const int WholeTone[] = {0,2,4,6,8,10,-1};
    
};

//=================================================================================================
//
//      GRID MODULE
//
//=================================================================================================

GridModule::GridModule(const int gridSize)
{
    for (int i = 0; i < gridSize; i++)
    {
        buttons.add(new GridButton());
        buttons.getLast()->addMouseListener(this, true);
        addAndMakeVisible(buttons.getLast());
        
        noteMappings.add(i+36);
    }
    
    setScale(Scale::Major);
    
    mapOut = "/AIMO Out";
    setInterceptsMouseClicks(true, true);
    
    //buttonBoundingBox.setBounds(0, 0, getWidth(), getHeight());
}

GridModule::~GridModule()
{
    
}


void GridModule::setMapOut (const String newMapOut)
{
    mapOut = newMapOut;
}

bool GridModule::routeMidi (const String address, const MidiMessage message)

{
    if (address.contains("key"))
    {
        MidiMessage mappedMessage(message);
        
        
        if (message.getNoteNumber() < buttons.size())
        {
            mappedMessage.setNoteNumber(noteMappings[message.getNoteNumber()]);
            
            if (message.isNoteOn())
            {
                buttons[message.getNoteNumber()]->setButtonState(true);
            }
            else if (message.isNoteOff())
            {
                buttons[message.getNoteNumber()]->setButtonState(false);
            }
        }
        else
        {
            jassert(false);
            //trying to route to a button that doesn't exist
        }
        
        
        //key and note scaling
        
        
        
        
        return AIMORouter::Instance()->routeMidi(mapOut, mappedMessage);
        
    }
    else
    {
        jassertfalse;
        return false;
        
    }
}

const int GridModule::getGridSize()
{
    return buttons.size();
}

void GridModule::paint(Graphics& g)
{
    g.fillAll(Colours::darkslategrey);
    g.setColour(Colours::darkgrey);
    g.fillRect(buttonBoundingBox);
}

void GridModule::resized()
{
    
    float x = getWidth();
    float y = getHeight();
   

    if (x<=y)
    {
        buttonBoundingBox.setBounds(0,0, x-10, x-10);
    }
    else
    {
        buttonBoundingBox.setBounds(0,0, y-10, y-10);
        
    }
    
    buttonBoundingBox.setCentre(x/2.0, y/2.0);

    
    int buttonsPerRow = 0;
    
    switch (buttons.size()){
        case 121:
            buttonsPerRow = 11;
            break;
        case 64:
            buttonsPerRow = 8;
            break;
        case 49:
            buttonsPerRow = 7;
            break;
        case 36:
            buttonsPerRow = 6;
            break;
        case 25:
            buttonsPerRow = 5;
            break;
        case 9:
            buttonsPerRow = 3;
            break;
        case 2:
            buttonsPerRow = 2;
            break;
        case 1:
            buttonsPerRow = 1;
            break;
        default:
            buttonsPerRow = 4;
            break;
    }
    
    if (buttons.size() > 0)
    {
        float buttonWidth = buttonBoundingBox.getWidth() / buttonsPerRow;
        
        buttons[0]->setBounds(buttonBoundingBox.getX(), buttonBoundingBox.getY(), buttonWidth, buttonWidth);
        
        for (int i = 1; i < buttons.size(); i++)
        {
            buttons[i]->setBounds(buttons[i-1]->getBounds().translated(buttonWidth, 0));
            if (buttons[i]->getRight() > buttonBoundingBox.getRight())
            {
                buttons[i]->setTopLeftPosition(buttonBoundingBox.getX(), ((i/buttonsPerRow) * buttonWidth)+buttonBoundingBox.getY());
            }
        }
    }
}

void GridModule::mouseDown(const MouseEvent &event)
{
    
    if (event.mods.isCtrlDown())
    {
        CallOutBox::launchAsynchronously(new ConfigComponent(this), this->getScreenBounds(), nullptr);
    }
    else
    {
        for (int i = 0; i < buttons.size(); i++)
        {
            if (event.eventComponent == buttons[i])
            {
                routeMidi("key", MidiMessage::noteOn(1, i, uint8(110)));
            }
        }
    }

    
}


void GridModule::mouseUp(const MouseEvent &event)
{
    
    for (int i = 0; i < buttons.size(); i++)
    {
        if (event.eventComponent == buttons[i])
        {
            routeMidi("key", MidiMessage::noteOff(1, i));
        }
    }
    
}

void GridModule::setRootNote(const int newNote)
{
    if (newNote > -1 && newNote < 128)
    {
        rootNote = newNote;
        setScale(getScale());
    }
}

const int GridModule::getRootNote()
{
    return rootNote;
}

void GridModule::setScale(const int newScale)
{
    if (newScale >= 0 && newScale < FINAL_SCALE)
    {
        Array<int> foundScale;
        int index = 0;
        
        switch (newScale) {
            case Major:
                while (IntervalSpacings::Major[index] != -1)
                {
                    foundScale.add(IntervalSpacings::Major[index]);
                    index++;
                }
                break;
            case NaturalMinor:
                while (IntervalSpacings::NaturalMinor[index] != -1)
                {
                    foundScale.add(IntervalSpacings::NaturalMinor[index]);
                    index++;
                }
                break;
            case HarmonicMinor:
                while (IntervalSpacings::HarmonicMinor[index] != -1)
                {
                    foundScale.add(IntervalSpacings::HarmonicMinor[index]);
                    index++;
                }
                break;
            case MelodicMinor:
                while (IntervalSpacings::MelodicMinor[index] != -1)
                {
                    foundScale.add(IntervalSpacings::MelodicMinor[index]);
                    index++;
                }
                break;
            case HarmonicMajor:
                while (IntervalSpacings::HarmonicMajor[index] != -1)
                {
                    foundScale.add(IntervalSpacings::HarmonicMajor[index]);
                    index++;
                }
                break;
            case Blues:
                while (IntervalSpacings::Blues[index] != -1)
                {
                    foundScale.add(IntervalSpacings::Blues[index]);
                    index++;
                }
                break;
            case MajorPentatonic:
                while (IntervalSpacings::MajorPentatonic[index] != -1)
                {
                    foundScale.add(IntervalSpacings::MajorPentatonic[index]);
                    index++;
                }
                break;
            case MinorPentatonic:
                while (IntervalSpacings::MinorPentatonic[index] != -1)
                {
                    foundScale.add(IntervalSpacings::MinorPentatonic[index]);
                    index++;
                }
                break;
            case Chromatic:
                while (IntervalSpacings::Chromatic[index] != -1)
                {
                    foundScale.add(IntervalSpacings::Chromatic[index]);
                    index++;
                }
                break;
            case Hungarian:
                while (IntervalSpacings::HungarianMinor[index] != -1)
                {
                    foundScale.add(IntervalSpacings::HungarianMinor[index]);
                    index++;
                }
                break;
            case Aeolian:
                while (IntervalSpacings::Aeolian[index] != -1)
                {
                    foundScale.add(IntervalSpacings::Aeolian[index]);
                    index++;
                }
                break;
            case Dorian:
                while (IntervalSpacings::Dorian[index] != -1)
                {
                    foundScale.add(IntervalSpacings::Dorian[index]);
                    index++;
                }
                break;
            case Ionian:
                while (IntervalSpacings::Ionian[index] != -1)
                {
                    foundScale.add(IntervalSpacings::Ionian[index]);
                    index++;
                }
                break;
            case Locrian:
                while (IntervalSpacings::Locrian[index] != -1)
                {
                    foundScale.add(IntervalSpacings::Locrian[index]);
                    index++;
                }
                break;
            case Lydian:
                while (IntervalSpacings::Lydian[index] != -1)
                {
                    foundScale.add(IntervalSpacings::Lydian[index]);
                    index++;
                }
                break;
            case Mixolydian:
                while (IntervalSpacings::Mixolydian[index] != -1)
                {
                    foundScale.add(IntervalSpacings::Mixolydian[index]);
                    index++;
                }
                break;
            case Phrygian:
                while (IntervalSpacings::Phrygian[index] != -1)
                {
                    foundScale.add(IntervalSpacings::Phrygian[index]);
                    index++;
                }
                break;
            case Acoustic:
                while (IntervalSpacings::Acoustic[index] != -1)
                {
                    foundScale.add(IntervalSpacings::Acoustic[index]);
                    index++;
                }
                break;
            case Algerian:
                while (IntervalSpacings::Algerian[index] != -1)
                {
                    foundScale.add(IntervalSpacings::Algerian[index]);
                    index++;
                }
                break;
            case Augmented:
                while (IntervalSpacings::Augmented[index] != -1)
                {
                    foundScale.add(IntervalSpacings::Augmented[index]);
                    index++;
                }
                break;
            case BebopDominant:
                while (IntervalSpacings::BebopDominant[index] != -1)
                {
                    foundScale.add(IntervalSpacings::BebopDominant[index]);
                    index++;
                }
                break;
            case DoubleHarmonic:
                while (IntervalSpacings::DoubleHarmonic[index] != -1)
                {
                    foundScale.add(IntervalSpacings::DoubleHarmonic[index]);
                    index++;
                }
                break;
            case Enigmatic:
                while (IntervalSpacings::Enigmatic[index] != -1)
                {
                    foundScale.add(IntervalSpacings::Enigmatic[index]);
                    index++;
                }
                break;
            case Gypsy:
                while (IntervalSpacings::Gypsy[index] != -1)
                {
                    foundScale.add(IntervalSpacings::Gypsy[index]);
                    index++;
                }
                break;
            case HalfDiminished:
                while (IntervalSpacings::HalfDiminished[index] != -1)
                {
                    foundScale.add(IntervalSpacings::HalfDiminished[index]);
                    index++;
                }
                break;
            case Hirajoshi:
                while (IntervalSpacings::Hirajoshi[index] != -1)
                {
                    foundScale.add(IntervalSpacings::Hirajoshi[index]);
                    index++;
                }
                break;
            case MajorBebop:
                while (IntervalSpacings::MajorBebop[index] != -1)
                {
                    foundScale.add(IntervalSpacings::MajorBebop[index]);
                    index++;
                }
                break;
            case MajorLocran:
                while (IntervalSpacings::MajorLocran[index] != -1)
                {
                    foundScale.add(IntervalSpacings::MajorLocran[index]);
                    index++;
                }
                break;
            case NeopolitanMajor:
                while (IntervalSpacings::NeopolitanMajor[index] != -1)
                {
                    foundScale.add(IntervalSpacings::NeopolitanMajor[index]);
                    index++;
                }
                break;
            case NeopolitanMinor:
                while (IntervalSpacings::NeopolitanMinor[index] != -1)
                {
                    foundScale.add(IntervalSpacings::NeopolitanMinor[index]);
                    index++;
                }
                break;
            case Persian:
                while (IntervalSpacings::Persian[index] != -1)
                {
                    foundScale.add(IntervalSpacings::Persian[index]);
                    index++;
                }
                break;
            case PhrygianDominant:
                while (IntervalSpacings::PhrygianDominant[index] != -1)
                {
                    foundScale.add(IntervalSpacings::PhrygianDominant[index]);
                    index++;
                }
                break;
            case Tritone:
                while (IntervalSpacings::Tritone[index] != -1)
                {
                    foundScale.add(IntervalSpacings::Tritone[index]);
                    index++;
                }
                break;
            case WholeTone:
                while (IntervalSpacings::WholeTone[index] != -1)
                {
                    foundScale.add(IntervalSpacings::WholeTone[index]);
                    index++;
                }
                break;
            default:
                jassertfalse;
                break;
        }
        
        if (foundScale.size() != 0)
        {
            currentScale = newScale;
            int currentOctave = 0;
            int positionInScale = 0;
            for (int i = 0; i < noteMappings.size(); i++)
            {
                int newNote = rootNote + 12*currentOctave + foundScale.getUnchecked(positionInScale);
                noteMappings.set(i, newNote);
                
                positionInScale++;
                
                if (i+1 < buttons.size())
                {
                    buttons[i+1]->setButtonHighlight(false);
                }
                
                if (positionInScale >= foundScale.size())
                {
                    positionInScale = 0;
                    currentOctave++;
                    if (i+1 < buttons.size())
                    {
                        buttons[i+1]->setButtonHighlight(true);
                    }
                }
            }
        }
    }
}

const int GridModule::getScale()
{
    return currentScale;
}





