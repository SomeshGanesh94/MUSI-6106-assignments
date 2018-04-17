#include "ErrorDef.h"
#include "Ppm.h"

Error_t CPpm::createInstance(CPpm *&pCPpm)
{
    pCPpm = new CPpm ();

    if (!pCPpm)
        return kUnknownError;

    m_bIsInitialized = false;
    
    return kNoError;
}

Error_t CPpm::destroyInstance(CPpm *&pCPpm)
{
    if (!pCPpm)
        return kUnknownError;

    delete pCPpm;
    pCPpm = 0;

    return kNoError;
}

Error_t CPpm::init(float fAlpha[], int iBlockSize, float fSampleRateInHz, int iNumChannels)
{
    m_iBlockSize = iBlockSize;
    m_fSampleRateInHz = fSampleRateInHz;
    m_iNumChannels = iNumChannels;
    
    m_pfVppm = new float[m_iBlockSize];
    
    m_fAlpha[kAlphaAttack] = fAlpha[kAlphaAttack];
    m_fAlpha[kAlphaRelease] = fAlpha[kAlphaRelease];
    
    m_bIsInitialized = true;
    
    return kNoError;
}

Error_t CPpm::reset()
{
    delete [] m_pfVppm;
    
    m_iBlockSize = 0;
    m_fSampleRateInHz = 0;
    m_iNumChannels = 0;
    m_fAlpha[kAlphaAttack] = 0;
    m_fAlpha[kAlphaRelease] = 0;
    
    m_bIsInitialized = false;
    
    return kNoError;
}

Error_t CPpm::process(float **ppfInputBuffer, int iNumberOfFrames)
{
    return kNoError;
}

CPpm::CPpm()
{
    
}

CPpm::~CPpm()
{
    
}
