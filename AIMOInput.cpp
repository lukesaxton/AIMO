//
//  AIMOInput.cpp
//  AIMO
//
//  Created by Luke Saxton on 18/01/2017.
//
//

#include "AIMOInput.hpp"


AIMOInput::AIMOInput()
{
    
}

AIMOInput::~AIMOInput()
{
    
}

String AIMOInput::getKeyMapping(const int forKey)
{
    if (forKey >= 0 && forKey < 128)
    {
        return keyMappings[forKey];
    }
    else
    {
        jassert(false);
        //Key out of range of midi standard
        return String();
    }
}
String AIMOInput::getControlMapping(const int forControl)
{
    if (forControl >= 0 && forControl < 128)
    {
        return keyMappings[forControl];
    }
    else
    {
        jassert(false);
        //Control out of range of midi standard
        return String();
    }
}

String AIMOInput::getProgramMapping(const int forProgram)
{
    if (forProgram >= 0 && forProgram < 128)
    {
        return keyMappings[forProgram];
    }
    else
    {
        jassert(false);
        //Control out of range of midi standard
        return String();
    }
}

void AIMOInput::setKeyMapping(const String newMapping, const int forKey)
{
    if (forKey >= 0 && forKey < 128)
    {
        keyMappings[forKey] = newMapping;
    }
}

void AIMOInput::setControlMapping(const String newMapping, const int forControl)
{
    if (forControl >= 0 && forControl < 128)
    {
        controlMappings[forControl] = newMapping;
    }

}

void AIMOInput::setProgramyMapping(const String newMapping, const int forProgram)
{
    if (forProgram >= 0 && forProgram < 128)
    {
        programMappings[forProgram] = newMapping;
    }

}
