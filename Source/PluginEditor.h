/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "SynthVoice.h"
#include "SynthSound.h"
#include "AnimatedBg.h"
#include "FrontGui.h"

//==============================================================================
/**
*/
class SubtractiveSynthPlugInAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    SubtractiveSynthPlugInAudioProcessorEditor (SubtractiveSynthPlugInAudioProcessor&);
    ~SubtractiveSynthPlugInAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    
    
    AnimatedBg animatedBg;
    FrontGui frontGui;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SubtractiveSynthPlugInAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SubtractiveSynthPlugInAudioProcessorEditor)
};
