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
FrontGui::FrontGui(SubtractiveSynthPlugInAudioProcessor& p) : processor(p)
{
    
    setSize (600, 300);
    
    // *** Labels ***
        
    addAndMakeVisible (purityLabel);
    purityLabel.setFont( Font("Inconsolata-Regular", 24.0f, Font::plain));
    purityLabel.setText ("Purity", dontSendNotification);
    purityLabel.attachToComponent (&qValSlider, true);
    
    addAndMakeVisible (attackLabel);
    attackLabel.setFont( Font("Inconsolata-Regular", 24.0f, Font::plain));
    attackLabel.setText ("Attack", dontSendNotification);
    attackLabel.attachToComponent (&attackSlider, true);
      
    addAndMakeVisible (decayLabel);
    decayLabel.setFont( Font("Inconsolata-Regular", 24.0f, Font::plain));
    decayLabel.setText ("Decay", dontSendNotification);
    decayLabel.attachToComponent (&decaySlider, true);
        
    addAndMakeVisible (releaseLabel);
    releaseLabel.setFont( Font("Inconsolata-Regular", 24.0f, Font::plain));
    releaseLabel.setText ("Release", dontSendNotification);
    releaseLabel.attachToComponent (&releaseSlider, true);
        
    addAndMakeVisible (sustainLabel);
    sustainLabel.setFont( Font("Inconsolata-Regular", 24.0f, Font::plain));
    sustainLabel.setText ("Sustain", dontSendNotification);
    sustainLabel.attachToComponent (&sustainSlider, true);
            
    // *** Sliders ***
        
    addAndMakeVisible(qValSlider);
            
    addAndMakeVisible(attackSlider);
    attackSlider.setTextValueSuffix(" sec");
        
    addAndMakeVisible(decaySlider);
    decaySlider.setTextValueSuffix(" sec");
            
    addAndMakeVisible(sustainSlider);
    sustainSlider.setTextValueSuffix("%");
            
    addAndMakeVisible(releaseSlider);
    releaseSlider.setTextValueSuffix(" sec");
            
    //sends value of the sliders to the tree state in the processor
    qVal = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "q-value", qValSlider);
    attackVal = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "attack-value", attackSlider);
    decayVal = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "decay-value", decaySlider);
    sustainVal = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "sustain-value", sustainSlider);
    releaseVal = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "release-value", releaseSlider);
}

FrontGui::~FrontGui()
{
}

void FrontGui::paint (Graphics& g)
{
    g.setOpacity(0.6f);
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
    g.setFont( Font("Inconsolata-Regular", 24.0f, Font::plain));
}

void FrontGui::resized()
{
    qValSlider.setBounds (100, 100, getWidth() - 120, 20);
    attackSlider.setBounds (100, 300, getWidth()/2 - 120, 20);
    decaySlider.setBounds (100 + getWidth()/2, 300, getWidth()/2 - 120, 20);
    releaseSlider.setBounds (100, 360, getWidth()/2 - 120, 20);
    sustainSlider.setBounds (100 + getWidth()/2, 360, getWidth()/2 - 120, 20);

}
