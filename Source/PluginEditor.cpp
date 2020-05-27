/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SubtractiveSynthPlugInAudioProcessorEditor::SubtractiveSynthPlugInAudioProcessorEditor (SubtractiveSynthPlugInAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), animatedBg(p), frontGui(p)
    {
        
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);
        
    // *** GUIs ***
        
    addAndMakeVisible(&animatedBg);
    addAndMakeVisible(&frontGui);
    
    
}

SubtractiveSynthPlugInAudioProcessorEditor::~SubtractiveSynthPlugInAudioProcessorEditor()
{
}

//==============================================================================
void SubtractiveSynthPlugInAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    
}

void SubtractiveSynthPlugInAudioProcessorEditor::resized()
{
//    int frontGuiWidth = 600;
//    int frontGuiHeight = 300;
    frontGui.setTopLeftPosition(100, 150);
}
