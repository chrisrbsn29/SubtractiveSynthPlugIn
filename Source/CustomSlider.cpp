/*
  ==============================================================================

    CustomSlider.cpp
    Created: 26 May 2020 10:05:20pm
    Author:  Chris Robinson

  ==============================================================================
*/

#include "CustomSlider.h"

//==============================================================================
CustomSlider::CustomSlider(){
    init(ValueType::Seconds);
}

CustomSlider::CustomSlider(ValueType vt){
    init(vt);
}

void CustomSlider::init(ValueType vt){
    
    inconsolata = Font (Typeface::createSystemTypefaceFor (BinaryData::InconsolataLight_ttf,
    BinaryData::InconsolataLight_ttfSize));
    percentSuffix = "%";
    msSuffix = " msec";
    secSuffix = " sec";
    setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    valueType = vt;
    setTextBoxStyle(TextEntryBoxPosition::NoTextBox, false, this->getBounds().getWidth(), this->getBounds().getHeight());
    addAndMakeVisible(label);
    label.setBounds(0,0,0,0);
    label.setVisible(false);
    label.addListener(this);
    showText = true;
    updateDisplayValue(this->getValue());
}

void CustomSlider::updateDisplayValue(float valueFromSlider)
{
    if(valueType == ValueType::Seconds)
    {
        if(valueFromSlider < 0.01f)
        {                                                       // 0.01 msec to 9.99ms
            displayVal = (int)(valueFromSlider * 100000.0 + 0.5);
            displayVal = displayVal * 0.01; //2 decimal precision
            displayStr = String(displayVal, 2) + msSuffix;
        }
        else if(valueFromSlider < 0.1f && valueFromSlider >= 0.01f)
        {                                                         // 10 msec to 99.99ms
            displayVal = (int)(valueFromSlider * 10000.0 + 0.5);
            displayVal = displayVal * 0.1; //1 decimal precision
            displayStr = String(displayVal, 1) + msSuffix;
            
        }
        else if(valueFromSlider < 1.0f && valueFromSlider >= 0.1f)
        { // 100 msec to 999.99ms
            displayVal = (int)(valueFromSlider * 1000.0 + 0.5);        // 3 digits, no decimal
            displayStr = String(displayVal) + msSuffix;
            
        }
        else if(valueFromSlider < 10.0f && valueFromSlider >= 1.0f)
        {                                                        // 1sec to 9.99sec
            displayVal = (int) (valueFromSlider * 100.0 + 0.5);        //2 decimal places
            displayVal = displayVal * 0.01;
            displayStr = String(displayVal, 2) + secSuffix;
        }
        else
        {                                                        //10+ sec
            displayVal = (int) (valueFromSlider * 10.0 + 0.5);
            displayVal = displayVal * 0.1;
            displayStr = String(displayVal, 1) + secSuffix;
        }
    }
    else if (valueType == ValueType::Neutral)
    {
        displayVal = (int) valueFromSlider;
        displayStr = String(displayVal);
    }
    else if (valueType == ValueType::Hz)
    {
        if (valueFromSlider < 10.0f)
        {
            displayVal = (int) valueFromSlider * 10.0;
            displayVal = displayVal * 0.1;
            displayStr = String(displayVal, 2) + " Hz";
        }
        else{
            displayVal = (int) valueFromSlider;
            displayStr = String(displayVal) + " Hz";
        }
    }
    else
    { //if percent
        displayVal = (int) valueFromSlider;
        displayStr = String(displayVal) + percentSuffix;
    }
}

void CustomSlider::paint(Graphics& g)
{
    if(showText){
        g.setFont(Font(inconsolata.withHeight(this->getBounds().getHeight() * 0.50f)));
        g.setColour(Colours::white);
        g.drawText(displayStr, 0, 0, this->getBounds().getWidth(),this->getBounds().getHeight(), Justification::centred);
    }
}

void CustomSlider::resized(){
//    label->setBounds( 0, 0, this->getBounds().getWidth(), this->getBounds().getHeight());
}

void CustomSlider::mouseDoubleClick(const MouseEvent& e)
{
    label.setBounds(0, 0, this->getBounds().getWidth(), this->getBounds().getHeight());
    label.setFont(Font(inconsolata.withHeight(this->getBounds().getHeight() * 0.50f)));
    label.setJustificationType(Justification::centred);
    label.setVisible(true);
    label.setText(displayStr, dontSendNotification);
    label.setColour(Label::textColourId, Colours::white);
    label.showEditor();
    
}

void CustomSlider::valueChanged()
{
    updateDisplayValue(this->getValue());
    repaint();
}

void CustomSlider::textChanged()
{
    auto newValue = snapValue (getValueFromText (label.getText()), notDragging);
    setValue (newValue, sendNotificationSync);

}

void CustomSlider::labelTextChanged(Label *labelThatHasChanged)
{
    textChanged();
}
void CustomSlider::editorShown (Label *, TextEditor &)
{
    showText = false;
    repaint();
}
void CustomSlider::editorHidden (Label *, TextEditor &)
{
    label.setBounds(0, 0, 0, 0);
    label.setVisible(false);
    showText = true;
    repaint();
}

double CustomSlider::getValueFromText(const String& text){
    if(text.endsWith(msSuffix) || text.endsWith("ms")){
        return 0.001 * text.initialSectionContainingOnly ("0123456789.,-")
        .getDoubleValue();
    }
    else return Slider::getValueFromText(text);
}
