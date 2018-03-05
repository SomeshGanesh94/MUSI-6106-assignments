
#include <iostream>
#include <ctime>
#include <string.h>

#include "MUSI6106Config.h"

#include "AudioFileIf.h"
#include "Vibrato.h"
#include "RingBuffer.h"

using namespace::std;

// local function declarations
void    showClInfo();

/////////////////////////////////////////////////////////////////////////////////
// main function
int main(int argc, char* argv[])
{
    std::string                 sInputFilePath,                 //!< file paths
    sOutputFilePath;
    
    static const int            kBlockSize = 1024;
    long long                   iNumFrames = kBlockSize;
    
    float                       fSampleRate;
    float                       fModFrequencyInHz;
    float                       fBasicDelayInSec;
    float                       fModWidthInSec;
    
    clock_t                     time = 0;
    
    float                       **ppfAudioData = 0;
    float                       **ppfAudioOutputData = 0;
    
    CAudioFileIf                *phAudioFile = 0;
    CAudioFileIf                *phAudioOutputFile = 0;
    
    CAudioFileIf::FileSpec_t    stFileSpec;
    
    CVibrato                    *pCVibrato = 0;
    
    showClInfo();
    
    //////////////////////////////////////////////////////////////////////////////
    // parse command line arguments
    // arguments should be in this sequence: (input_file_path output_file_path sample_rate mod_frequency mod_delay mod_width)
    
    sInputFilePath = argv[1];
    sOutputFilePath = argv[2];
    fSampleRate = atof(argv[3]);
    fModFrequencyInHz = atof(argv[4]);
    fBasicDelayInSec = atof(argv[5]);
    fModWidthInSec = atof(argv[6]);
    
    if(argc < 7)
    {
        cout << "Incorrect number of arguments!" << endl;
        return -1;
    }
    
    //////////////////////////////////////////////////////////////////////////////
    // open the input wave file
    CAudioFileIf::create(phAudioFile);
    CAudioFileIf::create(phAudioOutputFile);
    
    phAudioFile->openFile(sInputFilePath, CAudioFileIf::kFileRead);
    phAudioFile->getFileSpec(stFileSpec);
    phAudioOutputFile->openFile(sOutputFilePath, CAudioFileIf::kFileWrite, &stFileSpec);
    
    if (!phAudioFile->isOpen())
    {
        cout << "Input file open error!";
        
        CAudioFileIf::destroy(phAudioFile);
        CAudioFileIf::destroy(phAudioOutputFile);
        return -1;
    }
    else if (!phAudioOutputFile->isOpen())
    {
        cout << "Output file cannot be initialized!";
        
        CAudioFileIf::destroy(phAudioFile);
        CAudioFileIf::destroy(phAudioOutputFile);
        return -1;
    }
    
    //////////////////////////////////////////////////////////////////////////////
    // init vibrato
    CVibrato::create(pCVibrato);
    pCVibrato->init(fSampleRate, fModFrequencyInHz, fBasicDelayInSec, stFileSpec.iNumChannels);
    
    // Set parameters of vibrato
    pCVibrato->setParam(CVibrato::kParamModFreq, fModFrequencyInHz);
    pCVibrato->setParam(CVibrato::kParamWidth, fModWidthInSec);
    
    //////////////////////////////////////////////////////////////////////////////
    // allocate memory
    ppfAudioData = new float*[stFileSpec.iNumChannels];
    for (int i = 0; i < stFileSpec.iNumChannels; i++)
        ppfAudioData[i] = new float[kBlockSize];
    
    ppfAudioOutputData = new float*[stFileSpec.iNumChannels];
    for (int i = 0; i < stFileSpec.iNumChannels; i++)
        ppfAudioOutputData[i] = new float[kBlockSize];
    
    time = clock();
    
    //////////////////////////////////////////////////////////////////////////////
    // get audio data and write it to the output file
    
    while (!phAudioFile->isEof())
    {
        phAudioFile->readData(ppfAudioData, iNumFrames);
        pCVibrato->process(ppfAudioData, ppfAudioOutputData, iNumFrames);
        phAudioOutputFile->writeData(ppfAudioOutputData, iNumFrames);
    }
    
    cout << "\nreading/writing done in: \t" << (clock() - time)*1.F / CLOCKS_PER_SEC << " seconds." << endl;
    
    //////////////////////////////////////////////////////////////////////////////
    // clean-up
    CAudioFileIf::destroy(phAudioFile);
    CAudioFileIf::destroy(phAudioOutputFile);
    CVibrato::destroy(pCVibrato);
    
    for (int i = 0; i < stFileSpec.iNumChannels; i++)
    {
        delete[] ppfAudioData[i];
        delete[] ppfAudioOutputData[i];
    }
    delete[] ppfAudioData;
    delete[] ppfAudioOutputData;
    ppfAudioData = 0;
    ppfAudioOutputData = 0;
    
    return 0;
    
}

void showClInfo()
{
    cout << "GTCMT MUSI6106 Executable" << endl;
    cout << "(c) 2014-2018 by Alexander Lerch" << endl;
    cout  << endl;
    
    return;
}
