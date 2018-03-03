
// standard headers

// project headers
#include <iostream>

#include "MUSI6106Config.h"

#include "ErrorDef.h"

#include "RingBuffer.h"

#include "Lfo.h"
#include "Vibrato.h"

CVibrato::CVibrato(): m_bIsInitialized(false), m_pCLfo(0), m_pCRingBuffer(0), m_fSampleRateInSamples(0), m_fModFreqInSamples(0), m_fWidthInSamples(0), m_iNumChannels(0)
{
    this->reset();
}

CVibrato::~CVibrato()
{
    this->reset();
    
    delete [] m_pCRingBuffer;
    m_pCRingBuffer = 0;
    delete [] m_pCLfo;
    m_pCLfo = 0;
}

Error_t CVibrato::create(CVibrato*& pCMyProject)
{
    pCMyProject = new CVibrato ();
    
    if(!pCMyProject)
        return kUnknownError;
    
    return kNoError;
}

Error_t CVibrato::destroy(CVibrato*& pCMyProject)
{
    if(!pCMyProject)
        return kUnknownError;
    
    pCMyProject->reset();
    
    delete pCMyProject;
    pCMyProject = 0;
    
    return kNoError;
}

Error_t CVibrato::init(float fSampleRateInHz, float fModFrequencyInHz, float fWidthInHz, int iNumChannels)
{
    this->reset();
    
    if (fSampleRateInHz <=0     ||
        iNumChannels <= 0       ||
        fModFrequencyInHz <=0   ||
        fWidthInHz <=0)
        return kFunctionInvalidArgsError;
    
    m_fSampleRateInSamples = fSampleRateInHz;
    m_fModFreqInSamples = fModFrequencyInHz / m_fSampleRateInSamples;
//    std::cout << m_fModFreqInSamples << std::endl;
//    m_fModFreqInSamples = fModFrequencyInHz;
    m_fWidthInSamples = fWidthInHz * m_fSampleRateInSamples;
    m_iNumChannels = iNumChannels;
    
    m_pCLfo = new CLfo (m_fModFreqInSamples, m_fWidthInSamples, m_fSampleRateInSamples);
    m_pCLfo->processLfo();
    
    int iLength = int(2 + m_fWidthInSamples + m_fWidthInSamples * 2);
    //    std::cout << iLength << std::endl;
    m_pCRingBuffer = new CRingBuffer<float>(iLength);
    
    m_bIsInitialized = true;
    
    return kNoError;
}

Error_t CVibrato::reset()
{
    delete m_pCLfo;
    m_pCLfo = 0;
    delete m_pCRingBuffer;
    m_pCRingBuffer = 0;
    
    m_fSampleRateInSamples = 0;
    m_fModFreqInSamples = 0;
    m_fWidthInSamples = 0;
    m_bIsInitialized = false;
    
    return kNoError;
}

Error_t CVibrato::setParam(VibratoParam_t eParam, float fParamValue)
{
    if(!m_bIsInitialized)
        return kNotInitializedError;

    switch(eParam)
    {
        case kParamModFreq:
            m_fModFreqInSamples = round(fParamValue / m_fSampleRateInSamples);
            m_pCLfo->setModFreq(m_fModFreqInSamples);
        case kParamWidth:
            m_fWidthInSamples = fParamValue * m_fSampleRateInSamples;
        case kSampleRate:
            m_fSampleRateInSamples = fParamValue;
        case kNumVibratoParams:
            return kFunctionInvalidArgsError;
    }
    return kNoError;
}

float CVibrato::getParam(VibratoParam_t eParam) const
{
    if (!m_bIsInitialized)
        return kNotInitializedError;
    
    switch(eParam)
    {
        case kParamModFreq:
            return m_fModFreqInSamples;
        case kParamWidth:
            return m_fWidthInSamples;
        case kSampleRate:
            return m_fSampleRateInSamples;
        case kNumVibratoParams:
            return kFunctionInvalidArgsError;
    }
}

Error_t CVibrato::process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames)
{
    if (!m_bIsInitialized)
        return kNotInitializedError;
    
    for (int i = 0; i < m_iNumChannels; i++)
    {
        for (int j = 0; j < iNumberOfFrames; j++)
        {
            //get value from LFO
            //placeholder = mod
            float mod = m_pCLfo->returnLfoVal();
//            std::cout<< mod << std::endl;
            
            float fDelay = 1 + m_fWidthInSamples + m_fWidthInSamples * mod;
            int iDelayRounded = floor(fDelay);
            float fFracValue = fDelay - iDelayRounded;
            
            m_pCRingBuffer->put(ppfInputBuffer[i][j]);
            m_pCRingBuffer->setWriteIdx(m_pCRingBuffer->getWriteIdx()-1);
//            std::cout << mod << std::endl;
            
            ppfOutputBuffer[i][j] = m_pCRingBuffer->get(j+1)*fFracValue + m_pCRingBuffer->get(j)*(1 - fFracValue);
//            ppfOutputBuffer[i][j] = m_pCRingBuffer->get(fDelay);
//            std::cout << ppfInputBuffer[i][j] << " " << ppfOutputBuffer[i][j] << std::endl;
            m_pCRingBuffer->setReadIdx(m_pCRingBuffer->getReadIdx()-1);
        }
    }
    
    
    
    return kNoError;
}
