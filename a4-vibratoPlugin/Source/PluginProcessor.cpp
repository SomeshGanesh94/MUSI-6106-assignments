/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
//==============================================================================
VibratoPluginAudioProcessor::VibratoPluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
        m_treeState(*this, nullptr)
#endif
{
    CVibrato::createInstance(m_pCVibrato);
    m_VChangedParam = CVibrato::VibratoParam_t::kNumVibratoParams;
    m_fChangedParamValue = 0.0;
    m_treeState.createAndAddParameter(MOD_WIDTH_ID,MOD_WIDTH_NAME, MOD_WIDTH_NAME, NormalisableRange<float>(0.0f, 20.0f), 0.0f, nullptr, nullptr);
    m_treeState.createAndAddParameter(MOD_FREQ_ID, MOD_FREQ_NAME, MOD_FREQ_NAME, NormalisableRange<float>(0.0f, 20.0f), 0.0f, nullptr, nullptr);
    m_treeState.state = ValueTree ("savedParams");
}

VibratoPluginAudioProcessor::~VibratoPluginAudioProcessor()
{
    CVibrato::destroyInstance(m_pCVibrato);
    m_pCVibrato = NULL;
    m_VChangedParam = CVibrato::VibratoParam_t::kNumVibratoParams;
    m_fChangedParamValue = 0.0;
}

//==============================================================================
const String VibratoPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool VibratoPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool VibratoPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool VibratoPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double VibratoPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int VibratoPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int VibratoPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void VibratoPluginAudioProcessor::setCurrentProgram (int index)
{
}

const String VibratoPluginAudioProcessor::getProgramName (int index)
{
    return {};
}

void VibratoPluginAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void VibratoPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    m_pCVibrato->initInstance(1, sampleRate,
                              (this->getTotalNumOutputChannels() > this->getTotalNumInputChannels()) ? this->getTotalNumInputChannels() : this->getTotalNumOutputChannels());
}

void VibratoPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool VibratoPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void VibratoPluginAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());


    if (m_pCVibrato->isInitialized())
    {
        m_pCVibrato->process((float **)buffer.getArrayOfReadPointers(), buffer.getArrayOfWritePointers(), buffer.getNumSamples());
    }
}

//==============================================================================
bool VibratoPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* VibratoPluginAudioProcessor::createEditor()
{
    return new VibratoPluginAudioProcessorEditor (*this);
}

//==============================================================================
void VibratoPluginAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    ScopedPointer<XmlElement> xml(m_treeState.state.createXml());
    copyXmlToBinary(*xml, destData);
}

void VibratoPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    ScopedPointer<XmlElement> theParams (getXmlFromBinary(data, sizeInBytes));
    if (theParams != nullptr) {
        if (theParams->hasTagName(m_treeState.state.getType())) {
            m_treeState.state = ValueTree::fromXml(*theParams);
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new VibratoPluginAudioProcessor();
}


void VibratoPluginAudioProcessor::setParam(CVibrato::VibratoParam_t eParam, float fParamValue)
{
    if (eParam == CVibrato::VibratoParam_t::kParamModWidthInS)
        fParamValue /= 1000.0;
    m_pCVibrato->setParam(eParam, fParamValue);
}

void VibratoPluginAudioProcessor::setChangedParam(CVibrato::VibratoParam_t eParam, float fParamValue)
{
    m_VChangedParam = eParam;
    m_fChangedParamValue = fParamValue;
}
