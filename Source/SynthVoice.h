/*
  ==============================================================================

    SynthVoice.h
    Created: 22 May 2020 8:25:58pm
    Author:  Chris Robinson

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "SynthSound.h"

//==============================================================================
class SynthVoice   : public SynthesiserVoice

{
public:

    SynthVoice(int samplesPerBlockExpected );
    bool canPlaySound (SynthesiserSound* sound) override;
    void startNote (int midiNoteNumber, float velocity,
                    SynthesiserSound*, int /*currentPitchWheelPosition*/) override;
    void stopNote (float /*velocity*/, bool allowTailOff) override;
    void pitchWheelMoved (int) override;
    void controllerMoved (int, int) override;
    void renderNextBlock (AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override;
    void updateFilter();
    void setPitchBend(int pitchWheelPos);
    float pitchBendCents();
    void updateQVal(std::atomic<float>* treeQValue);
    void getEnvelopeParams(std::atomic<float>* attack, std::atomic<float>* decay, std::atomic<float>* sustain, std::atomic<float>* release);
    void setADSRSampleRate(double sr);
    void endNote();
    dsp::ProcessSpec spec;
    
private:
    double level = 0.0, tailOff = 0.0;
    double lastSample[2];
    bool isOn = false;
    Random random;
    double frequency;
    AudioSampleBuffer bufferBuffer;
    juce::dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> bpFilter;
    juce::dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> bpFilter2;
    int samplesPerBlock;
    float ogFreq;
    float freqRatio;
    float pitchBend = 0.0f;
    float pitchBendUpSemitones = 2.0f;
    float pitchBendDownSemitones = 2.0f;
    float qVal;
    int noteNumber;
    ADSR adsr;
    ADSR::Parameters adsrParams;
    double adsrSample;
};
