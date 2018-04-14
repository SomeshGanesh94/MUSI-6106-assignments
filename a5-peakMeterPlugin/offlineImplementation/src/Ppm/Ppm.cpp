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

Error_t CPpm::init()
{
    m_bIsInitialized = true;
}

Error_t CPpm::reset()
{
    m_bIsInitialized = false;
}

CPpm::CPpm()
{

}

CPpm::~CPpm()
{

}

Error_t CPpm::process(float **ppfInputBuffer, int iNumberOfFrames)
{
    return kNoError;
}
