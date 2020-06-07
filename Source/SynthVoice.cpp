/*
  ==============================================================================

    SynthVoice.cpp
    Created: 22 May 2020 8:25:58pm
    Author:  Chris Robinson

  ==============================================================================
*/

#include "SynthVoice.h"

//==============================================================================
SynthVoice::SynthVoice( int samplesPerBlockExpected )
    :bpFilter(dsp::IIR::Coefficients<float>::makeBandPass (getSampleRate(), 20000.0f, 0.0001f)),
    bpFilter2(dsp::IIR::Coefficients<float>::makeBandPass (getSampleRate(), 20000.0f, 0.0001f)),
    freqRatio(MidiMessage::getMidiNoteInHertz(16)/MidiMessage::getMidiNoteInHertz(18)), //18 because 16+PITCHBEND
    qVal(0.0001f)
{
    spec.sampleRate = getSampleRate();
    spec.maximumBlockSize = samplesPerBlockExpected;
    spec.numChannels = 2;
        
    samplesPerBlock = samplesPerBlockExpected;
    dsp::AudioBlock<float> block;
        
    lastVolume = 1.0f;
    
    createWavetables();
    
    sineOsc = std::make_unique<Oscillator>(sineTable);
    triOsc = std::make_unique<Oscillator>(triTable);
    squOsc = std::make_unique<Oscillator>(squTable);
    sawOsc = std::make_unique<Oscillator>(sawTable);
}

bool SynthVoice::canPlaySound (SynthesiserSound* sound){
        return dynamic_cast<SynthSound*> (sound) != nullptr;
}

void SynthVoice::startNote (int midiNoteNumber, float velocity,
                            SynthesiserSound*, int /*currentPitchWheelPosition*/) {
    
    bpFilter.reset();
    bpFilter2.reset();
    noteNumber = midiNoteNumber;
    
    adsr.noteOn();
    level = 0.5;
    isOn = true;
    
    frequency = ogFreq = MidiMessage::getMidiNoteInHertz (midiNoteNumber);
    
    updateFilter();
    tempBlock = juce::dsp::AudioBlock<float> (heapBlock, spec.numChannels, spec.maximumBlockSize);
    bpFilter.prepare(spec);
    bpFilter2.prepare(spec);
    
    sineOsc->setFrequency(frequency, getSampleRate());
    triOsc->setFrequency(frequency, getSampleRate());
    squOsc->setFrequency(frequency, getSampleRate());
    sawOsc->setFrequency(frequency, getSampleRate());
    
}

void SynthVoice::stopNote (float /*velocity*/, bool allowTailOff){
    if (!allowTailOff)
        adsrParams.release = 0.05f;
    adsr.noteOff();
}

void SynthVoice::endNote(){
    clearCurrentNote();
    isOn = false;
    bpFilter.reset();
    bpFilter2.reset();
}

void SynthVoice::setPitchBend(int pitchWheelPos)
{
    if (pitchWheelPos > 8192)
    {
        // shifting up
        pitchBend = float(pitchWheelPos - 8192) / (16383 - 8192);
    }
    else
    {
        // shifting down
        pitchBend = float(8192 - pitchWheelPos) / -8192;    // negative number
    }
}

float SynthVoice::pitchBendCents()
{
    if (pitchBend >= 0.0f)
    {
        // shifting up
        return pitchBend * pitchBendUpSemitones * 100;
    }
    else
    {
        // shifting down
        return pitchBend * pitchBendDownSemitones * 100;
    }
}

static double noteHz(int midiNoteNumber, double centsOffset)
{
    double hertz = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    hertz *= std::pow(2.0, centsOffset / 1200);
    return hertz;
}


void SynthVoice::pitchWheelMoved (int newPitchWheelValue){
    
    setPitchBend(newPitchWheelValue);
    frequency = noteHz(noteNumber, pitchBendCents());
    
}

void SynthVoice::controllerMoved (int, int){}

void SynthVoice::updateFilter(){

    if( qVal <= 0) qVal = 0.0001;
    
    *bpFilter.state = *dsp::IIR::Coefficients<float>::makeBandPass (getSampleRate(), frequency, qVal);
    *bpFilter2.state = *dsp::IIR::Coefficients<float>::makeBandPass (getSampleRate(), frequency, qVal);

}
void SynthVoice::renderNextBlock (AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    if( isOn )
    {
        auto block = tempBlock.getSubBlock (0, (size_t) numSamples);
        garboBuffer = AudioBuffer<float>( outputBuffer.getNumChannels(), outputBuffer.getNumSamples());
        block.clear();
        updateFilter();
        
        adsr.setParameters(adsrParams);

        for (auto sample = 0; sample < numSamples; ++sample)  //load up with noise values
        {
            auto currentSample = level * (-0.25f + (0.5f * (float) random.nextFloat()));
                
            for (auto i = 0; i < outputBuffer.getNumChannels(); ++i )
            {
                block.addSample(i, sample, currentSample);
            }
        }
        
        bpFilter.process(dsp::ProcessContextReplacing<float> (block));  //process the block
        bpFilter2.process(dsp::ProcessContextReplacing<float> (block));
        
        for (auto sample = 0; sample < numSamples; ++sample)        //add ADSR
        {
            if(lastVolume != targetVolume) rampVolume();
            adsrSample = adsr.getNextSample();
            if (adsrSample == 0)
            {
                endNote();
//                return;
            }
            for (auto i = 0; i < outputBuffer.getNumChannels(); ++i )
            {
                block.setSample(i, sample, block.getSample(i, sample) * (1.0f + qVal) * adsrSample * lastVolume);
            }
        }
        juce::dsp::AudioBlock<float> (outputBuffer)
            .getSubBlock ((size_t) startSample, (size_t) numSamples)
            .add (tempBlock);
        
        if (garboVal > 0.0f && garboWetDry > 0.0f) //garbage value sound
        {
            processGarbo(outputBuffer);
        }
    }
}

void SynthVoice::updateQVal(std::atomic<float>* treeQValue){
    qVal = pow(2, *treeQValue * 0.05f);
}

void SynthVoice::updateGarbage(std::atomic<float>* treeGarboValue, std::atomic<float>* treeGarboWetDryValue){
    garboVal = *treeGarboValue * 0.01f;
    garboWetDry = *treeGarboWetDryValue * 0.01f;
}

void SynthVoice::updateVolume(std::atomic<float>* treeVolValue)
{
    targetVolume = *treeVolValue * 0.02f;
}

void SynthVoice::getEnvelopeParams(std::atomic<float>* attack, std::atomic<float>* decay, std::atomic<float>* sustain, std::atomic<float>* release)
{
    adsrParams.attack = *attack;
    adsrParams.decay = *decay;
    adsrParams.sustain = *sustain * 0.01f;
    adsrParams.release = *release;
}

void SynthVoice::updateWaveforms(std::atomic<float>* sine, std::atomic<float>* tri, std::atomic<float>* squ, std::atomic<float>* saw)
{
    sineVal = *sine;
    triVal = *tri;
    squVal = *squ;
    sawVal = *saw;
}

void SynthVoice::setADSRSampleRate(double sr)
{
    adsr.setSampleRate(sr);
}

void SynthVoice::rampVolume()
{
    float slopeConstant = 0.001f;
    if( lastVolume < (targetVolume - slopeConstant))
    {
        lastVolume += slopeConstant;
    }
    else if( lastVolume > (targetVolume + slopeConstant))
    {
        lastVolume -= slopeConstant;
    }
    else
    {
        lastVolume = targetVolume;
    }
}

void SynthVoice::processGarbo(AudioBuffer<float>& outputBuffer)
{
    for (auto i = 0; i < outputBuffer.getNumChannels(); ++i)
    {
        for (auto sample = 0; sample < garboBuffer.getNumSamples(); ++sample)
        {
            float sampleVal = garboBuffer.getSample(i, sample);
            if( sampleVal > 0.01f ) sampleVal = 0.01f;
            if( sampleVal < -0.01f ) sampleVal = -0.01f;
            garboBuffer.setSample(i, sample, sampleVal);
            int garboDemon = (int) garboBuffer.getNumSamples() * (1.0f - garboVal);
            if( garboDemon == 0 ) garboDemon++;
            int garboRatio = sample % garboDemon;
            if (garboRatio == 0)
            {
                auto newSampleVal = (sampleVal * (garboWetDry)) + (outputBuffer.getSample(i, sample) * (1.0f - garboWetDry));
                outputBuffer.setSample(i, sample, newSampleVal);
            }
        }
    }
}

void SynthVoice::createWavetables()
{
    sineTable.setSize(1, tableSize + 1); //1 << 7 plus one for wrapping
    triTable.setSize(1, tableSize + 1);
    squTable.setSize(1, tableSize + 1);
    sawTable.setSize(1, tableSize + 1);
    sineTable.clear();
    triTable.clear();
    squTable.clear();
    sawTable.clear();
    
    int oddHarms[] =  { 1, 3, 5, 7, 9, 11, 13, 15};
    int everyHarm[] = { 1, 2, 3, 4, 5, 6, 7, 8};
    float sineWeights [] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    float squareWeights[] = { 1.0f, 0.33f, 0.2f, 0.14285f, 0.111f, 0.0909091f, 0.0769231f, 0.0666667f};
    float sawWeights[] = { 1.0f, 0.5f, 0.33f, 0.25f, 0.02f, 0.166667f, 0.14285f, 0.125f};
    float triangleWeights[] = { 1.0f, 0.111f, 0.04f, 0.02040816f, 0.012345679f, 0.00826446f, 0.00591716f, 0.004444444f};

    wavetableHelper(oddHarms, sineWeights, sineTable);
    wavetableHelper(oddHarms, triangleWeights, triTable);
    wavetableHelper(oddHarms, squareWeights, squTable);
    wavetableHelper(everyHarm, sawWeights, sawTable);
}

void SynthVoice::wavetableHelper(const int harmonics[], const float harmonicWeights[], AudioBuffer<float>& table)
{
    auto* samples = table.getWritePointer (0);
    
    for( int harmonic = 0; harmonic < numHarmonics; harmonic++ )
    {
        auto angleDelta = MathConstants<double>::twoPi / (double) (tableSize - 1) * harmonics[harmonic];
        auto currentAngle = 0.0;
        
        for (int i = 0; i < tableSize; i++)
        {
            auto sample = std::sin (currentAngle);
            samples[i] += (float) sample * harmonicWeights[harmonic];
            currentAngle += angleDelta;
        }
    }
    samples[tableSize] = samples[0];
}
    
