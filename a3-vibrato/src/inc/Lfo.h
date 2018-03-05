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
    CLfo(float fModFreqInSamples, float fSampleRate):
    m_pCRingBuffer(0),
    m_fModFreqInSamples(fModFreqInSamples),
    m_fSampleRateInHz(fSampleRate),
    m_fReadIdx(0)
    {
        if (m_fModFreqInSamples == 0)
            m_iBufferLength = 1;
        else
            m_iBufferLength = 1 / m_fModFreqInSamples;
        m_pCRingBuffer = new CRingBuffer<float>(m_iBufferLength);
    }
    
    ~CLfo()
    {
        delete m_pCRingBuffer;
        m_pCRingBuffer = 0;
    }
    
    // set parameters
    Error_t setParam(float fParamValue)
    {
        m_fModFreqInSamples = fParamValue;
        if (m_fModFreqInSamples == 0)
            m_iBufferLength = 1;
        else
            m_iBufferLength = 1 / m_fModFreqInSamples;
        
        delete m_pCRingBuffer;
        m_pCRingBuffer = new CRingBuffer<float>(m_iBufferLength);
        this->process();
        
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
    void process()
    {
        float *pfWaveformBuffer = new float [m_iBufferLength];
        
        CSynthesis::generateSine(pfWaveformBuffer, m_fModFreqInSamples * m_fSampleRateInHz, m_fSampleRateInHz, m_iBufferLength);
        
        m_pCRingBuffer-> putPostInc(pfWaveformBuffer, m_iBufferLength);
        
        delete [] pfWaveformBuffer;
    }
    
private:
    
    int                             m_iBufferLength;
    CRingBuffer<float>              *m_pCRingBuffer;
    float                           m_fModFreqInSamples;
    float                           m_fSampleRateInHz;
    float                           m_fReadIdx;
};

#endif // __Lfo_hdr__
