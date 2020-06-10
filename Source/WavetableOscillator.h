/*
  ==============================================================================

    WavetableOscillator.h
    Created: 6 Jun 2020 10:09:47pm
    Author:  Chris Robinson

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
class WavetableOscillator
{
public:
    
    WavetableOscillator(AudioBuffer<float>& wavetableToUse);
    void setFrequency (float frequency, float sampleRate);
    float getNextSample();
    
private:
    const AudioSampleBuffer& wavetable;
    const int tableSize;
    float currentIndex = 0.0f, tableDelta = 0.0f;
};
