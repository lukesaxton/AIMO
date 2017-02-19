//
//  AIMOInputManager.hpp
//  AIMO
//
//  Created by Luke Saxton on 14/02/2017.
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


#ifndef AIMOInputManager_hpp
#define AIMOInputManager_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "RoutableMidiInput.hpp"
#include "KTMHandController.hpp"
#include "MonomeInput.hpp"

class AIMOInputManager : public Component
{
public:
    AIMOInputManager();
    ~AIMOInputManager();
    
    void paint (Graphics& g) override;
    void resized() override;
    
    
private:
    OwnedArray<RoutableMidiInput> midiInputs;
    KTMHandController kTMController;
    MonomeInput monomeInput;
    
    };

#endif /* AIMOInputManager_hpp */
