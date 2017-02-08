//
//  AIMOInput.hpp
//  AIMO
//
//  Created by Luke Saxton on 18/01/2017.
//
//

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
