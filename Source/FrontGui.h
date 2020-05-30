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
#include "CustomSlider.h"

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
    
    CustomSlider qValSlider;
    CustomSlider attackSlider;
    CustomSlider decaySlider;
    CustomSlider sustainSlider;
    CustomSlider releaseSlider;
    Label attackLabel;
    Label decayLabel;
    Label sustainLabel;
    Label releaseLabel;
    Label purityLabel;
    Font inconsolata;
    
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> qVal;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> attackVal;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> decayVal;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> sustainVal;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> releaseVal;
    
    SubtractiveSynthPlugInAudioProcessor& processor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FrontGui)
};
