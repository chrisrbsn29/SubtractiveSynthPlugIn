/*
  ==============================================================================

    CustomSlider.h
    Created: 26 May 2020 10:05:20pm
    Author:  Chris Robinson

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class CustomSlider    : public Slider,
                        public Label::Listener

{
public:
    enum ValueType{
        Seconds,
        Percent,
        Decibels,
        Neutral
    };
    
    CustomSlider();
    CustomSlider(ValueType vt);
    
    void paint(Graphics&) override;
    void resized() override;
    void valueChanged() override;
    void mouseDoubleClick(const MouseEvent&) override;
    double getValueFromText (const String& text) override;
    void textChanged();
    
    void labelTextChanged(Label *labelThatHasChanged) override;
    void editorShown (Label *, TextEditor &) override;
    void editorHidden (Label *, TextEditor &) override;
    
private:
    void init(ValueType vt);
    void updateDisplayValue(float valueFromSlider);
    
    Label label;
    ValueType valueType;
    String percentSuffix;
    String msSuffix;
    String secSuffix;
    String displayStr;
    double displayVal;
    Font inconsolata;
    bool showText;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomSlider)
};
