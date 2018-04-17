
#include <iostream>
#include <ctime>
#include <assert.h>

#include "MUSI6106Config.h"

#include "AudioFileIf.h"
#include "Ppm.h"

using std::cout;
using std::endl;

// local function declarations
void    showClInfo ();
void numArgTest (int);

/////////////////////////////////////////////////////////////////////////////////
// main function
int main(int argc, char* argv[])
{
    // check command line arguments
    try {
        numArgTest(argc);
    } catch (const char* msg) {
        cout << msg << endl;
    }
    assert (argc == 5);
    
    std::string             sInputFilePath = argv[1];                 //!< file paths
    std::string             sOutputFilePath = argv[2];

    static const int        kBlockSize = atoi(argv[3]);
    const int               kHopSize = atoi(argv[4]);

    clock_t                 time = 0;

    float                   **ppfAudioData = 0;
    float                   *pfOutput = 0;

    CAudioFileIf            *phAudioFile = 0;
    std::fstream            hOutputFile;
    CAudioFileIf::FileSpec_t stFileSpec;
    
    CPpm                    *pCPpm = 0;

    showClInfo();

    //////////////////////////////////////////////////////////////////////////////
    // open the input wave file
    CAudioFileIf::create(phAudioFile);
    phAudioFile->openFile(sInputFilePath, CAudioFileIf::kFileRead);
    if (!phAudioFile->isOpen())
    {
        cout << "Wave file open error!";
        return -1;
    }
    phAudioFile->getFileSpec(stFileSpec);

    //////////////////////////////////////////////////////////////////////////////
    // open the output text file
    hOutputFile.open(sOutputFilePath.c_str(), std::ios::out);
    if (!hOutputFile.is_open())
    {
        cout << "Text file open error!";
        return -1;
    }

    //////////////////////////////////////////////////////////////////////////////
    // allocate memory
    CPpm::createInstance(pCPpm);
    pCPpm->init(kBlockSize, kHopSize, stFileSpec.fSampleRateInHz, stFileSpec.iNumChannels);
    
    ppfAudioData = new float*[stFileSpec.iNumChannels];
    for (int i = 0; i < stFileSpec.iNumChannels; i++)
        ppfAudioData[i] = new float[kBlockSize];
    
    pfOutput = new float[stFileSpec.iNumChannels];

    time = clock();
    //////////////////////////////////////////////////////////////////////////////
    // get audio data and write it to the output file
    while (!phAudioFile->isEof())
    {
        long long iNumFrames = kBlockSize;
        phAudioFile->readData(ppfAudioData, iNumFrames);

//        for (int i = 0; i < iNumFrames; i++)
//        {
//            for (int c = 0; c < stFileSpec.iNumChannels; c++)
//            {
//                hOutputFile << ppfAudioData[c][i] << "\t";
//            }
//            hOutputFile << endl;
//        }
        pCPpm->process(ppfAudioData, pfOutput, iNumFrames);
        hOutputFile << pfOutput[0] << endl;
    }

    cout << "reading/writing done in: \t" << (clock() - time)*1.F / CLOCKS_PER_SEC << " seconds." << endl;

    //////////////////////////////////////////////////////////////////////////////
    // clean-up
    CAudioFileIf::destroy(phAudioFile);
    hOutputFile.close();

    pCPpm->reset();
    CPpm::destroyInstance(pCPpm);
    pCPpm = 0;
    
    for (int i = 0; i < stFileSpec.iNumChannels; i++)
        delete[] ppfAudioData[i];
    delete[] ppfAudioData;
    ppfAudioData = 0;

    return 0;

}


void     showClInfo()
{
    cout << "GTCMT MUSI6106 Executable" << endl;
    cout << "(c) 2014-2018 by Alexander Lerch" << endl;
    cout  << endl;

    return;
}

void numArgTest(int argc)
{
    if (argc != 5)
        throw "Exception: your input arguments are incorrect.\n";
}

