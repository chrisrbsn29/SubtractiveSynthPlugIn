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
    bpFilter.prepare(spec);
    bpFilter2.prepare(spec);
    
}

void SynthVoice::stopNote (float /*velocity*/, bool /*allowTailOff*/){
    
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
    bufferBuffer = AudioBuffer<float>( outputBuffer.getNumChannels(), outputBuffer.getNumSamples());
    bufferBuffer.clear();
    adsr.setParameters(adsrParams);

    if( isOn ){
        int index = numSamples;
        if (tailOff > 0.0) // with tail off
        {
            while (--index >= 0)
            {
  
                auto currentSample = level * tailOff * (-0.25f + (0.5f * (float) random.nextFloat()));
                
                for (auto i = bufferBuffer.getNumChannels(); --i >= 0;){

                    bufferBuffer.addSample(i, startSample, currentSample);
                    
                }
                ++startSample;
            
                tailOff *= 0.999;
            
                if (tailOff <= 0.005)
                {
                    clearCurrentNote();
                    isOn = false;
                    bpFilter.reset();
                    bpFilter2.reset();
                    break;
                }
            }
        }
        else // without tail off
        {
            while (--index >= 0)
            {
                
                auto currentSample = level * (-0.25f + (0.5f * (float) random.nextFloat()));
                
                for (auto i = bufferBuffer.getNumChannels(); --i >= 0;){
                    
                    bufferBuffer.addSample(i, startSample, currentSample);
                    //outputBuffer.addSample(i, startSample, currentSample);

                }
                ++startSample;
            }
        }
        
        dsp::AudioBlock<float> block(bufferBuffer);
        updateFilter();
        bpFilter.process(dsp::ProcessContextReplacing<float> (block));
        bpFilter2.process(dsp::ProcessContextReplacing<float> (block));
        index = numSamples;
        while (--index >= 0){
            adsrSample = adsr.getNextSample();
            for( auto i = outputBuffer.getNumChannels(); --i >= 0;){
    
                outputBuffer.addSample(i, index, bufferBuffer.getSample(i, index) * (1.0f + qVal) * adsrSample);
                if (adsrSample == 0) endNote();
            }
        }
    }
}

void SynthVoice::updateQVal(std::atomic<float>* treeQValue){
    qVal = pow(2, *treeQValue * 0.05f);
}

void SynthVoice::getEnvelopeParams(std::atomic<float>* attack, std::atomic<float>* decay, std::atomic<float>* sustain, std::atomic<float>* release)
    {
        adsrParams.attack = *attack;
        adsrParams.decay = *decay;
        adsrParams.sustain = *sustain * 0.01f;
        adsrParams.release = *release;
    }

void SynthVoice::setADSRSampleRate(double sr){
    adsr.setSampleRate(sr);
}
