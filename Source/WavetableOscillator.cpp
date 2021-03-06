/*
  ==============================================================================

    WavetableWavetableOscillator.cpp
    Created: 6 Jun 2020 10:09:47pm
    Author:  Chris Robinson

  ==============================================================================
*/

#include "WavetableOscillator.h"

//==============================================================================
WavetableOscillator::WavetableOscillator(AudioBuffer<float>& wavetableToUse)
        : wavetable (wavetableToUse),
          tableSize (wavetable.getNumSamples() - 1)
{
    jassert (wavetable.getNumChannels() == 1);
}

void WavetableOscillator::setFrequency (float frequency, float sampleRate)
{
    auto tableSizeOverSampleRate = tableSize / sampleRate;
    tableDelta = frequency * tableSizeOverSampleRate;
}

float WavetableOscillator::getNextSample()  //todo: inline
{
    auto index0 = (unsigned int) currentIndex;
    auto index1 = index0 + 1;

    auto frac = currentIndex - (float) index0;

    auto* table = wavetable.getReadPointer (0);
    auto value0 = table[index0];
    auto value1 = table[index1];

    auto currentSample = value0 + frac * (value1 - value0);

    if ((currentIndex += tableDelta) > tableSize)
        currentIndex -= tableSize;

    return currentSample;
}

