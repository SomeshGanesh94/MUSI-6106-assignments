#if !defined(__Ppm_hdr__)
#define __Ppm_hdr__

#include "ErrorDef.h"

class CPpm
{
public:
    
    enum AlphaType_t
    {
        kAlphaAttack,
        kAlphaRelease
    };
    
    static Error_t createInstance (CPpm*& pCPpm);
    static Error_t destroyInstance (CPpm*& pCPpm);
    
    Error_t init();
    Error_t reset();
    
    Error_t process (float **ppfInputBuffer, int iNumberOfFrames);
    
protected:
    CPpm ();
    virtual ~CPpm ();

private:
    
    bool m_bIsInitialized;
    
    float m_fAlpha[2];
    float m_fSampleRateInHz;
};

#endif // #if !defined(__Ppm_hdr__)
