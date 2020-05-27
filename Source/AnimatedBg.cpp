/*
  ==============================================================================

    AnimatedBg.cpp
    Created: 25 May 2020 4:19:43pm
    Author:  Chris Robinson

  ==============================================================================
*/

#include "AnimatedBg.h"

//==============================================================================
AnimatedBg::AnimatedBg(SubtractiveSynthPlugInAudioProcessor& p) : processor(p){
    
    static1 = PNGImageFormat::loadFrom (File ("/Users/patrick/Documents/audio-programming/SubtractiveSynthPlugIn/images/static/static1.png"));
    static2 = PNGImageFormat::loadFrom (File ("/Users/patrick/Documents/audio-programming/SubtractiveSynthPlugIn/images/static/static2.png"));
    setSize (800, 600);
    setFramesPerSecond(24);
    
}

AnimatedBg::~AnimatedBg(){}

void AnimatedBg::update(){
    
}

void AnimatedBg::paint (Graphics& g){
    g.setOpacity(1.0f - (0.01f * *processor.tree.getRawParameterValue("q-value")));
    
    if( processor.getActiveVoiceCounter() == 0) g.drawImageAt(static1, 0, 0);
    else{
        if( getFrameCounter() % 2 == 0 )
            g.drawImageAt(static1, 0, 0);
        else g.drawImageAt(static2, 0, 0);
    }

}
void AnimatedBg::resized(){
    
}
