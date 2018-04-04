/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once
#include<iostream>
#include "../JuceLibraryCode/JuceHeader.h"
#include "Vibrato.h"

//==============================================================================
/**
*/
class VibratoPluginAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    VibratoPluginAudioProcessor();
    ~VibratoPluginAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //==============================================================================
    void setBypass(bool bValue);
    
    void setParam (CVibrato::VibratoParam_t eParam, float fParamValue);
    void setChangedParam (CVibrato::VibratoParam_t eParam, float fParamValue);

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VibratoPluginAudioProcessor)
    
    CVibrato *m_pCVibrato;
    bool m_bBypass;
    
    CVibrato::VibratoParam_t m_VChangedParam;
    float m_fChangedParamValue;
};
