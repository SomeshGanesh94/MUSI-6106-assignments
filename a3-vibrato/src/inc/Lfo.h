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
    CLfo(float fModFreqInSamples, float fModWidthInSamples, float fSampleRate):
    m_pCRingBuffer(0),
    m_fModFreqInSamples(fModFreqInSamples),
    m_fWidth(fModWidthInSamples),
    m_fSampleRateInHz(fSampleRate),
    m_fReadIdx(0)
    {
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
        m_fModFreqInSamples = modFreq;
        return kNoError;
    }
    
    //return current values
    float returnLfoVal()
    {
        
        float fLfoVal = m_pCRingBuffer->get(m_fReadIdx);
        
        m_fReadIdx = m_fReadIdx + m_fModFreqInSamples * m_iBufferLength;
        
        if (m_fReadIdx >= m_iBufferLength)
            m_fReadIdx -= m_iBufferLength;
        
        return fLfoVal;
        
    }
    
    //LFO waveform generation
    void processLfo()
    {
        float *pfWaveformBuffer = new float [m_iBufferLength];
        
        CSynthesis::generateSine(pfWaveformBuffer, m_fModFreqInSamples * m_fSampleRateInHz, m_fSampleRateInHz, m_iBufferLength);
        
        m_pCRingBuffer-> putPostInc(pfWaveformBuffer, m_iBufferLength);
        
        delete [] pfWaveformBuffer;
    }
    
private:
    
    int                             m_iBufferLength = int(44100/10);
    CRingBuffer<float>              *m_pCRingBuffer;
    float                           m_fModFreqInSamples;
    float                           m_fWidth;
    float                           m_fSampleRateInHz;
    float                           m_fReadIdx;
};

#endif // __Lfo_hdr__
