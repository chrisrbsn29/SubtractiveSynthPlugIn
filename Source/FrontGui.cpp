/*
  ==============================================================================

    frontGui.cpp
    Created: 26 May 2020 4:02:34pm
    Author:  Chris Robinson

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FrontGui.h"

//==============================================================================
FrontGui::FrontGui(SubtractiveSynthPlugInAudioProcessor& p) : qValSlider(CustomSlider::ValueType::Percent), attackSlider(CustomSlider::ValueType::Seconds), decaySlider(CustomSlider::ValueType::Seconds), sustainSlider(CustomSlider::ValueType::Percent), releaseSlider(CustomSlider::ValueType::Seconds),
    volumeSlider(CustomSlider::ValueType::Percent),
    garbageSlider(CustomSlider::ValueType::Neutral),
    garbageWetDrySlider(CustomSlider::ValueType::Percent),
    sineSlider(CustomSlider::ValueType::Percent),
    triSlider(CustomSlider::ValueType::Percent),
    squSlider(CustomSlider::ValueType::Percent),
    sawSlider(CustomSlider::ValueType::Percent),
    lfoAmountSlider(CustomSlider::ValueType::Percent),
    lfoSpeedSlider(CustomSlider::ValueType::Hz),
    lpAmountSlider(CustomSlider::ValueType::Percent),
    lpFreqSlider(CustomSlider::ValueType::Hz),
    envFilterSlider(CustomSlider::ValueType::Percent), processor(p)
{
    setSize (800, 400);
            
    inconsolata = Font (Typeface::createSystemTypefaceFor (BinaryData::InconsolataLight_ttf,
                                                           BinaryData::InconsolataLight_ttfSize));
    // *** CustomSliders ***
        
    addAndMakeVisible(qValSlider);
    addAndMakeVisible(attackSlider);
    addAndMakeVisible(decaySlider);
    addAndMakeVisible(sustainSlider);
    addAndMakeVisible(releaseSlider);
    addAndMakeVisible(volumeSlider);
    addAndMakeVisible(garbageSlider);
    addAndMakeVisible(garbageWetDrySlider);
    addAndMakeVisible(sineSlider);
    addAndMakeVisible(triSlider);
    addAndMakeVisible(squSlider);
    addAndMakeVisible(sawSlider);
    addAndMakeVisible(lfoAmountSlider);
    addAndMakeVisible(lfoSpeedSlider);
    addAndMakeVisible(lpFreqSlider);
    addAndMakeVisible(lpAmountSlider);
    addAndMakeVisible(envFilterSlider);
    
    // *** Labels ***
            
    addAndMakeVisible(purityLabel);
    addAndMakeVisible(attackLabel);
    addAndMakeVisible(decayLabel);
    addAndMakeVisible(sustainLabel);
    addAndMakeVisible(releaseLabel);
    addAndMakeVisible(volumeLabel);
    addAndMakeVisible(garbageLabel);
    addAndMakeVisible(garbageWetDryLabel);
    addAndMakeVisible(sineLabel);
    addAndMakeVisible(triLabel);
    addAndMakeVisible(squLabel);
    addAndMakeVisible(sawLabel);
    addAndMakeVisible(lfoAmountLabel);
    addAndMakeVisible(lfoSpeedLabel);
    addAndMakeVisible(lpFreqLabel);
    addAndMakeVisible(lpAmountLabel);
    addAndMakeVisible(envFilterLabel);
    
    //sends value of the sliders to the tree state in the processor
    qVal = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.tree, "q-value", qValSlider);
    attackVal = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.tree, "attack-value", attackSlider);
    decayVal = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.tree, "decay-value", decaySlider);
    sustainVal = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.tree, "sustain-value", sustainSlider);
    releaseVal = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.tree, "release-value", releaseSlider);
    volumeVal = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.tree, "volume-value", volumeSlider);
    garbageVal = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.tree, "garbage-value", garbageSlider);
    garbageWetDryVal = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.tree, "garbage-wet-dry", garbageWetDrySlider);
    sineVal = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.tree, "sine-val", sineSlider);
    triVal = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.tree, "tri-val", triSlider);
    squVal = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.tree, "squ-val", squSlider);
    sawVal = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.tree, "saw-val", sawSlider);
    lfoAmount = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.tree, "lfo-amount", lfoAmountSlider);
    lfoSpeed = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.tree, "lfo-speed", lfoSpeedSlider);
    lowPassAmount = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.tree, "lowpass-amount", lpAmountSlider);
    lowPassFreq = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.tree, "lowpass-freq", lpFreqSlider);
    envFilter = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.tree, "env-filter", envFilterSlider);
}

FrontGui::~FrontGui()
{
}

void FrontGui::paint (Graphics& g)
{
    
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
//    g.setOpacity(0.6f);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void FrontGui::resized()
{
    setUpSlider(attackSlider, attackLabel, "Attack", 150, 0, 0);
    setUpSlider(decaySlider, decayLabel, "Decay", 150, 150, 0);
    setUpSlider(sustainSlider, sustainLabel, "Sustain", 150, 0, 150);
    setUpSlider(releaseSlider, releaseLabel, "Release", 150, 150, 150);
    setUpSlider(qValSlider, purityLabel, "Purity", 200, 300, 0);
    setUpSlider(envFilterSlider, envFilterLabel, "Filter Env", 150, 500, 0);
    setUpSlider(volumeSlider, volumeLabel, "Volume", 150, 650, 0);
    setUpSlider(garbageWetDrySlider, garbageWetDryLabel, "Garbage Wet/Dry", 150, 500, 150);
    setUpSlider(garbageSlider, garbageLabel, "Garbage Values", 150, 650, 150);
    setUpSlider(sineSlider, sineLabel, "Sine", 100, 0, 300);
    setUpSlider(triSlider, triLabel, "Triangle", 100, 100, 300);
    setUpSlider(squSlider, squLabel, "Square", 100, 200, 300);
    setUpSlider(sawSlider, sawLabel, "Saw", 100, 300, 300);
    setUpSlider(lfoAmountSlider, lfoAmountLabel, "Pitch LFO", 100, 400, 300);
    setUpSlider(lfoSpeedSlider, lfoSpeedLabel, "LFO Speed", 100, 500, 300);
    setUpSlider(lpAmountSlider, lpAmountLabel, "Low Pass", 100, 600, 300);
    setUpSlider(lpFreqSlider, lpFreqLabel, "Low Pass Freq", 100, 700, 300);
    

    
}

void FrontGui::setUpSlider(CustomSlider& slider, Label& label, String text, int boxSize, int xplacement, int yplacement)
{
    float fontSize = 1.0f * ((int)(0.18f * boxSize));
    int sliderX = (int) (0.05f * boxSize + xplacement);
    int sliderY = (int) (0.375f * boxSize + yplacement);
    int sliderWidth =  boxSize - (int) (boxSize * 0.1f);
    int sliderHeight = (int) (0.375f * boxSize);
    int labelX = xplacement;
    int labelY = (int) (0.05f * boxSize + yplacement);
    int labelWidth = boxSize;
    int labelHeight = (int) (0.2f * boxSize);
    
    label.setFont(inconsolata.withHeight(fontSize));
    label.setText(text, dontSendNotification);
    label.setJustificationType(Justification::horizontallyCentred);
    
    slider.setBounds(sliderX, sliderY, sliderWidth, sliderHeight);
    label.setBounds(labelX, labelY, labelWidth, labelHeight);
}
