#include "ErrorDef.h"
#include "Ppm.h"
#include <math.h>
#include <iostream>
using namespace std;

Error_t CPpm::createInstance(CPpm *&pCPpm)
{
    pCPpm = new CPpm ();
    
    if (!pCPpm)
        return kUnknownError;
    
    pCPpm->m_bIsInitialized = false;
    
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
    
    m_ppfVtemp = new float*[50];
    for (int iChannel = 0; iChannel < 50; iChannel++)
    {
        m_ppfVtemp[iChannel] = new float[m_iBlockSize];
    }
    m_fAlpha[kAlphaAttack] = 1 - exp(-2.2 / (m_fSampleRateInHz * 0.01));
    m_fAlpha[kAlphaRelease] = 1 - exp(-2.2 / (m_fSampleRateInHz * 1.5));
    
    m_bIsInitialized = true;
    
    return kNoError;
}

Error_t CPpm::reset()
{
    for (int iChannel = 0; iChannel < 50; iChannel++)
    {
        delete m_ppfVtemp[iChannel];
    }
    delete [] m_ppfVtemp;
    
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
    if (m_bIsInitialized)
    {
        for (int iChannel = 0; iChannel < m_iNumChannels; iChannel++)
        {
            m_fFilterBuf = m_ppfVtemp[iChannel][m_iHopSize-1];
            float fMaxValue = 0;
            for (int iSample = 0; iSample < iNumberOfFrames; iSample++)
            {
                if (m_fFilterBuf > abs(ppfInputBuffer[iChannel][iSample]))
                {
                    m_ppfVtemp[iChannel][iSample] = (1 - m_fAlpha[kAlphaRelease]) * m_fFilterBuf;
                }
                else
                {
                    m_ppfVtemp[iChannel][iSample] = m_fAlpha[kAlphaAttack] * abs(ppfInputBuffer[iChannel][iSample]) + (1 - m_fAlpha[kAlphaAttack]) * m_fFilterBuf;
                }
                m_fFilterBuf = m_ppfVtemp[iChannel][iSample];
                
                if (fMaxValue < m_ppfVtemp[iChannel][iSample])
                {
                    fMaxValue = m_ppfVtemp[iChannel][iSample];
                }
            }
            pfOutputBuffer[iChannel] = fMaxValue;
            std::cout << pfOutputBuffer[iChannel] << " ";
        }
        std::cout << std::endl;
        return kNoError;
    }
    else
    {
        return kNotInitializedError;
    }
}

Error_t CPpm::setNumChannels(int iChannels)
{
    if (iChannels > 0)
    {
        m_iNumChannels = iChannels;
        return kNoError;
    }
    else
    {
        return kFunctionInvalidArgsError;
    }
}

bool CPpm::isInitialized()
{
    return m_bIsInitialized;
}

CPpm::CPpm()
{

}

CPpm::~CPpm()
{
    
}


