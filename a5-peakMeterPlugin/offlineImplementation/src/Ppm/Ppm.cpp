#include "ErrorDef.h"
#include "Ppm.h"
#include <math.h>

Error_t CPpm::createInstance(CPpm *&pCPpm)
{
    pCPpm = new CPpm ();
    
    if (!pCPpm)
        return kUnknownError;
    
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

Error_t CPpm::init(int iBlockSize, int iHopSize, float fSampleRateInHz, int iNumChannels)
{
    m_iBlockSize = iBlockSize;
    m_iHopSize = iHopSize;
    m_fFilterBuf = 0;
    m_fSampleRateInHz = fSampleRateInHz;
    m_iNumChannels = iNumChannels;
    
    m_pfVtemp = new float[m_iBlockSize];
    
    m_fAlpha[kAlphaAttack] = 1 - exp(-2.2 / (m_fSampleRateInHz * 0.01));
    m_fAlpha[kAlphaRelease] = 1 - exp(-2.2 / (m_fSampleRateInHz * 1.5));
    //    m_fAlpha[kAlphaAttack] = fAlpha[kAlphaAttack];
    //    m_fAlpha[kAlphaRelease] = fAlpha[kAlphaRelease];
    
    m_bIsInitialized = true;
    
    return kNoError;
}

Error_t CPpm::reset()
{
    delete [] m_pfVtemp;
    
    m_iBlockSize = 0;
    m_iHopSize = 0;
    m_fFilterBuf = 0;
    m_fSampleRateInHz = 0;
    m_iNumChannels = 0;
    m_fAlpha[kAlphaAttack] = 0;
    m_fAlpha[kAlphaRelease] = 0;
    
    m_bIsInitialized = false;
    
    return kNoError;
}

Error_t CPpm::process(float **ppfInputBuffer, float *pfOutputBuffer, int iNumberOfFrames)
{
    m_fFilterBuf = m_pfVtemp[m_iHopSize-1];
    float fMaxValue = 0;
    
    for (int iChannel = 0; iChannel < m_iNumChannels; iChannel++)
    {
        for (int iSample = 0; iSample < iNumberOfFrames; iSample++)
        {
            if (m_fFilterBuf > abs(ppfInputBuffer[iChannel][iSample]))
            {
                m_pfVtemp[iSample] = (1 - m_fAlpha[kAlphaRelease]) * m_fFilterBuf;
            }
            else
            {
                m_pfVtemp[iSample] = m_fAlpha[kAlphaAttack] * abs(ppfInputBuffer[iChannel][iSample]) + (1 - m_fAlpha[kAlphaAttack]) * m_fFilterBuf;
            }
            m_fFilterBuf = m_pfVtemp[iSample];
            
            if (fMaxValue < m_pfVtemp[iSample])
            {
                fMaxValue = m_pfVtemp[iSample];
            }
        }
        pfOutputBuffer[iChannel] = fMaxValue;
    }
    return kNoError;
}

CPpm::CPpm()
{
    m_bIsInitialized = false;
}

CPpm::~CPpm()
{
    
}

