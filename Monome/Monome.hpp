//
//  Monome.hpp
//  AI:MO
//
//  Created by Luke Saxton on 14/01/2017.
//
//

#ifndef Monome_hpp
#define Monome_hpp



#include "../JuceLibraryCode/JuceHeader.h"
#include "AIMOInput.hpp"

namespace NumberMasks{
    static const int oneL[8] {0, 0, 0, 2, 2, 2, 2, 2};
    static const int twoL[8] {0,0,0, 7, 4, 7, 1, 7};
    static const int threeL[8] {0,0,0, 7, 4, 7, 4, 7};
    static const int fourL[8] {0,0,0, 5, 5, 7, 4, 4};
    static const int fiveL[8] {0,0,0, 7, 1,  7, 4, 7};
    static const int sixL[8] {0,0,0, 7, 1, 7, 5, 7};
    static const int sevenL[8] {0,0,0, 7, 4, 4, 4, 4};
    static const int eightL[8] {0,0,0, 7, 5, 7, 5, 7};
    static const int nineL[8] {0, 0, 0, 7, 5, 7, 4, 4};
    static const int zeroL[8] {0,0,0, 7, 5, 5, 5, 7};

// Seven high Numbers
//    static const int oneL[8] {0, 6, 6, 6, 6, 6, 6, 6};
//    static const int twoL[8] {0, 15, 8, 8, 15, 1, 1, 15};
//    static const int threeL[8] {0, 15, 8, 8, 15, 8, 8, 15};
//    static const int fourL[8] {0, 9, 9, 9, 15, 8, 8, 8};
//    static const int fiveL[8] {0, 15, 1, 1, 15, 8, 8, 15};
//    static const int sixL[8] {0, 15, 1, 1, 15, 9, 9, 15};
//    static const int sevenL[8] {0, 15, 8, 8, 8, 8, 8, 8};
//    static const int eightL[8] {0, 15, 9, 9, 15, 9, 9, 15};
//    static const int nineL[8] {0, 15, 9, 9, 15, 8, 8, 8};
//    static const int zeroL[8] {0, 15, 9, 9, 9, 9, 9, 15};
//    
//    static const int oneR[8] {0, 96, 96, 96, 96, 96, 96, 96};
//    static const int twoR[8] {0, 240, 128, 128, 240, 16, 16, 240};
//    static const int threeR[8] {0, 240, 128, 128, 240, 128, 128, 240};
//    static const int fourR[8] {0, 144, 144, 144, 240, 128, 128, 128};
//    static const int fiveR[8] {0, 240, 16, 16, 240, 128, 128, 240};
//    static const int sixR[8] {0, 240, 16, 16, 240, 144, 144, 240};
//    static const int sevenR[8] {0, 240, 128, 128, 128, 128, 128, 128};
//    static const int eightR[8] {0, 240, 144, 144, 240, 144, 144, 240};
//    static const int nineR[8] {0, 240, 144, 144, 240, 128, 128, 128};
//    static const int zeroR[8] {0, 240, 144, 144, 144, 144, 144, 240};
}

namespace RingMasks{
    static const int ring1[8] {255, 129, 129, 129, 129, 129, 129, 255};
    static const int ring2[8] {0, 126, 66, 66, 66, 66, 126, 0};
    static const int ring3[8] {0,0, 60, 36, 36, 60, 0 ,0};
    static const int ring4[8] {0,0,0,24,24,0,0,0,};
}

namespace LetterMasks{
    static const int A[8] {0,0,0,2,5,5,7,5};
    static const int B[8] {0,0,0,3,5,3,5,3};
    static const int C[8] {0,0,0,7,1,1,1,7};
    static const int D[8] {0,0,0,3,5,5,5,3};
    static const int E[8] {0,0,0,7,1,7,1,7};
    static const int F[8] {0,0,0,7,1,7,1,1};
    static const int G[8] {0,0,0,3,5,1,5,7};
    static const int H[8] {0,0,0,5,5,7,5,5};
    static const int I[8] {0,0,0,7,2,2,2,7};
    static const int J[8] {0,0,0,4,4,4,4,7};
    static const int K[8] {0,0,0,5,5,3,5,5};
    static const int L[8] {0,0,0,1,1,1,1,7};
    static const int M[8] {0,0,0,5,7,5,5,5};
    static const int N[8] {0,0,0,7,5,5,5,5};
    static const int O[8] {0,0,0,7,5,5,5,7};
    static const int P[8] {0,0,0,7,5,7,1,1};
    static const int Q[8] {0,0,0,7,5,7,4,4};
    static const int R[8] {0,0,0,3,5,3,5,5};
    static const int S[8] {0,0,0,7,1,7,4,7};
    static const int T[8] {0,0,0,7,2,2,2,2};
    static const int U[8] {0,0,0,5,5,5,5,7};
    static const int V[8] {0,0,0,5,5,5,5,2};
    static const int W[8] {0,0,0,5,5,5,7,5};
    static const int X[8] {0,0,0,5,5,2,5,5};
    static const int Y[8] {0,0,0,5,5,7,2,2};
    static const int Z[8] {0,0,0,7,4,2,1,7};

};

class Monome : public OSCReceiver::Listener<>,
               public Timer,
               public HighResolutionTimer,
               public AIMOInput
{
    
    
public:
    struct MonomeData {
        String ID = "\0";
        String deviceName = "\0";
        int size[2] = {8,8};
        String host = "127.0.0.1";
        int port = 0;
        String prefix = "\0";
        int rotation = 0;
    };
    
    
    
    struct Grid{
        bool s = false;
        bool momentaryS = false;
        uint32 lastPress;
    };
    
    
    Monome();
    
    ~Monome();

    bool connectToDevice(MonomeData deviceInfo);
    
    void oscMessageReceived (const OSCMessage& message) override;
    
    void setIntensity(const int intensity);
    
    void lightsOff();

    void lightsOn();
    
    void setLight(const int x, const int y, const bool state);
    
    void setMask(const int bitmaskArray[8], bool makeForeground = false);
    
    void displayNumber(const int numToDisplay);

    void startIdle();
    void stopIdle();

    
    
private:
    void refresh();
    
    void hiResTimerCallback() override;
    
    void timerCallback() override;
    
    void idleTick();
    
    void ringTick();
    
    void pollConnectedMonome();
    
    UInt8 bitRotateLeft(UInt8 input, int shift, int intLength);

    UInt8 bitRotateRight(UInt8 input, int shift, int intLength);

    
    bool getMaskForNumber(const int number, int* arrayToFill, const bool isLeftNum);
    
    bool getMaskForLetter(const String letter, int* arrayToFill, const bool isLeftChar);

    
    Grid grid[8][8];
    
    MonomeData monomeData;
    
    OSCReceiver monomeReceive;
    OSCSender monomeSend;
    
    bool deviceConnected;
    
    Time time;
    
    int foregroundMask[8] = {0,0,0,0,0,0,0,0};
    int backgroundMask[8] = {0,0,0,0,0,0,0,0};
    
    Array<Array<int>> numberMasks;

    int keysPressed = 0;
    bool numberMode = false;
};

#endif /* Monome_hpp */
