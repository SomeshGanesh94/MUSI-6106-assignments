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
            CHECK_CLOSE(0.5F, m_pfOutputData[i], 1e-3);

    }

    TEST_FIXTURE(PpmData, SinewaveInput)
    {
        int iBlockSize = 1024;
        int iHopSize = iBlockSize;
        float fSinewaveFreq = 200.2F;

        for (int i = 0; i < iNumChannels; i++)
            CSynthesis::generateSine (m_ppfData[i], fSinewaveFreq, fSampleFreq, iDataLength, 1.F, 0.F);

        m_pCPpm->init(iBlockSize, iHopSize, fSampleFreq, iNumChannels);
        m_pCPpm->process(m_ppfData, m_pfOutputData, iDataLength);
        for (int i = 0; i < iNumChannels; i++)
            CHECK_CLOSE(.96F, m_pfOutputData[i], 1e-3);
    }

    TEST_FIXTURE(PpmData, AudioInput)
    {
        CAudioFileIf                *phAudioFile = 0;
        CAudioFileIf::FileSpec_t    stFileSpec;

        int                         kBlockSize = 1024;
        const int                   iExpectedBlocks = 104;
        float                       fExpectedDataCh1[iExpectedBlocks] = {0.534904651611038, 0.635411713242318, 0.633681220246789, 0.612590871604550, 0.988463668696079, 0.992292865015030, 0.994034376542397, 0.991892083559708, 0.981274529760354, 0.989411803730772, 0.991595774649010, 0.990163601894918, 0.990745406917540, 0.989278557442844, 0.985194161279649, 0.970542432067218, 0.938046189218333, 0.906638003691214, 0.876281444543949, 0.846941300635745, 0.818583539784850, 0.791175269293999, 0.764684697797555, 0.978794763220991, 0.996133660069394, 0.997872383969395, 0.998014379317528, 0.997882819486646, 0.997750078976589, 0.992226747376389, 0.989864791211411, 0.988807617212225, 0.985150043021720, 0.997777654612166, 0.998466624419042, 0.997796160499554, 0.997795125213931, 0.997385445863575, 0.993130279491724, 0.989865749769984, 0.988650214910613, 0.986760715664632, 0.996459108073508, 0.995810582305854, 0.995811384799399, 0.995590154556862, 0.996511159678172, 0.997249267568388, 0.997626784334688, 0.996909624313346, 0.997410257085792, 0.997638377510178, 0.996985666588137, 0.997889853367193, 0.997542670914180, 0.997248763527391, 0.995861971714517, 0.997465613120149, 0.996013651412364, 0.990890210531147, 0.995709246547919, 0.997650582024594, 0.998244905857258, 0.997019587171408, 0.992573570485146, 0.990053714817759, 0.986821823418160, 0.990381180437250, 0.987922957840816, 0.982489253329486, 0.974840467880799, 0.973811188013938, 0.957754960842655, 0.948272193009914, 0.934744897812848, 0.916542995864014, 0.902640502762842, 0.872417790170520, 0.843207011292277, 0.814974283999280, 0.982482824547066, 0.993547154662892, 0.977673467851134, 0.945977591220467, 0.914303842069258, 0.883690611047234, 0.854102389295089, 0.825504856880944, 0.797864842992012, 0.771150287459154, 0.978520970949906, 0.978580182530822, 0.978226277939860, 0.980655170804340, 0.989500370162946, 0.991173135465214, 0.985716157867265, 0.987989288153034, 0.981965029007539, 0.988090282807964, 0.996377955584692, 0.995115684297615, 0.973585389226575, 0.940987260389443};
        float                       fExpectedDataCh2[iExpectedBlocks] = {0.0145398802312916, 0.0170872616608835, 0.0170412926155760, 0.0164958412699887, 0.0869752081444675, 0.0857332386828111, 0.0830362336688942, 0.0802559681952727, 0.0775687931204118, 0.0749715915396765, 0.0724613509103704, 0.0700351595574315, 0.0676902032961260, 0.0654237621678223, 0.0632332072850596, 0.0611159977822537, 0.0590696778684980, 0.0570918739790468, 0.0551802920221721, 0.0533327147182045, 0.0515469990276690, 0.0498210736655325, 0.0481529366986831, 0.0835465036503289, 0.0840585581087378, 0.0830593678101696, 0.0802783277463310, 0.0796502908804121, 0.0794809358670560, 0.0768197108567379, 0.0742475904660153, 0.0717615912443336, 0.0693588296346930, 0.0673870832495067, 0.0667559439848612, 0.0691049396148393, 0.0733102556529275, 0.0755793805041652, 0.0743576980121700, 0.0718680121082361, 0.0695744265039712, 0.0672448967636496, 0.0951374158611109, 0.0947049261943842, 0.0915339630515945, 0.0884691718647632, 0.0855069977252630, 0.0826666656084108, 0.0798987742186478, 0.0792313702822747, 0.0785699805407601, 0.0770502349012642, 0.0757040003818199, 0.0744834468179295, 0.0731834254808164, 0.0709970619833324, 0.0686467625492040, 0.0663482933706464, 0.0644151114608298, 0.0625842587286853, 0.0604887776832735, 0.0901616109414798, 0.0912688538616843, 0.0882582051249084, 0.0853030946914190, 0.0824469289131240, 0.0796863948699096, 0.0770182905667013, 0.0744395212194092, 0.0719470956652288, 0.0695381228931374, 0.0672098086905551, 0.0649594524022852, 0.0627844437979741, 0.0606822600444566, 0.0586504627794740, 0.0566866952833722, 0.0547886797454985, 0.0529542146221270, 0.0511811720828459, 0.0692838032150671, 0.0691479873371499, 0.0668399768317631, 0.0646020034599936, 0.0624389631604687, 0.0603483469822833, 0.0583277299806258, 0.0563747684040389, 0.0544871969758580, 0.0526628262666752, 0.0657477975851524, 0.0651535504479684, 0.0629953033700652, 0.0608860594987875, 0.0588474385068438, 0.0568770757596117, 0.0558556159222211, 0.0546220503779052, 0.0529690348960653, 0.0767335145230486, 0.0777526853370549, 0.0759694670191194, 0.0734258149666323, 0.0709673308904128};
        int                         iCurrentBlockNum = 0;

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
