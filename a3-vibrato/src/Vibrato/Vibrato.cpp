/**
 *  CVibrator Class Source File
 */

// project headers
#include <iostream>

#include "MUSI6106Config.h"

#include "ErrorDef.h"

#include "RingBuffer.h"

#include "Lfo.h"
#include "Vibrato.h"
/**
 *   CVibrato Constructor
 */
CVibrato::CVibrato(): m_bIsInitialized(false), m_pCLfo(0), m_ppCRingBuffer(0), m_fSampleRateInSamples(0), m_fModFreqInSamples(0), m_fWidthInSamples(0), m_fDelayInSamples(0), m_iNumChannels(0)
{
    this->reset();
}
/**
 *   CVibrato Destructor
 */
CVibrato::~CVibrato()
{
    this->reset();
}
/**
 * Static Function allow to Access class function without
 * creating the object
 */
Error_t CVibrato::create(CVibrato*& pCMyProject)
{
    pCMyProject = new CVibrato ();
    
    if(!pCMyProject)
        return kUnknownError;
    
    return kNoError;
}
/**
 * Destroy CVibrato Object
 * Memory Clean
 */
Error_t CVibrato::destroy(CVibrato*& pCMyProject)
{
    if(!pCMyProject)
        return kUnknownError;
    
    pCMyProject->reset();
    
    delete pCMyProject;
    pCMyProject = 0;
    
    return kNoError;
}
/**
 *   CVibrato Class Prameter Initialization
 */
Error_t CVibrato::init(float fSampleRateInHz, float fModFrequencyInHz, float fBasicDelayInSec, int iNumChannels)
{
    this->reset();
    
    if (fSampleRateInHz <=0     ||
        iNumChannels <= 0       ||
        fModFrequencyInHz <0)
        return kFunctionInvalidArgsError;
    
    m_fSampleRateInSamples = fSampleRateInHz;
    m_fModFreqInSamples = fModFrequencyInHz / m_fSampleRateInSamples;
    m_fWidthInSamples = fBasicDelayInSec * m_fSampleRateInSamples;
    m_fDelayInSamples = fBasicDelayInSec * m_fSampleRateInSamples;
    m_iNumChannels = iNumChannels;
    
    m_aafParamRange[kParamModFreq][0] = 0;
    m_aafParamRange[kParamModFreq][1] = m_fSampleRateInSamples / 2;
    m_aafParamRange[kParamWidth][0] = 0;
    m_aafParamRange[kParamWidth][1] = fBasicDelayInSec;
    
    m_pCLfo = new CLfo (m_fModFreqInSamples, m_fSampleRateInSamples);
    m_pCLfo->process();
    
    m_ppCRingBuffer = new CRingBuffer<float>* [m_iNumChannels];
    for (int i = 0; i < m_iNumChannels; i++)
    {
        m_ppCRingBuffer[i] = new CRingBuffer<float> (int(2 + m_fDelayInSamples + m_fWidthInSamples * 2));
    }
    
    m_bIsInitialized = true;
    
    return kNoError;
}
/**
 *   Reset CVibrato Prameters
 */
Error_t CVibrato::reset()
{
    delete m_pCLfo;
    m_pCLfo = 0;
    
    for (int i = 0; i < m_iNumChannels; i++)
    {
        delete m_ppCRingBuffer[i];
    }
    delete [] m_ppCRingBuffer;
    m_ppCRingBuffer = 0;
    
    m_fSampleRateInSamples = 0;
    m_fModFreqInSamples = 0;
    m_fWidthInSamples = 0;
    m_fDelayInSamples = 0;
    m_iNumChannels = 0;
    
    m_bIsInitialized = false;
    
    return kNoError;
}
/**
 *  Parameter Range Check for CVibrato Class
 */
bool CVibrato::isParamInRange(VibratoParam_t eParam, float fParamValue)
{
    if (fParamValue < m_aafParamRange[eParam][0] || fParamValue > m_aafParamRange[eParam][1])
    {
        return false;
    }
    else
    {
        return true;
    }
}
/**
 *  Parameter Setup for CVibrato Class
 */
Error_t CVibrato::setParam(VibratoParam_t eParam, float fParamValue)
{
    if(!m_bIsInitialized)
        return kNotInitializedError;

    if(!isParamInRange(eParam, fParamValue))
        return kFunctionInvalidArgsError;
    
    switch(eParam)
    {
        case kParamModFreq:
            m_fModFreqInSamples = fParamValue / m_fSampleRateInSamples;
            m_pCLfo->setParam(m_fModFreqInSamples);
            break;
        case kParamWidth:
            m_fWidthInSamples = fParamValue * m_fSampleRateInSamples;
            break;
        case kParamDelay:
            m_fDelayInSamples = fParamValue * m_fSampleRateInSamples;
            break;
        case kSampleRate:
            m_fSampleRateInSamples = fParamValue;
            break;
        case kNumVibratoParams:
            return kFunctionInvalidArgsError;
    }
    return kNoError;
}
/**
 *  Return Parameter Values
 */
float CVibrato::getParam(VibratoParam_t eParam) const
{
    if (!m_bIsInitialized)
        return kNotInitializedError;
    
    switch(eParam)
    {
        case kParamModFreq:
            return m_fModFreqInSamples * m_fSampleRateInSamples;
        case kParamWidth:
            return m_fWidthInSamples / m_fSampleRateInSamples;
        case kParamDelay:
            return m_fDelayInSamples / m_fSampleRateInSamples;
        case kSampleRate:
            return m_fSampleRateInSamples;
        case kNumVibratoParams:
            return kFunctionInvalidArgsError;
    }
}
/**
 *  Apply CVibrato on Given Inputs
 */
Error_t CVibrato::process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames)
{
    if (!m_bIsInitialized)
        return kNotInitializedError;
    
    for (int i = 0; i < m_iNumChannels; i++)
    {
        for (int j = 0; j < iNumberOfFrames; j++)
        {
            float fMod = m_pCLfo->returnLfoVal();
            
            float fDelay = 1 + m_fDelayInSamples + m_fWidthInSamples * fMod;
            
            m_ppCRingBuffer[i]->putPostInc(ppfInputBuffer[i][j]);
            
            ppfOutputBuffer[i][j] = m_ppCRingBuffer[i]->get(fDelay);
            
            m_ppCRingBuffer[i]->getPostInc();
        }
    }
    return kNoError;
}
