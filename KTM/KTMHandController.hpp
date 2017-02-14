//
//  KTMHandController.hpp
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

#ifndef KTMHandController_hpp
#define KTMHandController_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "AIMOInput.hpp"

#define NUM_KTM_BUTTONS 12



class KTMHandController : public OSCReceiver::Listener<>,
                          public AIMOInput
{
public:
    KTMHandController();
    ~KTMHandController();
    void oscMessageReceived (const OSCMessage& message);
    
    bool connect();
    
private:
    OSCReceiver controllerReceive;
    int stateGrid[NUM_KTM_BUTTONS];
};

#endif /* KTMHandController_hpp */
