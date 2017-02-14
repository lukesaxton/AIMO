//
//  AIMOInput.hpp
//  AIMO
//
//  Created by Luke Saxton on 18/01/2017.
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

#ifndef AIMOInput_hpp
#define AIMOInput_hpp

#include "../JuceLibraryCode/JuceHeader.h"

class AIMOInput{
public:
    AIMOInput();
    ~AIMOInput();
    
    String getKeyMapping(const int forKey);
    String getControlMapping(const int forControl);
    String getProgramMapping(const int forProgram);
    
    void setKeyMapping(const String newMapping, const int forKey);
    void setControlMapping(const String newMapping, const int forControl);
    void setProgramMapping(const String newMapping, const int forProgram);

    
private:
    String keyMappings[127];
    String controlMappings[127];
    String programMappings[127];
};

#endif /* AIMOInput_hpp */
