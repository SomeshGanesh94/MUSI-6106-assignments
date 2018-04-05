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
#define MOD_WIDTH_ID "modWidInMs"
#define MOD_WIDTH_NAME "Mod width"
#define MOD_FREQ_ID "modFreqInHz"
#define MOD_FREQ_NAME "Mod frequency"


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

    void setParam (CVibrato::VibratoParam_t eParam, float fParamValue);
    void setChangedParam (CVibrato::VibratoParam_t eParam, float fParamValue);
    AudioProcessorValueTreeState m_treeState;
    

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VibratoPluginAudioProcessor)
    
    CVibrato *m_pCVibrato;
    CVibrato::VibratoParam_t m_VChangedParam;
    
    float m_fChangedParamValue;
    
    
};
