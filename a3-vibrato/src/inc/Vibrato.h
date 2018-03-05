/**
 *  CVibrato Class Header File
 */

#if !defined(__Vibrato_hdr__)
#define __Vibrato_hdr__

#include "ErrorDef.h"

// forward declaration
class CLfo;
template <class T>
class CRingBuffer;

/*
 * Design choices for Vibrato class interface:
 We implemented an interface similar to the comb filter class with a standard interface for users or developers to interact with. Trying to be consistent with other code in this repository, we used static methods for creating and destroying instances of the class and kept the constructors and destructors in the protected section of the class. The init() function is the place to basically allocate memory, set parameter ranges and the parameters themselves initially since some of the ranges depend on the parameters. A standard reset() function deletes all data and sets values to 0. We have the usual setParam and getParam functions to set and acccess values within the parameter ranges. The setParam function accordingly calls some functions in the LFO class as well to ensure that any parameter changes here don't break the code or make it not work in the right way. The final process() function just pulls values from the LFO buffer and performs simple operations to get the final result and store it in an output buffer. The parameter ranges values and function are not kept public so that they cannot be tampered with and can only be set on initialization. The core variables for the class are kept private.
 */

class CVibrato
{
public:
    /**
     *  Parameters for CVibrato Class
     */
    enum VibratoParam_t
    {
        kParamWidth,
        kParamModFreq,
        kParamDelay,
        kSampleRate,
        
        kNumVibratoParams
    };
    /**
     * Static Function allow to Access class function without
     * creates a new CVibrato Instance
     */
    static Error_t create (CVibrato*& pCVibrato);
    /**
     * Destroy the CVibrato Instance
     * Memory Clean
     */
    static Error_t destroy (CVibrato*& pCVibrato);
    /**
     *   CVibrato Class Prameter Initialization
     *   param fSampleRateInHz: Sample Rate in Hz
     *   param fModFrequencyInHz: Modulation Frequency in Hz
     *   param fWidthInHz: Basic Delay in sec
     *   param iNumChannels: Number of Channels
     */
    Error_t init (float fSampleRateInHz, float fModFrequencyInHz, float fWidthInHz, int iNumChannels);
    /**
     *   Reset CVibrato Prameters
     */
    Error_t reset();
    /**
     *  Parameter Setup for CVibrato Class
     *  param eParam: parameter to set
     *  param fParamValue: value to set to
     */
    Error_t setParam(VibratoParam_t eParam, float fParamValue);
    /**
     *  Return Parameter Values
     *  param eParam: paramter to get
     */
    float getParam(VibratoParam_t eParam) const;
    /**
     *  Apply Vibrato on One Block of Given Inputs
     *   param ppfInputBuffer: Input Buffer [iNumberOfChannels] * [iNumberOfFrames]
     *   param ppfOutputBuffer: Output Buffer [iNumberOfChannels] * [iNumberOfFrames]
     *   param iNumberOfFrames: Number of frames
     */
    Error_t process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames);
    
protected:
    /**
     *   CVibrato Constructor
     */
    CVibrato();
    /**
     *   CVibrato Destructor
     */
    ~CVibrato();

    float   m_aafParamRange[kNumVibratoParams][2];
    
private:
    
    bool m_bIsInitialized;
    
    bool isParamInRange(VibratoParam_t eParam, float fParamValue);
    /**
     *  LFO and Ringbuffer
     */
    CLfo *m_pCLfo;
    CRingBuffer<float> **m_ppCRingBuffer;
    /**
     *  CVibrato Class Parameters
     */
    float m_fSampleRateInSamples;
    float m_fModFreqInSamples;
    float m_fWidthInSamples;
    float m_fDelayInSamples;
    
    int   m_iNumChannels;
};

#endif // #if !defined(__Vibrato_hdr__)
