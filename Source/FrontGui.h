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
    void setUpSlider(CustomSlider& slider, Label& label, String text, int boxSize, int xplacement, int yplacement);
    
private:
    
    CustomSlider qValSlider;
    CustomSlider attackSlider;
    CustomSlider decaySlider;
    CustomSlider sustainSlider;
    CustomSlider releaseSlider;
    CustomSlider polySlider;
    CustomSlider volumeSlider;
    CustomSlider garbageSlider;
    CustomSlider garbageWetDrySlider;
    CustomSlider sineSlider;
    CustomSlider triSlider;
    CustomSlider squSlider;
    CustomSlider sawSlider;
    Label attackLabel;
    Label decayLabel;
    Label sustainLabel;
    Label releaseLabel;
    Label purityLabel;
    Label polyLabel;
    Label volumeLabel;
    Label garbageLabel;
    Label garbageWetDryLabel;
    Label sineLabel;
    Label triLabel;
    Label squLabel;
    Label sawLabel;
    Font inconsolata;
    
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> qVal;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> attackVal;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> decayVal;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> sustainVal;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> releaseVal;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> polyphonyVal;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> volumeVal;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> garbageVal;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> garbageWetDryVal;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> sineVal;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> triVal;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> squVal;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> sawVal;
    
    SubtractiveSynthPlugInAudioProcessor& processor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FrontGui)
};
