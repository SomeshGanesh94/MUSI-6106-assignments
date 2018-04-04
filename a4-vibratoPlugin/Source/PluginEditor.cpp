/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
VibratoPluginAudioProcessorEditor::VibratoPluginAudioProcessorEditor (VibratoPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    m_SliderAttachmentModWid = new AudioProcessorValueTreeState::SliderAttachment (processor.m_treeState, MOD_WIDTH_ID, m_slModWidth);
    m_SliderAttachmentModFreq = new AudioProcessorValueTreeState::SliderAttachment (processor.m_treeState, MOD_FREQ_ID, m_slModFrequency);
    
    m_lPluginName.setText("Vibrato plugin: assignment 4", dontSendNotification);
    m_lPluginName.setFont(18);
    addAndMakeVisible(m_lPluginName);
    
    m_lPluginCreators.setText("Somesh Ganesh & Yongliang He", dontSendNotification);
    m_lPluginCreators.setFont(10);
    addAndMakeVisible(m_lPluginCreators);
    
    m_lSlModWidth.setText("Modulation width", dontSendNotification);
    m_lSlModWidth.setFont(12);
    addAndMakeVisible(m_lSlModWidth);
    
    m_lSlModFrequency.setText("Modulation frequency", dontSendNotification);
    m_lSlModFrequency.setFont(12);
    addAndMakeVisible(m_lSlModFrequency);
    
    m_lTbBypass.setText("Bypass plugin", dontSendNotification);
    m_lTbBypass.setFont(12);
    addAndMakeVisible(m_lTbBypass);
    
    m_slModWidth.setRange(0.0 , 20.0);
    m_slModWidth.setTextValueSuffix("ms");
    m_slModWidth.addListener(this);
    addAndMakeVisible(m_slModWidth);
    
    m_slModFrequency.setRange(0.0 , 20.0);
    m_slModFrequency.setTextValueSuffix("Hz");
    m_slModFrequency.addListener(this);
    addAndMakeVisible(m_slModFrequency);
    
    m_tbBypass.setToggleState(0, dontSendNotification);
    m_tbBypass.addListener(this);
    addAndMakeVisible(m_tbBypass);
}

VibratoPluginAudioProcessorEditor::~VibratoPluginAudioProcessorEditor()
{
}

//==============================================================================
void VibratoPluginAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
}

void VibratoPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    auto sliderLeft = 50;
    m_lPluginName.setBounds(90, -130, getWidth(), getHeight());
    m_lPluginCreators.setBounds(125, -110, getWidth(), getHeight());
    
    m_lSlModWidth.setBounds(sliderLeft-5, -70, getWidth(), getHeight());
    m_lSlModFrequency.setBounds(sliderLeft-5, 0, getWidth(), getHeight());
    m_lTbBypass.setBounds(sliderLeft-5, 70, getWidth(), getHeight());
    
    m_slModWidth.setBounds(sliderLeft, 100, getWidth() - sliderLeft - 10, 20);
    m_slModFrequency.setBounds(sliderLeft, 170, getWidth() - sliderLeft - 10, 20);
    m_tbBypass.setBounds(sliderLeft, 240, 50, 20);
}

void VibratoPluginAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    if (slider == &m_slModWidth)
    {
        processor.setChangedParam(CVibrato::VibratoParam_t::kParamModWidthInS, slider->getValue());
    }
    else if (slider == &m_slModFrequency)
    {
        processor.setChangedParam(CVibrato::VibratoParam_t::kParamModFreqInHz, slider->getValue());
    }
}

void VibratoPluginAudioProcessorEditor::buttonClicked(Button* button)
{
    if (button == &m_tbBypass)
    {
        processor.setBypass(button->getToggleState(), m_slModWidth.getValue());
    }
}
