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
#include "WavetableOscillator.h"

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
    void getLfoAndFilters(std::atomic<float>* lfoAmount, std::atomic<float>* lfoSpeed, std::atomic<float>* loPassAmt, std::atomic<float>* loPassFreq, std::atomic<float>* envFilt);
    void processLowPassFilter(juce::dsp::AudioBlock<float>& block);
    double noteHz(int midiNoteNumber, double centsOffset);

    AudioBuffer<float> sineTable;
    AudioBuffer<float> triTable;
    AudioBuffer<float> squTable;
    AudioBuffer<float> sawTable;
    
    std::unique_ptr<WavetableOscillator> sineOsc;
    std::unique_ptr<WavetableOscillator> triOsc;
    std::unique_ptr<WavetableOscillator> squOsc;
    std::unique_ptr<WavetableOscillator> sawOsc;
    std::unique_ptr<WavetableOscillator> pitchLfo;
    
    
private:
    void wavetableHelper(const int harmonics[], const float harmonicWeights[], AudioBuffer<float>& table,  const int numHarms);
    void setOscFreq();
    void processLfo();
    
    const int numHarmonics = 8;
    const int tableSize = 256;
    double level = 0.0;
    double lastSample[2];
    bool isOn = false;
    Random random;
    double frequency;
    juce::dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> bpFilter;
    juce::dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> bpFilter2;
    juce::dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> lowPassFilter;
    dsp::ProcessSpec spec;
    juce::HeapBlock<char> heapBlock;
    juce::dsp::AudioBlock<float> tempBlock;
    juce::dsp::AudioBlock<float> lpTempBlock;
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
    float sineSample;
    float triSample;
    float squSample;
    float sawSample;
    float pitchLfoAmount;
    float pitchLfoSpeed;
    float envFilterAmount;
    float lowPassFreq;
    float lowPassAmount;
    float currPitchBendOffset;

};
