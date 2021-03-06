/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
SubtractiveSynthPlugInAudioProcessor::SubtractiveSynthPlugInAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
    tree(*this, nullptr, "PARAMETERS",
         {   std::make_unique<AudioParameterFloat>("attack-value", "Attack", NormalisableRange<float>(0.00001f, 20.0f, 0.00001f, 0.23f), 0.1f),
        std::make_unique<AudioParameterFloat>("decay-value", "Decay", NormalisableRange<float>(0.00001f, 60.0f, 0.00001f, 0.2f), 0.8f),
        std::make_unique<AudioParameterFloat>("sustain-value", "Sustain", NormalisableRange<float>(0.0f, 100.0f, 1.0f, 1.0f), 80.0f),
        std::make_unique<AudioParameterFloat>("release-value", "Release", NormalisableRange<float>(0.00001f, 60.0f, 0.00001f, 0.2f), 0.1f),
        std::make_unique<AudioParameterFloat>("q-value", "qVal", NormalisableRange<float>(0.0f, 100.0f, 1.0f, 1.0f), 80.0f),
        std::make_unique<AudioParameterFloat>("volume-value", "Volume", NormalisableRange<float>(0.0f, 100.0f, 0.0f, 1.0f), 50.0f),
        std::make_unique<AudioParameterFloat>("garbage-value", "Garbage", NormalisableRange<float>(0.0f, 100.0f, 0.01f, 1.0f), 0.0f),
        std::make_unique<AudioParameterFloat>("garbage-wet-dry", "Garbage Wet/Dry", NormalisableRange<float>(0.0f, 100.0f, 1.0f, 1.0f), 0.0f),
        std::make_unique<AudioParameterFloat>("sine-val", "Sine Val", NormalisableRange<float>(0.0f, 100.0f, 0.0f, 1.0f), 0.0f),
        std::make_unique<AudioParameterFloat>("tri-val", "Triange Val", NormalisableRange<float>(0.0f, 100.0f, 0.0f, 1.0f), 0.0f),
        std::make_unique<AudioParameterFloat>("squ-val", "Square Val", NormalisableRange<float>(0.0f, 100.0f, 0.0f, 1.0f), 0.0f),
        std::make_unique<AudioParameterFloat>("saw-val", "Sawtooth Val", NormalisableRange<float>(0.0f, 100.0f, 0.0f, 1.0f), 0.0f),
        std::make_unique<AudioParameterFloat>("lfo-amount", "Pitch LFO Amount", NormalisableRange<float>(0.0f, 100.0f, 0.0f, 1.0f), 0.0f),
        std::make_unique<AudioParameterFloat>("lfo-speed", "Pitch LFO Speed", NormalisableRange<float>(1.0f, 30.0f, 0.0f, 0.50f), 1.0f), //1Hz - 30Hz
        std::make_unique<AudioParameterFloat>("env-filter", "Envelope Filter Amount", NormalisableRange<float>(0.0f, 100.0f, 0.0f, 1.0f), 0.0f),
        std::make_unique<AudioParameterFloat>("lowpass-amount", "Lowpass Filter Amount", NormalisableRange<float>(0.0f, 100.0f, 0.0f, 1.0f), 0.0f),
        std::make_unique<AudioParameterFloat>("lowpass-freq", "Lowpass Filter Frequency", NormalisableRange<float>(100.0f, 20000.0f, 0.0f, 0.10f), 20000.0f)
    })

#endif
{
    activeVoiceCounter = 0;
}

SubtractiveSynthPlugInAudioProcessor::~SubtractiveSynthPlugInAudioProcessor()
{
}

//==============================================================================
const String SubtractiveSynthPlugInAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SubtractiveSynthPlugInAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SubtractiveSynthPlugInAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SubtractiveSynthPlugInAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SubtractiveSynthPlugInAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SubtractiveSynthPlugInAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SubtractiveSynthPlugInAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SubtractiveSynthPlugInAudioProcessor::setCurrentProgram (int index)
{
}

const String SubtractiveSynthPlugInAudioProcessor::getProgramName (int index)
{
    return {};
}

void SubtractiveSynthPlugInAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void SubtractiveSynthPlugInAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    prevSampleRate = sampleRate;
    synth.clearVoices();
    
    
    for (int i = 0; i < 36; i++)
    {
        synth.addVoice(new SynthVoice(samplesPerBlock));
    }
    synth.clearSounds();
    synth.addSound(new SynthSound());
    synth.setCurrentPlaybackSampleRate (sampleRate);
}

void SubtractiveSynthPlugInAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SubtractiveSynthPlugInAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SubtractiveSynthPlugInAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    buffer.clear();
    activeVoiceCounter = 0;

        for (int i = 0; i < synth.getNumVoices(); i++)
    {
        //if myVoice sucessfully casts as a SynthVoice*, get the voice and set the params
        if ((voice = dynamic_cast<SynthVoice*>(synth.getVoice(i))))
        {
            voice->getEnvelopeParams(tree.getRawParameterValue("attack-value"),
                                       tree.getRawParameterValue("decay-value"),
                                       tree.getRawParameterValue("sustain-value"),
                                       tree.getRawParameterValue("release-value"));
            voice->updateQVal(tree.getRawParameterValue("q-value"));
            voice->updateGarbage(tree.getRawParameterValue("garbage-value"),
                                 tree.getRawParameterValue("garbage-wet-dry"));
            voice->updateVolume(tree.getRawParameterValue("volume-value"));
            voice->updateWaveforms(tree.getRawParameterValue("sine-val"),
                                   tree.getRawParameterValue("tri-val"),
                                   tree.getRawParameterValue("squ-val"),
                                   tree.getRawParameterValue("saw-val"));
            voice->getLfoAndFilters(tree.getRawParameterValue("lfo-amount"),
                                    tree.getRawParameterValue("lfo-speed"),
                                    tree.getRawParameterValue("lowpass-amount"),
                                    tree.getRawParameterValue("lowpass-freq"),
                                    tree.getRawParameterValue("env-filter"));
            if(voice->isVoiceActive())
            {
                activeVoiceCounter++;
            }
        }
    }
    
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool SubtractiveSynthPlugInAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SubtractiveSynthPlugInAudioProcessor::createEditor()
{
    return new SubtractiveSynthPlugInAudioProcessorEditor (*this);
}

//==============================================================================
void SubtractiveSynthPlugInAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SubtractiveSynthPlugInAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

int SubtractiveSynthPlugInAudioProcessor::getActiveVoiceCounter()
{
    return activeVoiceCounter;
}


//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SubtractiveSynthPlugInAudioProcessor();
}
