/*
  ==============================================================================

    frontGui.h
    Created: 26 May 2020 4:02:34pm
    Author:  Chris Robinson

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class FrontGui    : public Component
{
public:
    FrontGui(SubtractiveSynthPlugInAudioProcessor&);
    ~FrontGui();

    void paint (Graphics&) override;
    void resized() override;

private:
    
    Slider qValSlider;
    Slider attackSlider;
    Slider decaySlider;
    Slider sustainSlider;
    Slider releaseSlider;
    Label attackLabel;
    Label decayLabel;
    Label sustainLabel;
    Label releaseLabel;
    Label purityLabel;
    
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> qVal;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> attackVal;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> decayVal;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> sustainVal;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> releaseVal;
    
    SubtractiveSynthPlugInAudioProcessor& processor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FrontGui)
};
