//
//  KTMHandController.cpp
//  AIMO
//
//  Created by Luke Saxton on 08/02/2017.
//
//
//  AI:MO is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License, version 3,
//  as published by the Free Software Foundation.
//
//  AI::MO is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "KTMHandController.hpp"
#include "AIMORouter.hpp"


#define KTM_CONTROLLER_RECEIVE 8010
#define KTM_CONTROLLER_SEND 9010


KTMHandController::KTMHandController()
{
    setInterceptsMouseClicks(false, true);
    
    controllerReceive.addListener(this);
    connect();
    
    for (int i = 0; i < NUM_KTM_BUTTONS; i++)
    {
        stateGrid[i] = 0;
        setKeyMapping("/controllerOne/Grid/key", i);
        
        buttonDisplayBoxes.add(new ColouredBox());
        buttonDisplayBoxes.getLast()->setColour(Colours::darkred);
        buttonDisplayBoxes.getLast()->addMouseListener(this, true);
        addAndMakeVisible(buttonDisplayBoxes.getLast());
        
        setButtonLED(i, false);
    }

    
    for (int i = 0; i < NUM_RGB_LEDS; i++)
    {
        if (i/4 < 1)
        {
            ledColours.add(new Colour(Colours::red));
        }
        else if (i/4 > 1)
        {
            ledColours.add(new Colour(Colours::green));

        }
        else
        {
            ledColours.add(new Colour(Colours::yellow));

        }
        ledDisplayBoxes.add(new ColouredBox());
        ledDisplayBoxes.getLast()->addMouseListener(this, true);
        addAndMakeVisible(ledDisplayBoxes.getLast());
        ledDisplayBoxes.getLast()->setColour(*ledColours.getLast());
        
       
    }
    
    setSceneLEDs(Colours::red);
    
    startTimer(50);
    
    setAddress("/KTM/");
    setOSCAddress("/KTMOSC/");
    AIMORouter::Instance()->addDestination(this);
    AIMORouter::Instance()->addOSCDestination(this);

}

KTMHandController::~KTMHandController()
{
    
}

bool KTMHandController::connect()
{
    if (controllerReceive.connect(KTM_CONTROLLER_RECEIVE) && controllerSend.connect("169.254.1.1", KTM_CONTROLLER_SEND))
    {
        DBG("Connected to KTMController ports");
        return true;
    }
    else
    {
        return false;
    }
    
   
}


void KTMHandController::oscMessageReceived (const OSCMessage& message)
{
    //DBG(message.getAddressPattern().toString() + " ARGS: " + String(message.size()));
    
    static String address;
    address = message.getAddressPattern().toString();
    
    
    if (address == "/inputs/digital")
    {
        OSCArgument* curArg = message.begin();;
        for (int i = 0; i < NUM_KTM_BUTTONS; i++)
        {
            if (curArg->isInt32())
            {
                pressButton(i, curArg->getInt32());
            }
            curArg++;
        }
    }
  
}

void KTMHandController::setMapOut (const String newMapOut)
{
    
}


bool KTMHandController::routeMidi (const String address, const MidiMessage message)
{
    if (address == getAddress() + "key")
    {
        if (message.isNoteOnOrOff())
        {
            return pressButton(message.getNoteNumber(), message.isNoteOn());
           
        }
        else
        {
            jassertfalse;
        }
        return false;
        
    }
    else
    {
        return false;
    }
}

void KTMHandController::setOSCMapOut()
{
    
}

bool KTMHandController::routeOSC (const OSCMessage message)
{
    if (message.getAddressPattern().toString().contains(getOSCAddress()+"lights"))
    {
        static int args[2];
        bool valid = true;
        
        OSCArgument* curArg = message.begin();
        
        for (int i = 0; i < 2; i++)
        {
            if (curArg->isInt32())
            {
                args[i] = curArg->getInt32();
            }
            else
            {
                valid = false;
                break;
            }
            curArg++;
            
        }
        if (valid)
        {
            setLEDColour(args[0], args[1]);

        }
        
    }
    
    return false;
}

bool KTMHandController::pressButton(const int buttonID, const bool state)
{
    if (buttonID > -1 && buttonID < NUM_KTM_BUTTONS)
    {
        if (stateGrid[buttonID] != state)
        {
            stateGrid[buttonID] = state;
            
            setButtonLED(buttonID, stateGrid[buttonID]);
            
            if (stateGrid[buttonID] == 1)
            {
                DBG("KTMHC Button On: " + String(buttonID));
                AIMORouter::Instance()->routeMidi(getKeyMapping(buttonID), MidiMessage::noteOn(1, buttonID, uint8(110)));
                OSCMessage lightsMessage(getOSCAddress()+"lights");
                lightsMessage.addInt32(buttonID);
                lightsMessage.addInt32(ledColours[buttonID]->withRotatedHue(0.5).getARGB());
                
                AIMORouter::Instance()->routeOSC(lightsMessage);
            }
            else if (stateGrid[buttonID] == 0)
            {
                DBG("KTMHC Button Off: " + String(buttonID));
                AIMORouter::Instance()->routeMidi(getKeyMapping(buttonID), MidiMessage::noteOff(1, buttonID));
                OSCMessage lightsMessage(getOSCAddress()+"lights");
                lightsMessage.addInt32(buttonID);
                lightsMessage.addInt32(ledColours[buttonID]->withRotatedHue(0.5).getARGB());
                
                AIMORouter::Instance()->routeOSC(lightsMessage);
            }
            return true;
        }
        
    }
    
    return false;
}


void KTMHandController::timerCallback() //garbage collection for when a button release callback isn't received
{
    static OSCMessage poll("/inputs/digital/read");
    
    controllerSend.send(poll);
}

void KTMHandController::setButtonLED(const int forButton, const bool state)
{
    if (forButton > -1 && forButton < NUM_KTM_BUTTONS)
    {
        OSCMessage message("/outputs/digital/" + String(forButton+2));
        message.addInt32(state);
        controllerSend.send(message);
        
        if (state)
        {
            buttonDisplayBoxes[forButton]->setColour(Colours::red);
        }
        else
        {
            buttonDisplayBoxes[forButton]->setColour(Colours::darkred);
        }
    }
}

void KTMHandController::setLEDColour(const int led, const uint8 r, const uint8 g, const uint8 b)
{
    if (led > -1 && led < NUM_RGB_LEDS)
    {
        *ledColours[led] = Colour::fromRGB(r, g, b);
        ledDisplayBoxes[led]->setColour(*ledColours[led]);
        refreshColourLEDs();
    }
    else
    {
        jassertfalse;
    }
}

void KTMHandController::setLEDColour(const int led, const int ARGB)
{
    uint8 colours[4];
    
    colours[0] = (ARGB & 0x000000ff);
    colours[1] = (ARGB & 0x0000ff00) >> 8;
    colours[2] = (ARGB & 0x00ff0000) >> 16;
    colours[3] = (ARGB & 0xff000000) >> 24;
    
    setLEDColour(led, colours[2], colours[1], colours[0]);
}


void KTMHandController::refreshColourLEDs()
{
    rgbBlob.reset();
    
    int cR, cG, cB;
    for (int i = 0; i < ledColours.size(); i++)
    {
        cR = ledColours[i]->getRed() * 0.1;
        cG = ledColours[i]->getGreen() * 0.1;
        cB = ledColours[i]->getBlue() * 0.1;
        
        rgbBlob.append(&cR, 1);
        rgbBlob.append(&cG, 1);
        rgbBlob.append(&cB, 1);
        
        ledDisplayBoxes[i]->setColour(*ledColours[i]);
    }
    
    OSCMessage rgbMessage("/outputs/rgb/1");
    rgbMessage.addBlob(rgbBlob);
    controllerSend.send(rgbMessage);
    
}

void KTMHandController::setSceneLEDs(const Colour newColour)
{
    *ledColours[12] = newColour;
    *ledColours[13] = newColour;
    *ledColours[14] = newColour;
    
    refreshColourLEDs();
}

void KTMHandController::resized()
{
    float x = getWidth();
    float y = getHeight();
    
    if (x<=y)
    {
        mainBox.setBounds(0,0, x-10, x-10);
    }
    else
    {
        mainBox.setBounds(0,0, y-10, y-10);
    }
    
    mainBox.setCentre(x/2.0, y/2.0);
    
    buttonRows[0].setBounds(mainBox.getX(), mainBox.getY(), mainBox.getWidth()*0.2, mainBox.getWidth());
    for (int i = 1; i < 4; i++)
    {
        buttonRows[i].setBounds(buttonRows[i-1].getX() + buttonRows[0].getWidth(), buttonRows[0].getY(), buttonRows[0].getWidth(), buttonRows[0].getHeight());
    }
    
    for (int i = 0; i < NUM_KTM_BUTTONS; i++)
    {
        ledDisplayBoxes[i]->setBounds(buttonRows[i%4].withHeight((mainBox.getHeight()*0.05)+1).translated(0, (mainBox.getHeight()*0.25)* float(i/4)));
        
        buttonDisplayBoxes[i]->setBounds(ledDisplayBoxes[i]->getBounds().translated(0, mainBox.getHeight()*0.05).withHeight(ledDisplayBoxes[i]->getWidth()));
    }
    ledDisplayBoxes[12]->setBounds(buttonRows[3].getRight(), mainBox.getY(), mainBox.getWidth()/15.0, mainBox.getHeight()*0.05);
    ledDisplayBoxes[13]->setBounds(ledDisplayBoxes[12]->getBounds().translated(mainBox.getWidth()/15.0, 0));
    ledDisplayBoxes[14]->setBounds(ledDisplayBoxes[13]->getBounds().translated(mainBox.getWidth()/15.0, 0));

}


void KTMHandController::paint(Graphics& g)
{
    g.fillAll(Colours::darkslategrey);
    
    g.setColour(Colours::darkgrey);
    g.fillRect(mainBox);
}


void KTMHandController::mouseDown (const MouseEvent& event)
{
    DBG("click");
}
