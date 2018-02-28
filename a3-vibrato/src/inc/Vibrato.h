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
    
    CVibrato();
    
    ~CVibrato();
    
    static Error_t create (CVibrato*& pCVibrato);
    
    static Error_t destroy (CVibrato*& pCVibrato);
    
    Error_t init (float fSampleRateInHz, int iNumChannels); // put arguments here

    Error_t reset();
    
    Error_t setParam(VibratoParam_t eParam, float fParamValue);
    
    float getParam(VibratoParam_t eParam) const;
    
    Error_t process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames);
    
private:
    
    bool m_bIsInitialized;
    
    CLfo *m_pCLfo;
    CRingBuffer<float> *m_pCRingbuffer;
    
    float m_fSampleRateInHz;
    float m_fModFreqInSamples;
    float m_fWidthInSamples;

};

#endif // #if !defined(__Vibrato_hdr__)
