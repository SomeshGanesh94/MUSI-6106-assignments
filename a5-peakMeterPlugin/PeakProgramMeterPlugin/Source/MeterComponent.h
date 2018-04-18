/*
  ==============================================================================

    MeterComponent.h
    Created: 17 Apr 2018 7:06:45pm
    Author:  Somesh Ganesh

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class MeterComponent    : public Component
{
public:
    MeterComponent()
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.
        
        // Assuming no more than 50 channels will be used
        m_pfPeakValue = new float[50];
        m_iNumChannels = 1;
    }

    ~MeterComponent()
    {
        delete [] m_pfPeakValue;
        m_pfPeakValue = NULL;
    }

    void paint (Graphics& g) override
    {
        /* This demo code just fills the component's background and
           draws some placeholder text to get you started.

           You should replace everything in this method with your own
           drawing code..
        */

        g.fillAll (Colours::black);   // clear the background

        g.setColour (Colours::green);
        
        float fChannelBlockSize = (getWidth() - (m_iNumChannels - 1) * 10) / m_iNumChannels;
        float fVolumeBlockSize = getHeight()/12;
        for (int iChannel = 0; iChannel < m_iNumChannels; iChannel++)
        {
            float fStart = (float)iChannel * fChannelBlockSize + 10 * float(iChannel) ;
            float fEnd = fStart + fChannelBlockSize;
            float fHeight = abs(m_pfPeakValue[iChannel]) * fVolumeBlockSize;
            if (m_pfPeakValue[iChannel] >= 0)
                g.setColour (Colours::red);
            else
                g.setColour (Colours::green);
            g.fillRect(fStart, fHeight, fEnd, (float)getHeight());
//            std::cout << m_pfPeakValue[iChannel] << " ";
        }
//        std::cout << std::endl;
        g.setColour (Colours::white);
        g.setFont (14.0f);
//        g.drawText ("Peak Program Meter", getLocalBounds(),
//                    Justification::centred, true);   // draw some placeholder text
    }

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..

    }
    
    void setValue(float* fPeakValue)
    {
        for (int iChannel = 0; iChannel < m_iNumChannels; iChannel++)
        {
            float fDBValue = 20.0*log10(fPeakValue[iChannel]);
            if (fDBValue > -12.0)
            {
                m_pfPeakValue[iChannel] = fDBValue;
                std::cout << fDBValue << " " ;
            }
            else
            {
                m_pfPeakValue[iChannel] = -12.0;
            }
        }
        std::cout << std::endl;
        this->repaint();
    }
    
    void setNumChannels(int iNumChannels)
    {
        m_iNumChannels = iNumChannels;
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MeterComponent)

    float* m_pfPeakValue;
    int m_iNumChannels;
    
    bool m_bIsInitialized;
};
