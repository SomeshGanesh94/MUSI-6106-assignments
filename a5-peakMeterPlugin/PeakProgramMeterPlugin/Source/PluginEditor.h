/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "MeterComponent.h"

//==============================================================================
/**
*/
class PeakProgramMeterPluginAudioProcessorEditor  : public AudioProcessorEditor, public Timer
{
public:
    PeakProgramMeterPluginAudioProcessorEditor (PeakProgramMeterPluginAudioProcessor&);
    ~PeakProgramMeterPluginAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    void timerCallback() override;
    
    void setNumChannels();
    void setValue();

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PeakProgramMeterPluginAudioProcessor& processor;
    
    MeterComponent m_CMeterComponent;
    
    Label m_lPluginName;
    Label m_lPluginCreators;
    Label m_lLowEndIndicator, m_lHighEndIndicator;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PeakProgramMeterPluginAudioProcessorEditor)
};
