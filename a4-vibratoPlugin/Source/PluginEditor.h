/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class VibratoPluginAudioProcessorEditor  : public AudioProcessorEditor, public Slider::Listener, public Button::Listener
{
public:
    VibratoPluginAudioProcessorEditor (VibratoPluginAudioProcessor&);
    ~VibratoPluginAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    void sliderValueChanged(Slider* slider) override;
    void buttonClicked(Button* button) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    VibratoPluginAudioProcessor& processor;
    
    Slider m_slModWidth;
    Slider m_slModFrequency;
    
    ToggleButton m_tbBypass;
    
    Label m_lPluginName;
    Label m_lPluginCreators;
    Label m_lSlModWidth;
    Label m_lSlModFrequency;
    Label m_lTbBypass;
    
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> m_SliderAttachmentModWid;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> m_SliderAttachmentModFreq;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VibratoPluginAudioProcessorEditor)
};
