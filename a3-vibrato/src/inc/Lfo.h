#if !defined(__Lfo_hdr__)
#define __Lfo_hdr__

#define _USE_MATH_DEFINES
#include <math.h>

#include "ErrorDef.h"
#include "Synthesis.h"
#include "RingBuffer.h"

class CLfo
{
public:
    CLfo(float modFreq, float Width, float sampleRate):
    m_pCRingBuffer(0),
    m_fModFreq(modFreq),
    m_fWidth(Width),
    m_fSampleRateInHz(sampleRate),
    m_fReadIdx(0)
    {
        m_iBufferLength = int(m_fSampleRateInHz / modFreq);
        m_pCRingBuffer = new CRingBuffer<float>(m_iBufferLength);
    }
    
    ~CLfo()
    {
        delete m_pCRingBuffer;
        m_pCRingBuffer = 0;
    }
    
    // set parameters
    Error_t setModFreq(float modFreq)
    {
        m_fModFreq = modFreq;
        return kNoError;
    }
    
    //return current values
    float returnLfoVal()
    {
        float fLfoVal =  m_pCRingBuffer->getPostInc();
        return fLfoVal;
    }
    
    //LFO waveform generation
    void processLfo()
    {
        float *pfWaveformBuffer = new float [m_iBufferLength];
        CSynthesis::generateSine(pfWaveformBuffer, m_fModFreq, m_fSampleRateInHz, m_iBufferLength);
        for (int i = 0; i < m_iBufferLength; i++)
        {
            m_pCRingBuffer-> putPostInc(pfWaveformBuffer[i]);
        }
        delete [] pfWaveformBuffer;
    }
    
private:
    
    int                             m_iBufferLength;
    CRingBuffer<float>              *m_pCRingBuffer;
    float                           m_fModFreq;
    float                           m_fWidth;
    float                           m_fSampleRateInHz;
    float                           m_fReadIdx;
};

#endif // __Lfo_hdr__
