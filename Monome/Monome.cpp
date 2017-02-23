//
//  Monome.cpp
//  AI:MO
//
//  Created by Luke Saxton on 14/01/2017.
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

#include "Monome.hpp"
#define MONOME_RECEIVE 7865
#include "AIMORouter.hpp"

Monome::Monome()
{
    monomeReceive.addListener(this);
    
    //numberMasks.add(NumberMasks::oneL);
    
    for (int i = 0; i < 64; ++i)
    {
        setKeyMapping("/controllerOne/grid/key", i);
    }
    
}

Monome::~Monome()
{
    HighResolutionTimer::stopTimer();
    
    
    if (deviceConnected)
    {
        OSCMessage lightsOff(monomeData.prefix + "/grid/led/all");
        lightsOff.addInt32(0);
        
        monomeSend.send(lightsOff);
    }
}



bool Monome::connectToDevice(MonomeData deviceInfo)
{
    
    monomeData = deviceInfo;
    
    if (monomeReceive.connect(MONOME_RECEIVE) && monomeSend.connect(monomeData.host, monomeData.port))
    {
        deviceConnected = true;
        
        OSCMessage setPrefix("/sys/prefix");
        monomeData.prefix = "/aimo";
        setPrefix.addString(monomeData.prefix);
        monomeSend.send(setPrefix);
        
        OSCMessage setPort("/sys/port");
        setPort.addInt32(MONOME_RECEIVE);
        monomeSend.send(setPort);
        
        setIntensity(15);
        
        //Timer::startTimer(100);
        
        //setMask(CircleMasks::ring4);
        
        HighResolutionTimer::startTimer(20);
        
        
        //setMask(LetterMasks::Z);

    }
    else{
        jassertfalse;
        return false;
    }

    return true;
    
        //pollConnectedMonome();
        
        //startIdle();
    
    
}

void Monome::oscMessageReceived (const OSCMessage& message)
{
    if (message.getAddressPattern().toString() == (monomeData.prefix + "/grid/key"))
    {

        //stopIdle();
        OSCArgument* cur = message.begin();
        
        OSCArgument xArg(*cur);
        OSCArgument yArg(*++cur);
        OSCArgument sArg(*++cur);
        
        int x, y, s;
        x = xArg.getInt32();
        y = yArg.getInt32();
        s = sArg.getInt32();

        if (x < monomeData.size[0] && y < monomeData.size[1])
        {
            grid[x][y].lastPress = time.getMillisecondCounter();
            grid[x][y].s = s;
            setLight(x, y, s);

            int padNum = x + (y*8);

            String mappingString = getKeyMapping(padNum);
            
            MidiMessage midiMessage;
            
            if (s)
            {
                midiMessage = MidiMessage::noteOn(1, padNum, uint8(120));

            }
            else
            {
                midiMessage = MidiMessage::noteOff(1, padNum);
            }
            midiMessage.setChannel(1);
            
            
            AIMORouter::Instance()->routeMidi(mappingString, midiMessage);
            
            
            if (numberMode && keysPressed == 0)
            {
                displayNumber(((y*8)+x)+1);
            }
            
            if (s)
            {
                keysPressed++;
            }
            else{
                keysPressed--;
                
            }
            
            //DBG("Keys Pressed: " + String(keysPressed));
            
           
            
            if (keysPressed == 2) {
                if (grid[0][0].s && grid[1][0].s)
                {
                    if (Timer::isTimerRunning())
                    {
                        stopIdle();
                    }
                    else
                    {
                        Timer::startTimer(100);
                    }
                }
                else if (grid[6][7].s && grid[7][7].s) //kill all lights
                {
                    Timer::stopTimer();
                    lightsOff();
                }
                else if (grid[7][6].s && grid[7][7].s) //show all Lights
                {
                    Timer::stopTimer();
                    lightsOn();
                }
                else if (grid[7][0].s && grid[7][1].s) //enable number mode
                {
                    numberMode = true;
                }
                else if (grid[7][0].s && grid[6][0].s) //disable number mode
                {
                    numberMode = false;
                    lightsOff();
                }
            }
        }

    }
}

void Monome::setIntensity(const int intensity)
{
    OSCMessage setIntensity(monomeData.prefix + "/grid/led/intensity");
    setIntensity.addInt32(intensity);
    
    monomeSend.send(setIntensity);
}

void Monome::pollConnectedMonome()
{
    OSCMessage pollMonome("/sys/info");
    pollMonome.addString("127.0.0.1");
    pollMonome.addInt32(MONOME_RECEIVE);
    
    monomeSend.send(pollMonome);
}


void Monome::lightsOff()
{
//    OSCMessage lightsOff(monomeData.prefix + "/grid/led/all");
//    lightsOff.addInt32(0);
    
    for (int i = 0; i < 8; i++)
    {
        backgroundMask[i] = 0;
    }
    
    //monomeSend.send(lightsOff);
}

void Monome::lightsOn()
{
//    OSCMessage lightsOn(monomeData.prefix + "/grid/led/all");
//    lightsOn.addInt32(1);
    
    
    for (int i = 0; i < 8; i++)
    {
        backgroundMask[i] = 255;
    }
    //monomeSend.send(lightsOn);
    
}

void Monome::setLight(const int x, const int y, const bool state)
{
    OSCMessage setLight(monomeData.prefix + "/grid/led/set");
    setLight.addInt32(x);
    setLight.addInt32(y);
    setLight.addInt32(state);
    
    if (state)
    {
        foregroundMask[y] = foregroundMask[y] | int32(pow(2, x));
    }
    else
    {
        foregroundMask[y] = foregroundMask[y] & !int32(pow(2, x));
    }
    
}

void Monome::setMask(const int bitmaskArray[8], bool makeForeground)
{
    for (int i = 0; i < 8; ++i)
    {
        if (makeForeground)
        {
            foregroundMask[i] = bitmaskArray[i];

        }
        else
        {
            backgroundMask[i] = bitmaskArray[i];

        }
    }
   
}

void Monome::displayNumber(const int numToDisplay)
{
    
    if (numToDisplay < 100 && numToDisplay > -1)
    {
        int rightNumMask[8];
        
        int moduloNum = numToDisplay % 10;
        
       
        if (getMaskForNumber(moduloNum, rightNumMask, false))
        {
            
        }
        
        if (numToDisplay / 10 > 0)
        {
            int leftNumMask[8];

            if (getMaskForNumber(numToDisplay/10, leftNumMask, true))
            {
                int intMask[8] = {leftNumMask[0] | rightNumMask[0], leftNumMask[1] | rightNumMask[1], leftNumMask[2] | rightNumMask[2], leftNumMask[3] | rightNumMask[3], leftNumMask[4] |rightNumMask[4], leftNumMask[5] | rightNumMask[5], leftNumMask[6] | rightNumMask[6], leftNumMask[7] | rightNumMask[7]};
                setMask(intMask);
                
            }
        }
        else
        {
            int intMask[8] = {rightNumMask[0], rightNumMask[1], rightNumMask[2], rightNumMask[3], rightNumMask[4], rightNumMask[5], rightNumMask[6], rightNumMask[7]};
            setMask(intMask);
        }
    }
    
   
  
}


bool Monome::getMaskForNumber(const int number, int* arrayToFill, const bool isLeftNum)
{
    if (number < 10 && number > -1)
    {
        
        const int* foundMaskStart = nullptr;
        
        
            switch (number) {
                case 0:
                    foundMaskStart = &NumberMasks::zeroL[0];
                    break;
                case 1:
                    foundMaskStart = &NumberMasks::oneL[0];
                    break;
                case 2:
                    foundMaskStart = &NumberMasks::twoL[0];
                    break;
                case 3:
                    foundMaskStart = &NumberMasks::threeL[0];
                    break;
                case 4:
                    foundMaskStart = &NumberMasks::fourL[0];
                    break;
                case 5:
                    foundMaskStart = &NumberMasks::fiveL[0];
                    break;
                case 6:
                    foundMaskStart = &NumberMasks::sixL[0];
                    break;
                case 7:
                    foundMaskStart = &NumberMasks::sevenL[0];
                    break;
                case 8:
                    foundMaskStart = &NumberMasks::eightL[0];
                    break;
                case 9:
                    foundMaskStart = &NumberMasks::nineL[0];
                    break;
                default:
                    return false;
                    break;
            }
            
            if (foundMaskStart != nullptr)
            {
                if (isLeftNum)
                {
                    for (int i = 0 ; i < 8; i++)
                    {
                        *(arrayToFill + i) = *(foundMaskStart + i)<<1;
                    }
                    return true;
                }
                else
                {
                    for (int i = 0 ; i < 8; i++)
                    {
                        *(arrayToFill + i) = (*(foundMaskStart + i)<<5);
                    }
                    return true;
                }
                

            }
            
            return false;
        }
    else{
        return false;
    }
}

bool Monome::getMaskForLetter(const String letter, int* arrayToFill, const bool isLeftChar)
{
    
    
    if (arrayToFill != nullptr)
    {
        const int* foundMaskStart = nullptr;

        if (letter.compareIgnoreCase("A"))
        {
            foundMaskStart = &LetterMasks::A[0];
        }
        else if (letter.compareIgnoreCase("B"))
        {
            foundMaskStart = &LetterMasks::B[0];
        }
        else if (letter.compareIgnoreCase("C"))
        {
            foundMaskStart = &LetterMasks::C[0];
        }
        else if (letter.compareIgnoreCase("D"))
        {
            foundMaskStart = &LetterMasks::D[0];
        }
        else if (letter.compareIgnoreCase("E"))
        {
            foundMaskStart = &LetterMasks::E[0];
        }
        else if (letter.compareIgnoreCase("F"))
        {
            foundMaskStart = &LetterMasks::F[0];
        }
        else if (letter.compareIgnoreCase("G"))
        {
            foundMaskStart = &LetterMasks::G[0];
        }
        else if (letter.compareIgnoreCase("H"))
        {
            foundMaskStart = &LetterMasks::H[0];
        }
        else if (letter.compareIgnoreCase("I"))
        {
            foundMaskStart = &LetterMasks::I[0];
        }
        else if (letter.compareIgnoreCase("J"))
        {
            foundMaskStart = &LetterMasks::J[0];
        }
        else if (letter.compareIgnoreCase("K"))
        {
            foundMaskStart = &LetterMasks::K[0];
        }
        else if (letter.compareIgnoreCase("L"))
        {
            foundMaskStart = &LetterMasks::L[0];
        }
        else if (letter.compareIgnoreCase("M"))
        {
            foundMaskStart = &LetterMasks::M[0];
        }
        else if (letter.compareIgnoreCase("N"))
        {
            foundMaskStart = &LetterMasks::N[0];
        }
        else if (letter.compareIgnoreCase("O"))
        {
            foundMaskStart = &LetterMasks::O[0];
        }
        else if (letter.compareIgnoreCase("P"))
        {
            foundMaskStart = &LetterMasks::P[0];
        }
        else if (letter.compareIgnoreCase("Q"))
        {
            foundMaskStart = &LetterMasks::Q[0];
        }
        else if (letter.compareIgnoreCase("R"))
        {
            foundMaskStart = &LetterMasks::R[0];
        }
        else if (letter.compareIgnoreCase("S"))
        {
            foundMaskStart = &LetterMasks::S[0];
        }
        else if (letter.compareIgnoreCase("T"))
        {
            foundMaskStart = &LetterMasks::T[0];
        }
        else if (letter.compareIgnoreCase("U"))
        {
            foundMaskStart = &LetterMasks::U[0];
        }
        else if (letter.compareIgnoreCase("V"))
        {
            foundMaskStart = &LetterMasks::V[0];
        }
        else if (letter.compareIgnoreCase("W"))
        {
            foundMaskStart = &LetterMasks::W[0];
        }
        else if (letter.compareIgnoreCase("X"))
        {
            foundMaskStart = &LetterMasks::X[0];
        }
        else if (letter.compareIgnoreCase("Y"))
        {
            foundMaskStart = &LetterMasks::Y[0];
        }
        else if (letter.compareIgnoreCase("Z"))
        {
            foundMaskStart = &LetterMasks::Z[0];
        }
        
        
        if (foundMaskStart != nullptr)
        {
            if (isLeftChar)
            {
                for (int i = 0 ; i < 8; i++)
                {
                    *(arrayToFill + i) = *(foundMaskStart + i)<<1;
                }
                return true;
            }
            else
            {
                for (int i = 0 ; i < 8; i++)
                {
                    *(arrayToFill + i) = (*(foundMaskStart + i)<<5);
                }
                return true;
            }
            
            
        }
    }
    return false;
}


void Monome::startIdle()
{
    Timer::startTimer(100);
}

void Monome::stopIdle()
{
    Timer::stopTimer();
    lightsOff();
}

// PRIVATE ===================================================================================
void Monome::refresh()
{
    OSCMessage lights(monomeData.prefix + "/grid/led/map");
    lights.addInt32(0);
    lights.addInt32(0);
    
    for (int i = 0 ; i < 8; i++)
    {
        lights.addInt32(backgroundMask[i] | foregroundMask[i]);
    }
    monomeSend.send(lights);
}

void Monome::hiResTimerCallback()
{
    refresh();
}


void Monome::timerCallback()
{
//    static int counter = 0;
//    
//    displayNumber(counter);
//    
//    counter++;
//    
//    if (counter> 99) {
//        counter = 0;
//    }
    
    //ringTick();
    idleTick();
}

void Monome::idleTick()
{
    static int bitmaskArray[8] = {248, 124, 62, 31, 143, 199, 227, 241};
    static int callCount = 0;
    static bool directionflag = true;
    
    OSCMessage lights(monomeData.prefix + "/grid/led/map");
    lights.addInt32(0);
    lights.addInt32(0);
    
    for (int i = 0 ; i < 8; i++)
    {
        lights.addInt32(bitmaskArray[i]);
        if (directionflag)
        {
            bitmaskArray[i] = bitRotateRight(bitmaskArray[i], 1, 8);
        }
        else
        {
            bitmaskArray[i] = bitRotateLeft(bitmaskArray[i], 1, 8);
        }
        backgroundMask[i] = bitmaskArray[i];

    }
    
    
    
    if (directionflag)
    {
        callCount++;
    }
    else
    {
        callCount--;
    }
    
    if (callCount == 100)
    {
        directionflag = false;
    }
    else if (callCount == 0)
    {
        directionflag = true;
    }
    
}

void Monome::ringTick()
{
    static int counter = 0;
    
    switch (counter) {
        case 0:
            setMask(RingMasks::ring1);
            break;
        case 1:
            setMask(RingMasks::ring2);
            break;
        case 2:
            setMask(RingMasks::ring3);
            break;
        case 3:
            setMask(RingMasks::ring4);
            break;
        default:
            counter = 0;
            setMask(RingMasks::ring1);
            break;
    }
    counter++;
}


UInt8 Monome::bitRotateRight(UInt8 input, int shift, int intLength)
{
    uint16 temp = (input << shift) | (input >> (intLength - shift));
    return temp;
    
}

UInt8 Monome::bitRotateLeft(UInt8 input, int shift, int intLength)
{
    uint16 temp = (input >> shift) | (input << (intLength - shift));
    return temp;
    
}
