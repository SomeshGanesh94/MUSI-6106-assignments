#if !defined(__Ppm_hdr__)
#define __Ppm_hdr__

#include "ErrorDef.h"

class CPpm
{
public:
    
    enum AlphaType_t
    {
        kAlphaAttack,
        kAlphaRelease,
        
        kNumAlphaTypes
    };
    
    static Error_t createInstance (CPpm*& pCPpm);
    static Error_t destroyInstance (CPpm*& pCPpm);
    
    Error_t init(int iBlockSize, int iHopSize, float fSampleRateInHz, int iNumChannels);
    Error_t reset();
    
    Error_t process (float **ppfInputBuffer, float *pfOutputBuffer, int iNumberOfFrames);
    
protected:
    CPpm ();
    virtual ~CPpm ();
    
private:
    
    bool m_bIsInitialized;
    
    float m_fAlpha[kNumAlphaTypes];
    int m_iBlockSize;
    int m_iHopSize;
    float m_fSampleRateInHz;
    int m_iNumChannels;
    float m_fFilterBuf;
    
    float *m_pfVtemp;
};

#endif // #if !defined(__Ppm_hdr__)

