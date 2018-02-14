// standard headers
#include <limits>
#include <iostream>

// project headers
#include "MUSI6106Config.h"

#include "ErrorDef.h"
#include "Util.h"
#include "RingBuffer.h"

#include "CombFilterIf.h"
#include "CombFilter.h"

#define MIN_DELAY_TIME_IN_FRAMES 1.0f
#define MIN_IIR_GAIN (-1.0f)
#define MAX_IIR_GAIN (1.0f)
#define DEFAULT_DELAY_TIME_IN_FRAMES 10.0f
#define DEFAULT_GAIN 0.5f


CCombFilterBase::CCombFilterBase( int iMaxDelayInFrames, int iNumChannels ) :
m_ppCRingBuffer(0),
m_iNumChannels(iNumChannels)
{
    assert(iMaxDelayInFrames > 0);
    assert(iNumChannels > 0);
    
    // Parameter range set for FIR filters
    CCombFilterBase::m_aafParamRange[0][0] = std::numeric_limits<float>::lowest();
    CCombFilterBase::m_aafParamRange[0][1] = std::numeric_limits<float>::max();
    CCombFilterBase::m_aafParamRange[1][0] = MIN_DELAY_TIME_IN_FRAMES;
    CCombFilterBase::m_aafParamRange[1][1] = iMaxDelayInFrames;
    CCombFilterBase::m_afParam[0] = DEFAULT_GAIN;
    CCombFilterBase::m_afParam[1] = DEFAULT_DELAY_TIME_IN_FRAMES;
    
    isUsingDefaultParams[0] = true;
    isUsingDefaultParams[1] = true;
    isFirstTimeProcess = true;
    
    // Allocate memory for ring buffer
    m_ppCRingBuffer = new CRingBuffer<float>* [iNumChannels];
    for(int i = 0; i < iNumChannels; i++)
    {
        m_ppCRingBuffer[i] = new CRingBuffer<float>(iMaxDelayInFrames);
    }
    
}

CCombFilterBase::~CCombFilterBase()
{
    // Deallocating memory to avoid memory leak
    
    for(int i = 0; i < m_iNumChannels; i++)
    {
        delete m_ppCRingBuffer[i];
    }
    delete [] m_ppCRingBuffer;
    m_ppCRingBuffer = 0;
}

Error_t CCombFilterBase::resetInstance()
{
    for(int i = 0; i < m_iNumChannels; i++)
    {
        m_ppCRingBuffer[i]->reset();
    }
    return kNoError;
}

Error_t CCombFilterBase::setParam( CCombFilterIf::FilterParam_t eParam, float fParamValue )
{
    if (!isInParamRange(eParam, fParamValue))
    {
        std::cout << "Warning: " << (eParam ? "Delay in frames" : "Gain") << " out of bounds and was set to default value: "
        << (eParam ? DEFAULT_DELAY_TIME_IN_FRAMES : DEFAULT_GAIN) << "." << std::endl;
        return kFunctionInvalidArgsError;
    }
    
    // Sets value for given filter parameter
    if (eParam == CCombFilterIf::FilterParam_t::kParamDelay)
    {
        isUsingDefaultParams[1] = false;
        fParamValue = floor(fParamValue);
        CCombFilterBase::m_afParam[eParam] = fParamValue;
        for(int i = 0; i < m_iNumChannels; i++)
        {
            m_ppCRingBuffer[i]->setWriteIdx(CUtil::float2int<int>(fParamValue));
            m_ppCRingBuffer[i]->setReadIdx(0);
        }
    }
    else
    {
        isUsingDefaultParams[0] = false;
        CCombFilterBase::m_afParam[eParam] = fParamValue;
    }
    
    return kNoError;
}

float CCombFilterBase::getParam( CCombFilterIf::FilterParam_t eParam ) const
{
    // Returns value of desired filter parameter
    return m_afParam[eParam];
}

bool CCombFilterBase::isInParamRange( CCombFilterIf::FilterParam_t eParam, float fValue )
{
    if (fValue < m_aafParamRange[eParam][0] || fValue > m_aafParamRange[eParam][1])
    {
        return false;
    }
    else
    {
        return true;
    }
}

Error_t CCombFilterFir::process( float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames )
{
    if (isFirstTimeProcess)
    {
        if (isUsingDefaultParams[0])
            std::cout << "Warning: Using default value (0.5f) for Gain." << std::endl;
        if (isUsingDefaultParams[1])
            std::cout << "Warning: Using default values (10.0f) for Delay time in frames." << std::endl;
        isFirstTimeProcess = false;
    }
    
    for(int i = 0; i < CCombFilterBase::m_iNumChannels; i++)
    {
        for(int j = 0; j < iNumberOfFrames; j++)
        {
            ppfOutputBuffer[i][j] = ppfInputBuffer[i][j] + CCombFilterBase::m_afParam[0] * CCombFilterBase::m_ppCRingBuffer[i]->getPostInc();
            CCombFilterBase::m_ppCRingBuffer[i]->putPostInc(ppfInputBuffer[i][j]);
        }
        
    }
    
    return kNoError;
}


CCombFilterIir::CCombFilterIir (int iMaxDelayInFrames, int iNumChannels) : CCombFilterBase(iMaxDelayInFrames, iNumChannels)
{
    CCombFilterBase::m_aafParamRange[0][0] = MIN_IIR_GAIN;
    CCombFilterBase::m_aafParamRange[0][1] = MAX_IIR_GAIN;
}

Error_t CCombFilterIir::process( float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames )
{
    if (isFirstTimeProcess)
    {
        if (isUsingDefaultParams[0])
            std::cout << "Warning: Using default value (0.5f) for Gain." << std::endl;
        if (isUsingDefaultParams[1])
            std::cout << "Warning: Using default values (10.0f) for Delay time in frames." << std::endl;
        isFirstTimeProcess = false;
    }
    
    for(int i = 0; i < CCombFilterBase::m_iNumChannels; i++)
    {
        for(int j = 0; j < iNumberOfFrames; j++)
        {
            ppfOutputBuffer[i][j] = ppfInputBuffer[i][j] + CCombFilterBase::m_afParam[0] * CCombFilterBase::m_ppCRingBuffer[i]->getPostInc();
            CCombFilterBase::m_ppCRingBuffer[i]->putPostInc(ppfOutputBuffer[i][j]);
        }
        
    }
    
    return kNoError;
}
