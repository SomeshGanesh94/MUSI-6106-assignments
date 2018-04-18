#include "MUSI6106Config.h"

#ifdef WITH_TESTS
#include <cassert>

#include "UnitTest++.h"
#include "Synthesis.h"
#include "AudioFileIf.h"
#include "Ppm.h"


SUITE(Ppm)
{
    struct PpmData
    {
        PpmData()
        {
            CPpm::createInstance(m_pCPpm);
            
            m_ppfData = new float*[iNumChannels];
            for (int i = 0; i < iNumChannels; i++)
            {
                m_ppfData[i] = new float[iDataLength];
                memset(m_ppfData[i], 0, sizeof(float) * iDataLength);
            }
            m_pfOutputData = new float[iNumChannels];
            memset(m_pfOutputData, 0, sizeof(float) * iNumChannels);
        }
        
        ~PpmData()
        {
            for (int i = 0; i < iNumChannels; i++)
                delete []  m_ppfData[i];
            delete []  m_ppfData;
            m_ppfData = 0;
        
            delete [] m_pfOutputData;
            m_pfOutputData = 0;
            
            m_pCPpm->reset();
            CPpm::destroyInstance(m_pCPpm);
        }
        
        const int         iDataLength = 44100;
        const int         iNumChannels = 2;
        const float       fSampleFreq = 44100.F;
        
        CPpm        *m_pCPpm = NULL;
        float       **m_ppfData = NULL;
        float       *m_pfOutputData = NULL;
    };
    
    
    //Low-level unit tests
    
    TEST_FIXTURE(PpmData, Api)
    {
        CHECK_EQUAL(kFunctionInvalidArgsError, m_pCPpm->init(-1, 4, 1, 2));
        m_pCPpm -> reset();
        CHECK_EQUAL(kFunctionInvalidArgsError, m_pCPpm->init(3, 0, 44100.2, 2));
        m_pCPpm -> reset();
        CHECK_EQUAL(kNoError, m_pCPpm->init(1024, 256, fSampleFreq, iNumChannels));
        m_pCPpm -> reset();
    }

    TEST_FIXTURE(PpmData, WithoutInitialization)
    {
        CHECK_EQUAL(kNotInitializedError, m_pCPpm->process(m_ppfData, m_pfOutputData, iDataLength));
//        m_pCPpm -> reset();
    }

    //High-level validation tests

    TEST_FIXTURE(PpmData, ZeroInput)
    {
        int iBlockSize = 512;
        int iHopSize = iBlockSize;

        m_pCPpm->init(iBlockSize, iHopSize, fSampleFreq, iNumChannels);
        m_pCPpm->process(m_ppfData, m_pfOutputData, iDataLength);
        for (int i = 0; i < iNumChannels; i++)
            CHECK_CLOSE(0.F, m_pfOutputData[i], 1e-3);
    }
    
    

    TEST_FIXTURE(PpmData, DCInput)
    {
        int iBlockSize = 256;
        int iHopSize = iBlockSize;

        for (int i = 0; i < iNumChannels; i++)
            CSynthesis::generateDc (m_ppfData[i], iDataLength, 0.5F);

        m_pCPpm->init(iBlockSize, iHopSize, fSampleFreq, iNumChannels);
        m_pCPpm->process(m_ppfData, m_pfOutputData, iDataLength);
        for (int i = 0; i < iNumChannels; i++)
            CHECK_CLOSE(0.496F, m_pfOutputData[i], 1e-3);

    }

    TEST_FIXTURE(PpmData, SinewaveInput)
    {
        int iBlockSize = 1024;
        int iHopSize = iBlockSize;
        float fSinewaveFreq = 2000.2F;

        for (int i = 0; i < iNumChannels; i++)
            CSynthesis::generateSine (m_ppfData[i], fSinewaveFreq, fSampleFreq, iDataLength, 1.F, 0.F);

        m_pCPpm->init(iBlockSize, iHopSize, fSampleFreq, iNumChannels);
        m_pCPpm->process(m_ppfData, m_pfOutputData, iDataLength);
        for (int i = 0; i < iNumChannels; i++)
            CHECK_CLOSE(1.F, m_pfOutputData[i], 1e-3);
    }

    TEST_FIXTURE(PpmData, AudioInput)
    {
        CAudioFileIf                *phAudioFile = 0;
        CAudioFileIf::FileSpec_t    stFileSpec;

        int                         kBlockSize = 1024;
        const int                   iExpectedBlocks = 8;
        float                       fExpectedDataCh1[iExpectedBlocks] = {0.541717529296875, 0.816558837890625, 0.808593750000000, 0.494659423828125, 0.344970703125000, 0.388488769531250, 0.370483398437500, 0.0796203613281250};
        float                       fExpectedDataCh2[iExpectedBlocks] = {0.194394887725952, 0.443898168955833, 0.516327633154901, 0.514099495008026, 0.468038309330244, 0.426104015132895, 0.387926860030305, 0.353170219919278};
        int                         iCurrentBlockNum = 1;

        CAudioFileIf::create(phAudioFile);
        phAudioFile->openFile("../../inc/testAudio.wav", CAudioFileIf::kFileRead);
        phAudioFile->getFileSpec(stFileSpec);

        CHECK_EQUAL(2, stFileSpec.iNumChannels);
        m_pCPpm->init(kBlockSize, kBlockSize, stFileSpec.fSampleRateInHz, stFileSpec.iNumChannels);
        while (!phAudioFile->isEof())
        {
            long long iNumFrames = kBlockSize;
            phAudioFile->readData(m_ppfData, iNumFrames);
            m_pCPpm->process(m_ppfData, m_pfOutputData, iNumFrames);

            CHECK_CLOSE(fExpectedDataCh1[iCurrentBlockNum], m_pfOutputData[0], 1e-3);
            CHECK_CLOSE(fExpectedDataCh2[iCurrentBlockNum], m_pfOutputData[1], 1e-3);

            iCurrentBlockNum ++;
        }
        CHECK_EQUAL(iExpectedBlocks, iCurrentBlockNum);

        CAudioFileIf::destroy(phAudioFile);
    }
    
}

#endif //WITH_TESTS
