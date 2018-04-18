/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PeakProgramMeterPluginAudioProcessor::PeakProgramMeterPluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    CPpm::createInstance(m_pCPpm);
    m_pfVPpm = new float[50];
}

PeakProgramMeterPluginAudioProcessor::~PeakProgramMeterPluginAudioProcessor()
{
    CPpm::destroyInstance(m_pCPpm);
    m_pCPpm = NULL;
    delete [] m_pfVPpm;
    m_pfVPpm = NULL;
}

//==============================================================================
const String PeakProgramMeterPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PeakProgramMeterPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PeakProgramMeterPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PeakProgramMeterPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PeakProgramMeterPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PeakProgramMeterPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PeakProgramMeterPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PeakProgramMeterPluginAudioProcessor::setCurrentProgram (int index)
{
}

const String PeakProgramMeterPluginAudioProcessor::getProgramName (int index)
{
    return {};
}

void PeakProgramMeterPluginAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void PeakProgramMeterPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    m_pCPpm->init(samplesPerBlock, samplesPerBlock, sampleRate, getTotalNumInputChannels());
}

void PeakProgramMeterPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PeakProgramMeterPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void PeakProgramMeterPluginAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    if (m_pCPpm->isInitialized())
    {
        ScopedNoDenormals noDenormals;
        auto totalNumInputChannels  = getTotalNumInputChannels();
        auto totalNumOutputChannels = getTotalNumOutputChannels();
        
        m_pCPpm->setNumChannels(totalNumInputChannels);

        // In case we have more outputs than inputs, this code clears any output
        // channels that didn't contain input data, (because these aren't
        // guaranteed to be empty - they may contain garbage).
        // This is here to avoid people getting screaming feedback
        // when they first compile a plugin, but obviously you don't need to keep
        // this code if your algorithm always overwrites all the output channels.
        for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
            buffer.clear (i, 0, buffer.getNumSamples());

        // This is the place where you'd normally do the guts of your plugin's
        // audio processing...
        // Make sure to reset the state if your inner loop is processing
        // the samples and the outer loop is handling the channels.
        // Alternatively, you can process the samples with the channels
        // interleaved by keeping the same state.
        
        m_pCPpm->process((float **)buffer.getArrayOfReadPointers(), m_pfVPpm, buffer.getNumSamples());
    }
}

//==============================================================================
bool PeakProgramMeterPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* PeakProgramMeterPluginAudioProcessor::createEditor()
{
    return new PeakProgramMeterPluginAudioProcessorEditor (*this);
}

//==============================================================================
void PeakProgramMeterPluginAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void PeakProgramMeterPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PeakProgramMeterPluginAudioProcessor();
}

float* PeakProgramMeterPluginAudioProcessor::getLastMaxPpmValue()
{
    return m_pfVPpm;
}
