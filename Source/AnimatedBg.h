/*
  ==============================================================================

    AnimatedBg.h
    Created: 25 May 2020 4:19:43pm
    Author:  Chris Robinson

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "PluginProcessor.h"

//==============================================================================
class AnimatedBg   : public AnimatedAppComponent
{
public:
    AnimatedBg(SubtractiveSynthPlugInAudioProcessor&);
    ~AnimatedBg();
    
    //==============================================================================
    void update() override;
    void paint (Graphics&) override;
    void resized() override;
    
private:
    
    SubtractiveSynthPlugInAudioProcessor& processor;
    Image static1;
    Image static2;

};

