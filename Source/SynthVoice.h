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
#include "Oscillator.h"

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
    void updateGarbage(std::atomic<float>* treeGarboValue,std::atomic<float>* treeGarboWetDryValue);
    void updateVolume(std::atomic<float>* treeVolValue);
    void getEnvelopeParams(std::atomic<float>* attack, std::atomic<float>* decay, std::atomic<float>* sustain, std::atomic<float>* release);
    void setADSRSampleRate(double sr);
    void endNote();
    void rampVolume();
    void processGarbo(AudioBuffer<float>& outputBuffer);
    void updateWaveforms(std::atomic<float>* sine, std::atomic<float>* tri, std::atomic<float>* squ, std::atomic<float>* saw);
    void createWavetables();

    AudioBuffer<float> sineTable;
    AudioBuffer<float> triTable;
    AudioBuffer<float> squTable;
    AudioBuffer<float> sawTable;
    
    std::unique_ptr<Oscillator> sineOsc;
    std::unique_ptr<Oscillator> triOsc;
    std::unique_ptr<Oscillator> squOsc;
    std::unique_ptr<Oscillator> sawOsc;
    
    
private:
    void wavetableHelper(const int harmonics[], const float harmonicWeights[], AudioBuffer<float>& table);
    
    const int numHarmonics = 8;
    const int tableSize = 128;
    double level = 0.0;
    double lastSample[2];
    bool isOn = false;
    Random random;
    double frequency;
    juce::dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> bpFilter;
    juce::dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> bpFilter2;
    dsp::ProcessSpec spec;
    juce::HeapBlock<char> heapBlock;
    juce::dsp::AudioBlock<float> tempBlock;
    juce::AudioBuffer<float> garboBuffer;
    int samplesPerBlock;
    float ogFreq;
    float freqRatio;
    float pitchBend = 0.0f;
    float pitchBendUpSemitones = 2.0f;
    float pitchBendDownSemitones = 2.0f;
    float qVal;
    float garboVal;
    float garboWetDry;
    float targetVolume;
    float lastVolume;
    float sineVal;
    float triVal;
    float squVal;
    float sawVal;
    int noteNumber;
    ADSR adsr;
    ADSR::Parameters adsrParams;
    double adsrSample;
    

};
