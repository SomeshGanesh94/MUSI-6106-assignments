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
    
    m_slModWidth.setRange(0.0 , 1.0);
    m_slModWidth.setTextValueSuffix("Modulation width in sec");
    m_slModWidth.addListener(this);
    addAndMakeVisible(m_slModWidth);
    
    m_slModFrequency.setRange(0.0 , processor.getSampleRate()*0.5);
    m_slModFrequency.setTextValueSuffix("Modulation frequency in Hz");
    m_slModFrequency.addListener(this);
    addAndMakeVisible(m_slModFrequency);
    
    m_tbBypass.setButtonText("Bypass switch");
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
    g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
    
}

void VibratoPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    auto sliderLeft = 50;
    m_slModWidth.setBounds(sliderLeft, 20, getWidth() - sliderLeft - 10, 20);
    m_slModFrequency.setBounds(sliderLeft, 50, getWidth() - sliderLeft - 10, 20);
    m_tbBypass.setBounds(sliderLeft, 80, 50, 20);

}

void VibratoPluginAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
//    std::cout << "Slider value : " << (float)slider->getValue() << std::endl;
    if (slider == &m_slModWidth)
    {
        processor.setParam(CVibrato::kParamModWidthInS, (float)slider->getValue());
    }
    else if (slider == &m_slModFrequency)
    {
        processor.setParam(CVibrato::kParamModFreqInHz, (float)slider->getValue());
    }
}

void VibratoPluginAudioProcessorEditor::buttonClicked(Button* button)
{
    if (button == &m_tbBypass)
    {
        processor.setBypass(button->getToggleState());
    }
}
