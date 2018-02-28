
#include <iostream>
#include <ctime>

#include "MUSI6106Config.h"

#include "AudioFileIf.h"
#include "Fft.h"
#include "RingBuffer.h"

using std::cout;
using std::endl;

// local function declarations
void    showClInfo ();

/////////////////////////////////////////////////////////////////////////////////
// main function
int main(int argc, char* argv[])
{
    std::string             sInputFilePath,                 //!< file paths
        sOutputFilePath;

    static const int        kBlockSize = atoi(argv[2]);
    static const int        kHopSize   = atoi(argv[3]);

    clock_t                 time = 0;

    float                   **ppfAudioData = 0;
    float                   **ppfOutAudioData = 0;

    CAudioFileIf            *phAudioFile = 0;
    std::fstream            hOutputFile;
    CAudioFileIf::FileSpec_t stFileSpec;
    CFft                    *pCFft = 0;
    CRingBuffer<float>      *pCRingBuffer = 0;
    
    showClInfo();
    //////////////////////////////////////////////////////////////////////////////
    // parse command line arguments
    if (argc < 3)
    {
        cout << "Missing some arguments!";
        return -1;
    }
    else
    {
        sInputFilePath = argv[1];
        sOutputFilePath = sInputFilePath + ".txt";
    }

    pCRingBuffer = new CRingBuffer<float>(kBlockSize);
    
    CFft::createInstance(pCFft);
    
    pCFft->initInstance(kBlockSize);
    
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
    ppfAudioData = new float*[stFileSpec.iNumChannels];
    for (int i = 0; i < stFileSpec.iNumChannels; i++)
        ppfAudioData[i] = new float[kBlockSize];
    
    ppfOutAudioData = new float*[stFileSpec.iNumChannels];
    for (int i = 0; i < stFileSpec.iNumChannels; i++)
        ppfOutAudioData[i] = new float[kBlockSize];

    time = clock();
    //////////////////////////////////////////////////////////////////////////////
    // get audio data and write it to the output file
    long long iNumFramesBlockSize = (long long)kBlockSize;
    
    pCRingBuffer->putPostInc(*ppfAudioData, iNumFramesBlockSize);
    
    while (!phAudioFile->isEof())
    {
        long long iNumFrames = kHopSize;

        cout << "\r" << "reading and writing";

        for (int i = 0; i < iNumFrames; i++)
        {
            for (int c = 0; c < stFileSpec.iNumChannels; c++)
            {
                pCRingBuffer->get(*ppfOutAudioData, iNumFramesBlockSize);
                pCRingBuffer->setReadIdx(iNumFrames);
                
                hOutputFile << ppfOutAudioData[c][i] << "\t";
            }
            hOutputFile << endl;
        }
        pCRingBuffer->putPostInc(*ppfAudioData, iNumFrames);
        phAudioFile->readData(ppfAudioData, iNumFrames);
    }

    cout << "\nreading/writing done in: \t" << (clock() - time)*1.F / CLOCKS_PER_SEC << " seconds." << endl;

    //////////////////////////////////////////////////////////////////////////////
    // clean-up
    CAudioFileIf::destroy(phAudioFile);
    hOutputFile.close();

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

