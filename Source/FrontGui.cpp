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
FrontGui::FrontGui(SubtractiveSynthPlugInAudioProcessor& p) : qValSlider(CustomSlider::ValueType::Percent), attackSlider(CustomSlider::ValueType::Seconds), decaySlider(CustomSlider::ValueType::Seconds), sustainSlider(CustomSlider::ValueType::Percent), releaseSlider(CustomSlider::ValueType::Seconds), processor(p)
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
    
    // *** Labels ***
            
    addAndMakeVisible(purityLabel);
    purityLabel.setFont(inconsolata.withHeight(72.0f));
    purityLabel.setText("Purity", dontSendNotification);
    purityLabel.setJustificationType(Justification::horizontallyCentred);
    
    addAndMakeVisible(attackLabel);
    attackLabel.setFont(inconsolata.withHeight(36.0f));
    attackLabel.setText("Attack", dontSendNotification);
    attackLabel.setJustificationType(Justification::horizontallyCentred);

    addAndMakeVisible(decayLabel);
    decayLabel.setFont(inconsolata.withHeight(36.0f));
    decayLabel.setText("Decay", dontSendNotification);
    decayLabel.setJustificationType(Justification::horizontallyCentred);
   
    addAndMakeVisible(sustainLabel);
    sustainLabel.setFont(inconsolata.withHeight(36.0f));
    sustainLabel.setText("Sustain", dontSendNotification);
    sustainLabel.setJustificationType(Justification::horizontallyCentred);
    
    addAndMakeVisible(releaseLabel);
    releaseLabel.setFont(inconsolata.withHeight(36.0f));
    releaseLabel.setText("Release", dontSendNotification);
    releaseLabel.setJustificationType(Justification::horizontallyCentred);
    
    //sends value of the sliders to the tree state in the processor
    qVal = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.tree, "q-value", qValSlider);
    attackVal = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.tree, "attack-value", attackSlider);
    decayVal = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.tree, "decay-value", decaySlider);
    sustainVal = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.tree, "sustain-value", sustainSlider);
    releaseVal = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.tree, "release-value", releaseSlider);
    
    attackSlider.setSkewFactor(0.23f);
    std::cout << attackSlider.getSkewFactor() << NewLine();
}

FrontGui::~FrontGui()
{
}

void FrontGui::paint (Graphics& g)
{
    
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.setOpacity(0.6f);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
    g.setFont( Font("Inconsolata-ExtraLight.ttf", 24.0f, Font::plain));
}

void FrontGui::resized()
{

    attackSlider.setBounds (10, 75, 180, 75);
    decaySlider.setBounds (210, 75, 180, 75);
    sustainSlider.setBounds (10, 275, 180, 75);
    releaseSlider.setBounds (210, 275, 180, 75);
    qValSlider.setBounds (420, 150, 360, 150);

    attackLabel.setBounds(0, 10, 200, 40);
    decayLabel.setBounds(200, 10, 200, 40);
    sustainLabel.setBounds(0, 210, 200, 40);
    releaseLabel.setBounds(200, 210, 200, 40);
    purityLabel.setBounds(400, 20, 400, 80);
}

