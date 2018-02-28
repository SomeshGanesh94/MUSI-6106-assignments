
// standard headers

// project headers
#include "MUSI6106Config.h"

#include "ErrorDef.h"

#include "RingBuffer.h"

#include "Lfo.h"
#include "Vibrato.h"

CVibrato::CVibrato(): m_bIsInitialized(false), m_pCLfo(0), m_pCRingbuffer(0), m_fSampleRateInHz(0)
{
    this->reset();
    
}

CVibrato::~CVibrato()
{
    this->reset();
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

Error_t CVibrato::init(float fSampleRateInHz, int iNumChannels)
{
    this->reset();
    
    if (fSampleRateInHz <=0 ||
        iNumChannels <= 0)
        return kFunctionInvalidArgsError;
    
    m_fSampleRateInHz = fSampleRateInHz;
    m_bIsInitialized = true;
    
    return kNoError;
}

Error_t CVibrato::reset()
{
    delete m_pCRingbuffer;
    m_pCRingbuffer = 0;
    delete m_pCLfo;
    m_pCLfo = 0;
    
    m_fSampleRateInHz = 0;
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
            m_fModFreqInSamples = round(fParamValue / m_fSampleRateInHz);
        case kParamWidth:
            m_fWidthInSamples = fParamValue;
        case kSampleRate:
            m_fSampleRateInHz = fParamValue;
    }
    return kNoError;
}

Error_t CVibrato::process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames)
{
    if (!m_bIsInitialized)
        return kNotInitializedError;
    
    // m_pCLfo->process(**ppfInputBuffer, **ppfOutputBuffer, iNumberOfFrames);
    return kNoError;
}
