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



KTMHandController::KTMHandController() : oscInputSocket(true)
{
    setInterceptsMouseClicks(false, true);
    
    controllerReceive.addListener(this);
    
    loadPreferencesFromFile();
    
    Array<IPAddress> addresses;
    IPAddress::findAllAddresses(addresses);
    bool ipSet = false;
    for (int i = 0; i < addresses.size(); i++)
    {
        if (addresses[i].toString().contains("192.168."))
        {
            if (oscInputSocket.bindToPort(KTM_CONTROLLER_RECEIVE, addresses[i].toString()))
            {
                Logger::writeToLog("OSC input socket bound to IP: " + addresses[i].toString() + ":" + String(KTM_CONTROLLER_RECEIVE));
            }
            else
            {
                 Logger::writeToLog("OSC input socket failed to bind to IP: " + addresses[i].toString() + ":" + String(KTM_CONTROLLER_RECEIVE));
            }
            ipSet = true;
        }
    }
    if (!ipSet)
    {
        Logger::writeToLog("ERROR - this machine is not connected to a network");
    }
    
    
    
    for (int i = 0; i < NUM_KTM_BUTTONS; i++)
    {
        stateGrid[i] = 0;
        setKeyMapping("/AIMO Control/", i);
        
        buttonModules[0].add(new MidiButtonModule(*this));
        buttonModules[0].getLast()->setColour(Colours::red);
        buttonModules[0].getLast()->addMouseListener(this, true);
        addAndMakeVisible(buttonModules[0].getLast());
        
        buttonModules[1].add(new MidiButtonModule(*this));
        buttonModules[1].getLast()->setColour(Colours::red);
        buttonModules[1].getLast()->addMouseListener(this, true);
        addAndMakeVisible(buttonModules[1].getLast());
        
        buttonModules[2].add(new MidiButtonModule(*this));
        buttonModules[2].getLast()->setColour(Colours::red);
        buttonModules[2].getLast()->addMouseListener(this, true);
        addAndMakeVisible(buttonModules[2].getLast());
        
        buttonModules[3].add(new MidiButtonModule(*this));
        buttonModules[3].getLast()->setColour(Colours::red);
        buttonModules[3].getLast()->addMouseListener(this, true);
        addAndMakeVisible(buttonModules[3].getLast());
        
        //setButtonLED(i, false);
    }

    setKeyMapping("/KTM/Scene", 10);
    setKeyMapping("/KTM/Scene", 10);
    
    setKeyMapping("/KTM/Page", 10+MAX_SCENES);
    setKeyMapping("/KTM/Page", 10+MAX_SCENES);

    
    buttonModules[0][0]->setButtonMode(MidiButtonModule::LiveLooper);
    buttonModules[0][1]->setButtonMode(MidiButtonModule::LiveLooper);
    buttonModules[0][2]->setButtonMode(MidiButtonModule::LiveLooper);
    buttonModules[0][3]->setButtonMode(MidiButtonModule::LiveLooper);
    
    buttonModules[0][0]->setLooperNumber(1);
    buttonModules[0][1]->setLooperNumber(2);
    buttonModules[0][2]->setLooperNumber(3);
    buttonModules[0][3]->setLooperNumber(4);
    
    buttonModules[0][4]->setButtonMode(MidiButtonModule::ToggleCC);
    buttonModules[0][5]->setButtonMode(MidiButtonModule::ToggleCC);
    buttonModules[0][6]->setButtonMode(MidiButtonModule::ToggleCC);
    buttonModules[0][7]->setButtonMode(MidiButtonModule::ToggleCC);
    
    buttonModules[1][0]->setButtonMode(MidiButtonModule::ToggleCC);
    buttonModules[1][1]->setButtonMode(MidiButtonModule::ToggleCC);
    buttonModules[1][2]->setButtonMode(MidiButtonModule::ToggleCC);
    buttonModules[1][3]->setButtonMode(MidiButtonModule::ToggleCC);
    buttonModules[1][4]->setButtonMode(MidiButtonModule::IncCC);
    buttonModules[1][5]->setButtonMode(MidiButtonModule::IncCC);
    buttonModules[1][6]->setButtonMode(MidiButtonModule::IncCC);
    buttonModules[1][7]->setButtonMode(MidiButtonModule::IncCC);
    
    buttonModules[2][0]->setButtonMode(MidiButtonModule::Standard);
    buttonModules[2][1]->setButtonMode(MidiButtonModule::Standard);
    buttonModules[2][2]->setButtonMode(MidiButtonModule::Standard);
    buttonModules[2][3]->setButtonMode(MidiButtonModule::Standard);
    buttonModules[2][4]->setButtonMode(MidiButtonModule::MultiPress);
    buttonModules[2][5]->setButtonMode(MidiButtonModule::MultiPress);
    buttonModules[2][6]->setButtonMode(MidiButtonModule::MultiPress);
    buttonModules[2][7]->setButtonMode(MidiButtonModule::MultiPress);
    
    buttonModules[3][0]->setButtonMode(MidiButtonModule::Standard);
    buttonModules[3][1]->setButtonMode(MidiButtonModule::Standard);
    buttonModules[3][2]->setButtonMode(MidiButtonModule::Standard);
    buttonModules[3][3]->setButtonMode(MidiButtonModule::Standard);
    buttonModules[3][4]->setButtonMode(MidiButtonModule::IncCC);
    buttonModules[3][5]->setButtonMode(MidiButtonModule::IncCC);
    buttonModules[3][6]->setButtonMode(MidiButtonModule::IncCC);
    buttonModules[3][7]->setButtonMode(MidiButtonModule::IncCC);
    
    ledColours[0].set(0, new Colour(Colours::black));
    ledColours[0].set(1, new Colour(Colours::black));
    ledColours[0].set(2, new Colour(Colours::black));
    ledColours[0].set(3, new Colour(Colours::black));
    ledColours[0].set(4, new Colour(Colours::blue));
    ledColours[0].set(5, new Colour(Colours::blue));
    ledColours[0].set(6, new Colour(Colours::blue));
    ledColours[0].set(7, new Colour(Colours::blue));
    ledColours[0].set(8, new Colour(Colours::black));
    ledColours[0].set(9, new Colour(Colours::black));
    ledColours[0].set(10, new Colour(Colours::indianred));
    ledColours[0].set(11, new Colour(Colours::indianred));
    
    ledColours[1].set(0, new Colour(Colours::blue));
    ledColours[1].set(1, new Colour(Colours::blue));
    ledColours[1].set(2, new Colour(Colours::blue));
    ledColours[1].set(3, new Colour(Colours::blue));
    ledColours[1].set(4, new Colour(Colours::green));
    ledColours[1].set(5, new Colour(Colours::green));
    ledColours[1].set(6, new Colour(Colours::green));
    ledColours[1].set(7, new Colour(Colours::green));
    ledColours[1].set(8, new Colour(Colours::black));
    ledColours[1].set(9, new Colour(Colours::green));
    ledColours[1].set(10, new Colour(Colours::indianred));
    ledColours[1].set(11, new Colour(Colours::indianred));
    
    ledColours[2].set(0, new Colour(Colours::red));
    ledColours[2].set(1, new Colour(Colours::red));
    ledColours[2].set(2, new Colour(Colours::red));
    ledColours[2].set(3, new Colour(Colours::red));
    ledColours[2].set(4, new Colour(Colours::orange));
    ledColours[2].set(5, new Colour(Colours::orange));
    ledColours[2].set(6, new Colour(Colours::orange));
    ledColours[2].set(7, new Colour(Colours::orange));
    ledColours[2].set(8, new Colour(Colours::red));
    ledColours[2].set(9, new Colour(Colours::black));
    ledColours[2].set(10, new Colour(Colours::indianred));
    ledColours[2].set(11, new Colour(Colours::indianred));
    
    ledColours[3].set(0, new Colour(Colours::red));
    ledColours[3].set(1, new Colour(Colours::green));
    ledColours[3].set(2, new Colour(Colours::purple));
    ledColours[3].set(3, new Colour(Colours::purple));
    ledColours[3].set(4, new Colour(Colours::purple));
    ledColours[3].set(5, new Colour(Colours::purple));
    ledColours[3].set(6, new Colour(Colours::purple));
    ledColours[3].set(7, new Colour(Colours::purple));
    ledColours[3].set(8, new Colour(Colours::purple));
    ledColours[3].set(9, new Colour(Colours::black));
    ledColours[3].set(10, new Colour(Colours::indianred));
    ledColours[3].set(11, new Colour(Colours::indianred));

    
    for (int i = 0; i < NUM_KTM_BUTTONS; i++)
    {
        
        ledDisplayBoxes[0].add(new ColouredBox());
        ledDisplayBoxes[0].getLast()->addMouseListener(this, true);
        addAndMakeVisible(ledDisplayBoxes[0].getLast());
        ledDisplayBoxes[0].getLast()->setColour(*ledColours[0].getUnchecked(i));
        
        ledDisplayBoxes[1].add(new ColouredBox());
        ledDisplayBoxes[1].getLast()->addMouseListener(this, true);
        addChildComponent(ledDisplayBoxes[1].getLast());
        ledDisplayBoxes[1].getLast()->setColour(*ledColours[1].getUnchecked(i));
        
        ledDisplayBoxes[2].add(new ColouredBox());
        ledDisplayBoxes[2].getLast()->addMouseListener(this, true);
        addChildComponent(ledDisplayBoxes[2].getLast());
        ledDisplayBoxes[2].getLast()->setColour(*ledColours[2].getUnchecked(i));
        
        ledDisplayBoxes[3].add(new ColouredBox());
        ledDisplayBoxes[3].getLast()->addMouseListener(this, true);
        addChildComponent(ledDisplayBoxes[3].getLast());
        ledDisplayBoxes[3].getLast()->setColour(*ledColours[3].getUnchecked(i));
        
    }
    
    for (int i = 0; i < 3; i++) //sceneLEDS
    {
        ledDisplayBoxes[0].add(new ColouredBox());
        ledDisplayBoxes[0].getLast()->addMouseListener(this, true);
        addAndMakeVisible(ledDisplayBoxes[0].getLast());
    }
    
    setAddress("/KTM/");
    setOSCAddress("/KTMOSC/");
    AIMORouter::Instance()->addDestination(this);
    AIMORouter::Instance()->addOSCDestination(this);
    
    clearButtonChannel.setRange(1, 16, 1);
    clearButtonChannel.setSliderStyle(Slider::IncDecButtons);
    addAndMakeVisible(clearButtonChannel);
    
    sendClearButton.setButtonText("Map Clear");
    sendClearButton.addListener(this);
    sendClearButton.setConnectedEdges(15);
    addAndMakeVisible(sendClearButton);
    
    sceneLEDColour = *sceneColours.getFirst();

    sceneNumberLabel.setText("Scene: " + String(currentScene+1), dontSendNotification);
    sceneNumberLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(sceneNumberLabel);
    
    controllerIpLabel.setText("Device Address:", dontSendNotification);
    addAndMakeVisible(controllerIpLabel);
    
    controllerIpEditor.setMultiLine(false);
    controllerIpEditor.setText("192.168.0.103");
    controllerIpEditor.addListener(this);
    addAndMakeVisible(controllerIpEditor);
    
    //startTimer(INPUT_POLL, 50);
    if (ipSet)
    {
        pollForControllerAtAddress("192.168.0.103");
    }
    
    setPage(0);
}

KTMHandController::~KTMHandController()
{
    controllerReceive.disconnect();
    oscInputSocket.shutdown();
}

bool KTMHandController::connect(String sendAddress)
{
    controllerSend.disconnect();
    controllerReceive.disconnect();
    
    if (controllerReceive.connectToSocket(oscInputSocket) && controllerSend.connect(sendAddress, KTM_CONTROLLER_SEND))
    {
        Logger::writeToLog("Connected to OSC send/receive sockets");
        return true;
    }
    else
    {
        Logger::writeToLog("OSC send/receive sockets connection error");
        return false;
    }
    
   
}


void KTMHandController::oscMessageReceived (const OSCMessage& message)
{
    //Logger::writeToLog(message.getAddressPattern().toString() + " ARGS: " + String(message.size()));
    
    static String address;
    address = message.getAddressPattern().toString();
    
    
    if (address == "/inputs/digital")
    {
        OSCArgument* curArg = message.begin();;
        for (int i = 0; i < NUM_KTM_BUTTONS; i++)
        {
            if (curArg->isInt32())
            {
                //pressButton(i, curArg->getInt32());
                if (curArg->getInt32() != stateGrid[i])
                {
                    stateGrid[i] = curArg->getInt32();
                    if (stateGrid[i] == 1)
                    {
                        routeMidi(getAddress()+"key", MidiMessage::noteOn(1, i, uint8(110)));
                    }
                    else if (stateGrid[i] == 0)
                    {
                        routeMidi(getAddress()+"key", MidiMessage::noteOff(1, i));
                    }
                }
            }
            curArg++;
        }
        
        if(!controllerConnected)
        {
            Logger::writeToLog("Hand controller connected - initialising!");
            refreshColourLEDs();
            for (int i = 0; i < 12; i++)
            {
                setButtonLED(i, buttonModules[currentPage][i]->getButtonState());
            }
            startTimer(INPUT_POLL, 50);
            controllerConnected = true;
            Logger::writeToLog("Ready.");
        }
        timeOfLastPing = Time::getMillisecondCounter();
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
            int buttonsDown = 0;
            for (int i = 0; i < NUM_KTM_BUTTONS; i++)
            {
                if (stateGrid[i])
                {
                    buttonsDown++;
                }
            }
            
            int noteNumber = message.getNoteNumber();
            if (noteNumber > -1 && noteNumber < NUM_KTM_BUTTONS)
            {
                if (buttonsDown <= 1)
                {
                    bool prevState = buttonModules[currentPage][noteNumber]->getButtonState();
                    MidiMessage mappedMessage(message);
                    
                    mappedMessage.setChannel(currentPage+1);
                    
                    buttonModules[currentPage][message.getNoteNumber()]->processMidi(&mappedMessage);
                    setButtonLED(noteNumber, buttonModules[currentPage][noteNumber]->getButtonState());
                    
                    // when a button state changes
                    if (buttonModules[currentPage][noteNumber]->getButtonState() != prevState)
                    {
                        if (noteNumber < 8) //not scene/page change
                        {
                            if (currentPage == 2 && noteNumber < 4) // master clear buttons
                            {
                                if (mappedMessage.isNoteOn())
                                {
                                    if (buttonModules[2][noteNumber]->getTimeSinceLastClick() < 200)
                                    {
                                        buttonModules[0][message.getNoteNumber()]->clearLooper();
                                    }
                                    else
                                    {
                                        buttonModules[0][message.getNoteNumber()]->looperUndo();
                                    }
                                }
                            }
                            else if (currentPage == 3) // master clear/play buttons
                            {
                                if (noteNumber == 0)
                                {
                                    if (mappedMessage.isNoteOn())
                                    {
                                        buttonModules[0][0]->looperStop();
                                        buttonModules[0][1]->looperStop();
                                        buttonModules[0][2]->looperStop();
                                        buttonModules[0][3]->looperStop();
                                    }
                                }
                                else if (noteNumber == 1)
                                {
                                    if (mappedMessage.isNoteOn())
                                    {
                                        buttonModules[0][0]->looperStart();
                                        buttonModules[0][1]->looperStart();
                                        buttonModules[0][2]->looperStart();
                                        buttonModules[0][3]->looperStart();
                                    }
                                }
                            }
                            
                            else if (buttonModules[currentPage][noteNumber]->getButtonMode() == MidiButtonModule::LiveLooper)
                            {
                                if (message.isNoteOn())
                                {
                                    AIMORouter::Instance()->routeMidi(getKeyMapping(noteNumber), mappedMessage);
                                    refreshLooperState(noteNumber);
                                }
                            }
                            
                            
                            else
                            {
                                AIMORouter::Instance()->routeMidi(getKeyMapping(noteNumber), mappedMessage);
                            }
                            
                        }
                        else // scene/page change
                        {
                            buttonModules[0][noteNumber]->setButtonState(buttonModules[currentPage][noteNumber]->getButtonState());
                            buttonModules[1][noteNumber]->setButtonState(buttonModules[currentPage][noteNumber]->getButtonState());
                            buttonModules[2][noteNumber]->setButtonState(buttonModules[currentPage][noteNumber]->getButtonState());
                            buttonModules[3][noteNumber]->setButtonState(buttonModules[currentPage][noteNumber]->getButtonState());
                            
                            if (buttonModules[currentPage][noteNumber]->getButtonState() == 1)
                            {
                                if (noteNumber == 10) // scene up
                                {
                                    setScene(currentScene-1);
                                }
                                else if (noteNumber == 11) // scene down
                                {
                                    setScene(currentScene+1);
                                }
                                else if (noteNumber == 9) // page up
                                {
                                    if (currentPage == 0)
                                    {
                                        setPage(1);
                                    }
                                    else if (currentPage == 2)
                                    {
                                        setPage(3);
                                    }
                                    else if (currentPage == 3)
                                    {
                                        setPage(0);
                                    }
                                }
                                else if (noteNumber == 8) // page down
                                {
                                    if (currentPage == 0)
                                    {
                                        setPage(3);
                                    }
                                    else if (currentPage == 1)
                                    {
                                        setPage(0);
                                    }
                                    else if (currentPage == 3)
                                    {
                                        setPage(2);
                                    }
                                }
                            }
                        }
                    }
                    
                }
                else if (buttonsDown == 2)
                {

                    if (currentPage == 2 || currentPage == 1 || currentPage == 3)
                    {
                        for (int i = 4; i < 8; i++) // middle row
                        {
                            if (stateGrid[i] && stateGrid[i-4]) // middle and top
                            {
                                MidiMessage multiFunction(MidiMessage::controllerEvent(currentPage+1, noteNumber, 127));
                                buttonModules[currentPage][i]->processMidi(&multiFunction);
                                AIMORouter::Instance()->routeMidi(getKeyMapping(i), multiFunction);
                            }
                            else if (stateGrid[i] && stateGrid[i+4]) // middle and bottom
                            {
                                MidiMessage multiFunction(MidiMessage::controllerEvent(currentPage+1, noteNumber, 0));
                                buttonModules[currentPage][i]->processMidi(&multiFunction);
                                AIMORouter::Instance()->routeMidi(getKeyMapping(i), multiFunction);
                            }
                        }
                    }
                }
                // Instant button press feedback
                
                if (message.getNoteNumber() != 8 && message.getNoteNumber() != 9)
                {
                    if (buttonModules[currentPage][noteNumber]->getButtonMode() != MidiButtonModule::LiveLooper)
                    {
                        OSCMessage lightsMessage(getOSCAddress()+"lights");
                        lightsMessage.addInt32((noteNumber%NUM_KTM_BUTTONS));
                        lightsMessage.addInt32(ledColours[currentPage][noteNumber%NUM_KTM_BUTTONS]->withRotatedHue(0.5).getARGB());
                        
                        AIMORouter::Instance()->routeOSC(lightsMessage);
                    }
                   
                }
                return true;

            }
        }
        else
        {
            jassertfalse;
        }
    }
    else if (address.contains(getAddress()+"Scene"))
    {
        
    }
    return false;
}

void KTMHandController::sendToAddress(const MidiMessage message)
{
    AIMORouter::Instance()->routeMidi(getKeyMapping(0), message);
    
    for (int i = 0; i < 4; i++)
    {
        refreshLooperState(i);
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



void KTMHandController::timerCallback (int timerID)//garbage collection for when a button release callback isn't received
{
    if(timerID == INPUT_POLL)
    {
        static OSCMessage poll("/inputs/digital/read");
        controllerSend.send(poll);
    }
    else if (timerID == CONNECTION_CHECK)
    {
        if (controllerConnected)
        {
            if (Time::getMillisecondCounter() - timeOfLastPing > 500)
            {
                Logger::writeToLog("CONTROLLER DISCONNECTED - TIMED OUT");
                controllerConnected = false;
                stopTimer(INPUT_POLL);
            }
        }
        else
        {
            
            Array<IPAddress> addresses;
            IPAddress::findAllAddresses(addresses);
            bool pollSent = false;
            for (int i = 0; i < addresses.size(); i++)
            {
                if (addresses[i].toString().contains("192.168."))
                {
                    OSCMessage ipReset("/osc/remote/ip");
                    ipReset.addString("255.255.255.255");
                    controllerSend.send(ipReset);
                    
                    OSCMessage ipSet("/osc/remote/ip");
                    ipSet.addString(addresses[i].toString());
                    controllerSend.send(ipSet);
        
                    static OSCMessage poll("/inputs/digital/read");
                    controllerSend.send(poll);
                    
                    pollSent = true;
                }
            }
            if (!pollSent)
            {
               Logger::writeToLog("Connection error: no network detected");
            }
        }
    }
    
}

void KTMHandController::setButtonLED(const int forButton, const bool state)
{
    if (forButton > -1 && forButton < NUM_KTM_BUTTONS)
    {
        OSCMessage message("/outputs/digital/" + String(forButton+2));
        message.addInt32(state);
        controllerSend.send(message);
        
    }
}

void KTMHandController::setLEDColour(const int led, const uint8 r, const uint8 g, const uint8 b)
{
    if (led > -1 && led < NUM_KTM_BUTTONS)
    {
        *ledColours[currentPage][led] = Colour::fromRGB(r, g, b);
        //*ledColours[led] = Colour::fromRGB(r, g, b);

        //ledDisplayBoxes[led]->setColour(*liveLEDColours[led]);
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
    for (int i = 0; i < NUM_KTM_BUTTONS; i++)
    {
        cR = ledColours[currentPage][i]->getRed() * 0.1;
        cG = ledColours[currentPage][i]->getGreen() * 0.1;
        cB = ledColours[currentPage][i]->getBlue() * 0.1;
        
        rgbBlob.append(&cR, 1);
        rgbBlob.append(&cG, 1);
        rgbBlob.append(&cB, 1);
        
        ledDisplayBoxes[currentPage][i]->setColour(*ledColours[currentPage][i]);
    }
    
    cR = sceneLEDColour.getRed() * 0.1;
    cG = sceneLEDColour.getGreen() * 0.1;
    cB = sceneLEDColour.getBlue() * 0.1;
    
    for (int i = 0; i < 3; i++)
    {
        rgbBlob.append(&cR, 1);
        rgbBlob.append(&cG, 1);
        rgbBlob.append(&cB, 1);
        
        ledDisplayBoxes[0][NUM_KTM_BUTTONS+i]->setColour(sceneLEDColour);
    }
    
    
    OSCMessage rgbMessage("/outputs/rgb/1");
    rgbMessage.addBlob(rgbBlob);
    controllerSend.send(rgbMessage);
    
}

void KTMHandController::setScene(const int newScene)
{
    if (newScene > -1 && newScene < MAX_SCENES)
    {
        currentScene = newScene;
        setSceneLEDs(*sceneColours[newScene]);
        sceneNumberLabel.setText("Scene: " + String(currentScene+1), dontSendNotification);
        
        AIMORouter::Instance()->routeMidi("AIMO Control", MidiMessage::noteOn(16, newScene, uint8(127)));

    }
    else if (newScene == -1)
    {
        setScene(0);
    }
    else if (newScene == MAX_SCENES)
    {
        setScene(MAX_SCENES-1);
    }
}


void KTMHandController::setSceneLEDs(const Colour newColour)
{
    sceneLEDColour = newColour;
    
    refreshColourLEDs();
}

void KTMHandController::setPage(const int page)
{
    if (page > -1 && page < KTM_NUM_PAGES)
    {
        currentPage = page;

        
        for (int i = 0; i < KTM_NUM_PAGES; i++)
        {
            if (i == currentPage)
            {
                for (int j = 0; j < buttonModules[i].size(); j++)
                {
                    buttonModules[i].getUnchecked(j)->setVisible(true);
                    ledDisplayBoxes[i].getUnchecked(j)->setVisible(true);

                }
            }
            else
            {
                for (int j = 0; j < buttonModules[i].size(); j++)
                {
                    buttonModules[i].getUnchecked(j)->setVisible(false);
                    ledDisplayBoxes[i].getUnchecked(j)->setVisible(false);

                }
            }
        }
        
        for (int i = 0; i < NUM_KTM_BUTTONS; i++)
        {
            setButtonLED(i, buttonModules[currentPage][i]->getButtonState());
            if (i < 4)
            {
                refreshLooperState(i);
            }
            refreshColourLEDs();
        }

    }
   

}


void KTMHandController::resized()
{
    float x = getWidth();
    float y = getHeight();
    
    if (x<=y)
    {
        mainBox.setBounds(0,0, x, x);
    }
    else
    {
        mainBox.setBounds(0,0, y, y);
    }
    
   // mainBox.setCentre(x/2.0, y/2.0);
    
    buttonRows[0].setBounds(mainBox.getX(), mainBox.getY(), mainBox.getWidth()*0.2, mainBox.getWidth());
    for (int i = 1; i < 4; i++)
    {
        buttonRows[i].setBounds(buttonRows[i-1].getX() + buttonRows[0].getWidth(), buttonRows[0].getY(), buttonRows[0].getWidth(), buttonRows[0].getHeight());
    }
    
    for (int i = 0; i < NUM_KTM_BUTTONS; i++)
    {
        ledDisplayBoxes[0][i]->setBounds(buttonRows[i%4].withHeight((mainBox.getHeight()*0.05)+1).translated(0, (mainBox.getHeight()*0.25)* float(i/4)));
        ledDisplayBoxes[1][i]->setBounds(ledDisplayBoxes[0][i]->getBounds());
        ledDisplayBoxes[2][i]->setBounds(ledDisplayBoxes[0][i]->getBounds());
        ledDisplayBoxes[3][i]->setBounds(ledDisplayBoxes[0][i]->getBounds());

        
        buttonModules[0][i]->setBounds(ledDisplayBoxes[0][i]->getBounds().translated(0, mainBox.getHeight()*0.05).withHeight(ledDisplayBoxes[0][i]->getWidth()));
        buttonModules[1][i]->setBounds(ledDisplayBoxes[0][i]->getBounds().translated(0, mainBox.getHeight()*0.05).withHeight(ledDisplayBoxes[0][i]->getWidth()));
        buttonModules[2][i]->setBounds(ledDisplayBoxes[0][i]->getBounds().translated(0, mainBox.getHeight()*0.05).withHeight(ledDisplayBoxes[0][i]->getWidth()));
        buttonModules[3][i]->setBounds(ledDisplayBoxes[0][i]->getBounds().translated(0, mainBox.getHeight()*0.05).withHeight(ledDisplayBoxes[0][i]->getWidth()));
    }
    
    ledDisplayBoxes[0][12]->setBounds(buttonRows[3].getRight(), mainBox.getY(), mainBox.getWidth()/15.0, mainBox.getHeight()*0.05);
    ledDisplayBoxes[0][13]->setBounds(ledDisplayBoxes[0][12]->getBounds().translated(mainBox.getWidth()/15.0, 0));
    ledDisplayBoxes[0][14]->setBounds(ledDisplayBoxes[0][13]->getBounds().translated(mainBox.getWidth()/15.0, 0));
    
    sceneNumberLabel.setBounds(ledDisplayBoxes[0][12]->getBounds().translated(0, ledDisplayBoxes[0][12]->getHeight()+5).withWidth(mainBox.getWidth()/5));
    
    mainBox.setBottom(buttonModules[0][11]->getBottom()+10);
    
    sendClearButton.setBounds(mainBox.getX(), mainBox.getBottom()+10, 100, 30);
    clearButtonChannel.setBounds(sendClearButton.getBounds().translated(110, 0));
    
    controllerIpLabel.setBounds(sendClearButton.getBounds().translated(0, 40).withWidth(100));
    controllerIpEditor.setBounds(controllerIpLabel.getBounds().translated(110, 0).withWidth(200));
}


void KTMHandController::paint(Graphics& g)
{
    g.fillAll(Colour(0xff263238));
    
    g.setColour(Colour(0xff323e44));
    g.fillRect(mainBox);
}


void KTMHandController::mouseDown (const MouseEvent& event)
{
    if (event.mods.isRightButtonDown())
    {
        if (event.eventComponent != this)
        {
            for (int i = 0; i < NUM_KTM_BUTTONS; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    if (event.eventComponent == buttonModules[j][i])
                    {
                        CallOutBox::launchAsynchronously(new MidiButtonModule::ConfigComponent(buttonModules[j][i]), buttonModules[j][i]->getScreenBounds(), this);
                    }
                }
            }
            
        }
    }
}

void KTMHandController::refreshLooperState(const int forButton)
{
    
    switch (buttonModules[currentPage][forButton]->getLooperButtonMode())
    {
        case MidiButtonModule::Play:
            setLEDColour(forButton, Colours::cyan.getARGB());
            break;
        case MidiButtonModule::Stop:
            setLEDColour(forButton, Colours::white.getARGB());
            break;
        case MidiButtonModule::Record:
            setLEDColour(forButton, Colours::deeppink.getARGB());
            break;
        case MidiButtonModule::Overdub:
            setLEDColour(forButton, Colours::orange.getARGB());
            break;
        case MidiButtonModule::Clear:
            setLEDColour(forButton, Colours::black.getARGB());
            break;
        default:
            break;
    }
}

void KTMHandController::buttonClicked(Button* button)
{
    if (button == &sendClearButton)
    {
        AIMORouter::Instance()->routeMidi("AIMO Out", MidiMessage::noteOn(clearButtonChannel.getValue(), 124, uint8(127)));
    }
}

void KTMHandController::textEditorReturnKeyPressed (TextEditor& editor)
{
    if (&editor == &controllerIpEditor)
    {
        String newIp = editor.getText().retainCharacters("1234567890.");
        //TODO: Better validation here
        if (newIp != controllerIpString)
        {
            pollForControllerAtAddress(newIp);
            controllerIpString = newIp;
        }
    }
}

void KTMHandController::pollForControllerAtAddress(String controllerAddress)
{
    connect(controllerAddress);
    Logger::writeToLog("Polling for KTM hand controller with IP: " + controllerAddress);
    startTimer(CONNECTION_CHECK, 500);
}


void KTMHandController::loadPreferencesFromFile()
{
    preferencesFile = File("./KTM Config.xml");
    if (!preferencesFile.exists())
    {
        Logger::writeToLog("XML Prefs not found - creating file");
        preferencesFile.create();
        XmlElement defaultData("KTM_Scene_Data");
        
        
        for (int i = 0; i < MAX_SCENES; i++)
        {
            defaultData.createNewChildElement("Scene_" + String(i));
            XmlElement* thisScene = defaultData.getChildElement(i);
            thisScene->setAttribute("dress_command", "~/Scene" + String(i) + ".xml");
            
            if (i == 0)
            {
                sceneColours.add(new Colour(Colours::blue));
            }
            else
            {
                sceneColours.add(new Colour(sceneColours.getLast()->withRotatedHue(0.63)));
            }
            thisScene->setAttribute("r", String(sceneColours.getLast()->getRed()));
            thisScene->setAttribute("g", String(sceneColours.getLast()->getGreen()));
            thisScene->setAttribute("b", String(sceneColours.getLast()->getBlue()));
        }
        
        if(defaultData.writeToFile(preferencesFile, ""))
        {
            Logger::writeToLog("XML prefs file created successfully");
        }
        else
        {
            Logger::writeToLog("EROOR - XML prefs file not created");
        }
    }
    else
    {
        Logger::writeToLog("XML prefs file found - loading");
        
        XmlDocument prefsDoc(preferencesFile);
        prefsXML = prefsDoc.getDocumentElement();
        
        if (prefsXML != nullptr)
        {
            for (int i = 0; i < MAX_SCENES; i++)
            {
                XmlElement* thisElement = prefsXML->getChildElement(i);
                dress.setSceneCommand(i, thisElement->getStringAttribute("dress_command"));
                int r, g, b;
                r = thisElement->getStringAttribute("r").getIntValue();
                g = thisElement->getStringAttribute("g").getIntValue();
                b = thisElement->getStringAttribute("b").getIntValue();
                sceneColours.set(i, new Colour(r, g, b));
            }
        }
        else
        {
            Logger::writeToLog("ERROR - Bad data in prefs file");
        }
        
        
    }
}
