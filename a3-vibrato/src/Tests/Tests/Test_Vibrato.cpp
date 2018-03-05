#include "MUSI6106Config.h"

#ifdef WITH_TESTS
#include <iostream>
#include <cassert>
#include <cstdio>
#include <algorithm>

#include "UnitTest++.h"

#include "Synthesis.h"
#include "Vector.h"
#include "ErrorDef.h"

#include "Vibrato.h"

SUITE(Vibrato)
{
    struct VibratoData
    {
        // e.g., a member vibrato object to be reused in each test
        
        CVibrato* pCVibrato = 0;
        float**   ppfInputSig = 0;
        float**   ppfOutputSig = 0;
        int iNumChannels;
        float fSampleRateInHz;
        float fWidth;
        float fDelay;
        float fModInHz;
        float fTestSigFrequency;
        int iSigLength;
        float fBasicDelay;
        
        VibratoData()
        {
            // setup
            // e.g., allocate a vibrato object and test signal (newly created for each test case)
            iNumChannels = 2;
            fSampleRateInHz = 44100;
            fWidth = 0.001f;
            fModInHz = 15;
            iSigLength = 4096;
            fTestSigFrequency = 1000;
            fBasicDelay = 0.1f;
            fDelay = 1 + fWidth * fSampleRateInHz + fWidth * fSampleRateInHz * fModInHz;
            
            CVibrato::create(pCVibrato);
            pCVibrato->init(fSampleRateInHz, fModInHz, fWidth, iNumChannels);
            
            ppfInputSig = new float*[iNumChannels];
            ppfOutputSig = new float*[iNumChannels];
            
            for (int i = 0; i < iNumChannels; i++)
            {
                ppfInputSig[i] = new float[iSigLength];
                CVectorFloat::setZero(ppfInputSig[i], iSigLength);
                ppfOutputSig[i] = new float[iSigLength];
                CVectorFloat::setZero(ppfOutputSig[i], iSigLength);
            }
            
        }
        
        ~VibratoData()
        {
            // teardown
            // e.g., deallocate the vibrato object and test signal
            CVibrato::destroy(pCVibrato);
            for (int i = 0; i < iNumChannels; i++)
            {
                delete [] ppfInputSig[i];
                delete [] ppfOutputSig[i];
            }
            delete [] ppfInputSig;
            delete [] ppfOutputSig;
            ppfInputSig = 0;
            ppfOutputSig = 0;
        }
        
        void reset()
        {
            for (int i = 0; i < iNumChannels; i++)
            {
                for (int j = 0; j < iSigLength; j++)
                {
                    ppfInputSig[i][j] = 0;
                    ppfOutputSig[i][j] = 0;
                }
            }
        }
        
        // e.g., a reusable process() function
        
        void process()
        {
            pCVibrato->process(ppfInputSig, ppfOutputSig, iSigLength);
        }
        
        
    };
    
    
    
    
    
    
    //    TEST(MyTestWithNoFixture)
    //    {
    //        // e.g., allocate & deallocate local data for testing
    //    }
    
    TEST_FIXTURE(VibratoData, ModAmpEqaulsZero)
    {
        float fModAmp = 0;
        float fNewDelay = 1 + fModAmp * fSampleRateInHz + fModAmp * fSampleRateInHz * fModInHz;
        reset();
        pCVibrato->reset();
        for (int i = 0; i < iNumChannels; i++)
        {
            CSynthesis::generateSine(ppfInputSig[i], fTestSigFrequency, fSampleRateInHz, iSigLength);
        }
        pCVibrato->init(fSampleRateInHz, fModInHz, fModAmp, iNumChannels);
        process();
        for (int i = 0; i < iNumChannels; i++)
        {
            for (int j = 0; j < iSigLength - static_cast<int>(round(fNewDelay)); j++)
            {
                CHECK_CLOSE(ppfInputSig[i][j], ppfOutputSig[i][j+static_cast<int>(round(fNewDelay))], 1e-3);
            }
        }
    }
    
    TEST_FIXTURE(VibratoData, ZeroInput)
    {
        reset();
        pCVibrato->reset();
        pCVibrato->init(fSampleRateInHz, fModInHz, fWidth, iNumChannels);
        process();
        for (int i = 0; i < iNumChannels; i++)
        {
            for (int j = 0; j < iSigLength; j++)
            {
                CHECK_EQUAL(0, ppfOutputSig[i][j]);
            }
        }
    }
    
    TEST_FIXTURE(VibratoData, DCInput)
    {
        reset();
        pCVibrato->reset();
        for (int i = 0; i < iNumChannels; i++)
        {
            CSynthesis::generateDc(ppfInputSig[i], iSigLength);
        }
        pCVibrato->init(fSampleRateInHz, fModInHz, fWidth, iNumChannels);
        process();
        for (int i = 0; i < iNumChannels; i++)
        {
            for (int j = static_cast<int>(round(2*fWidth*fSampleRateInHz)); j < iSigLength; j++)
            {
                CHECK_CLOSE(ppfInputSig[i][j], ppfOutputSig[i][j], 1e-3);
            }
        }
    }
    
    TEST_FIXTURE(VibratoData, ModFreqEqaulsZero)
    {
        float fModFreq = 0;
        reset();
        pCVibrato->reset();
        for (int i = 0; i < iNumChannels; i++)
        {
            CSynthesis::generateSine(ppfInputSig[i], fTestSigFrequency, fSampleRateInHz, iSigLength);
        }
        pCVibrato->init(fSampleRateInHz, fModFreq, fWidth, iNumChannels);
        process();
        for (int i = 0; i < iNumChannels; i++)
        {
            for (int j = 0; j < (iSigLength - static_cast<int>(round(fDelay))); j++)
            {
                CHECK_CLOSE(ppfInputSig[i][j], ppfOutputSig[i][j+static_cast<int>(round(fDelay))], 1e-3);
            }
        }
    }
    
    TEST_FIXTURE(VibratoData, VaryingInputBlockSize)
    {
        int iBlockLength = 512;
        float**   ppfTempInputSig = 0;
        float**   ppfTempOutputSig = 0;
        int iSampleremaining = iSigLength;
        ppfTempInputSig = new float*[iNumChannels];
        ppfTempOutputSig = new float*[iNumChannels];
        for (int i = 0; i < iNumChannels; i++)
        {
            ppfTempInputSig[i] = new float[iBlockLength];
            CVectorFloat::setZero(ppfTempInputSig[i], iBlockLength);
            ppfTempOutputSig[i] = new float[iBlockLength];
            CVectorFloat::setZero(ppfTempOutputSig[i], iBlockLength);
        }
        
        reset();
        pCVibrato->reset();
        for (int i = 0; i < iNumChannels; i++)
        {
            CSynthesis::generateSine(ppfInputSig[i], fTestSigFrequency, fSampleRateInHz, iSigLength);
        }
        pCVibrato->init(fSampleRateInHz, fModInHz, fWidth, iNumChannels);
        process();
        
        while (iSampleremaining > 0)
        {
            for (int i = 0; i < iNumChannels; i++)
            {
                for (int j = 0; j < iBlockLength; j++)
                {
                    ppfTempInputSig[i][j] = ppfInputSig[i][j + iSigLength - iSampleremaining];
                }
                
            }
            VibratoData::pCVibrato->process(ppfTempInputSig, ppfTempOutputSig, iBlockLength);
            for (int i = 0; i < iNumChannels; i++)
            {
                for (int j = 0; j < iBlockLength; j++)
                {
                    CHECK_CLOSE(ppfTempOutputSig[i][j], ppfOutputSig[i][j + iSigLength - iSampleremaining], 1e-3);
                }
            }
            iSampleremaining = iSampleremaining - iBlockLength;
        }
        for (int i = 0; i < iNumChannels; i++) {
            delete[] ppfTempInputSig[i];
            delete[] ppfTempOutputSig[i];
        }
        delete[] ppfTempInputSig;
        delete[] ppfTempOutputSig;
    }
    
    // unit tests
    TEST_FIXTURE(VibratoData, SetAndGetModFreq)
    {
        pCVibrato->reset();
        fSampleRateInHz = 44100;
        fModInHz = 10;
        fWidth = 0.001;
        iNumChannels = 1;
        
        pCVibrato->init(fSampleRateInHz, fModInHz, fWidth, iNumChannels);
        float fModFreq[4] = {-100, 0, 100, 50000};
        float fResults[4] = {10, 0, 100, 100};
        
        for (int i = 0; i < 4; i++)
        {
            pCVibrato->setParam(CVibrato::kParamModFreq, fModFreq[i]);
            CHECK_CLOSE(pCVibrato->getParam(CVibrato::kParamModFreq), fResults[i], 1e-3);
        }
        
    }
    
    TEST_FIXTURE(VibratoData, SetAndGetWidth)
    {
        pCVibrato->reset();
        fSampleRateInHz = 44100;
        fModInHz = 10;
        fBasicDelay = 0.1;
        iNumChannels = 1;
        
        pCVibrato->init(fSampleRateInHz, fModInHz, fBasicDelay, iNumChannels);
        float fWidth[4] = {-100, 0, 0.001, 0.2};
        float fResults[4] = {0.1, 0, 0.001, 0.001};
        
        for(int i = 0;i < 4; i++)
        {
            pCVibrato->setParam(CVibrato::kParamWidth, fWidth[i]);
            float x = pCVibrato->getParam(CVibrato::kParamWidth);
            CHECK_CLOSE(pCVibrato->getParam(CVibrato::kParamWidth), fResults[i], 1e-3);
        }
    }
    
    
    
}


#endif //WITH_TESTS



