#if !defined(__Vibrato_hdr__)
#define __Vibrato_hdr__

#include "ErrorDef.h"

// forward declaration
class CLfo;
template <class T>
class CRingBuffer;

/*
 * brief explanation about your class-interface design
 */

class CVibrato
{
public:
    
    enum VibratoParam_t
    {
        kParamWidth,
        kParamModFreq,
        kSampleRate,
        
        kNumVibratoParams
    };
    
    static Error_t create (CVibrato*& pCVibrato);
    
    static Error_t destroy (CVibrato*& pCVibrato);
    
    Error_t init (float fSampleRateInHz, float fModFrequencyInHz, float fWidthInHz, int iNumChannels);
    
    Error_t reset();
    
    Error_t setParam(VibratoParam_t eParam, float fParamValue);
    
    float getParam(VibratoParam_t eParam) const;
    
    Error_t process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames);
    
protected:
    
    CVibrato();
    
    ~CVibrato();
    
private:
    
    bool m_bIsInitialized;
    
    CLfo *m_pCLfo;
    CRingBuffer<float> **m_ppCRingBuffer;
    
    float m_fSampleRateInSamples;
    float m_fModFreqInSamples;
    float m_fWidthInSamples;
    
    int   m_iNumChannels;
};

#endif // #if !defined(__Vibrato_hdr__)
