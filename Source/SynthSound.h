/*
  ==============================================================================

    SynthSound.h
    Created: 22 May 2020 8:28:05pm
    Author:  Chris Robinson

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>


//==============================================================================
struct SynthSound   : public SynthesiserSound
{
public:
    SynthSound();
    
    bool appliesToNote    (int) override;
    bool appliesToChannel (int) override;
};
