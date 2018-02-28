#if !defined(__Lfo_hdr__)
#define __Lfo_hdr__

#define _USE_MATH_DEFINES
#include <math.h>

#include "ErrorDef.h"
#include "Synthesis.h"

class CLfo
{
public:
    CLfo (int bufferLength, float modFreq, float sampleRateInHz){
        m_pCRingBuffer = new CRingBuffer<float>(bufferLength);
        float waveTable[bufferLength];
        CSynthesis::generateSine(waveTable, modFreq, sampleRateInHz, bufferLength);
        for (int i = 0; i < bufferLength; i++) {
            m_pCRingBuffer-> putPostInc(waveTable[i]);
        }
        
    }
    ~CLfo() {
        delete [] m_pCRingBuffer;
    }
    float returnFreqVal(){
        return m_pCRingBuffer->getPostInc();
    }
    
private:
    CRingBuffer<float>  *m_pCRingBuffer;
    
};

#endif // __Lfo_hdr__
