/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PeakProgramMeterPluginAudioProcessorEditor::PeakProgramMeterPluginAudioProcessorEditor (PeakProgramMeterPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    addAndMakeVisible(m_CMeterComponent);
    m_CMeterComponent.setSize(100, 100);
    
    m_lPluginName.setText("Peak program meter: assignment 5", dontSendNotification);
    m_lPluginName.setFont(18);
    addAndMakeVisible(m_lPluginName);
    
    m_lPluginCreators.setText("Somesh Ganesh & Hanyu Li", dontSendNotification);
    m_lPluginCreators.setFont(10);
    addAndMakeVisible(m_lPluginCreators);
    
    startTimer(10);
}

PeakProgramMeterPluginAudioProcessorEditor::~PeakProgramMeterPluginAudioProcessorEditor()
{
}

//==============================================================================
void PeakProgramMeterPluginAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
}

void PeakProgramMeterPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    m_CMeterComponent.setBounds(0, 0, 100, 100);
    m_lPluginName.setBounds(90, -130, getWidth(), getHeight());
    m_lPluginCreators.setBounds(125, -110, getWidth(), getHeight());
}

void PeakProgramMeterPluginAudioProcessorEditor::timerCallback()
{
    this->setNumChannels();
    this->setValue();
}

void PeakProgramMeterPluginAudioProcessorEditor::setNumChannels()
{
    m_CMeterComponent.setNumChannels(processor.getTotalNumInputChannels());
}

void PeakProgramMeterPluginAudioProcessorEditor::setValue()
{
    m_CMeterComponent.setValue(processor.getLastMaxPpmValue());
}
