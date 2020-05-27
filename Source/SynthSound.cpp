/*
  ==============================================================================

    SynthSound.cpp
    Created: 22 May 2020 8:28:05pm
    Author:  Chris Robinson

  ==============================================================================
*/

#include "SynthSound.h"


//==============================================================================
SynthSound::SynthSound(){}

bool SynthSound::appliesToNote( int )    { return true; }
bool SynthSound::appliesToChannel( int ) { return true; }

