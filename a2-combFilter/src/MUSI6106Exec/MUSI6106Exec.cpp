
#include <iostream>
#include <ctime>
#include <string.h>

#include "MUSI6106Config.h"

#include "AudioFileIf.h"
#include "CombFilterIf.h"

using std::cout;
using std::endl;

// local function declarations
void    showClInfo();
int     testcases(CCombFilterIf *&pInstance);

/////////////////////////////////////////////////////////////////////////////////
// main function
int main(int argc, char* argv[])
{
    std::string                 sInputFilePath,                 //!< file paths
                                sOutputFilePath;

    static const int            kBlockSize = 1024;
    long long                   iNumFrames = kBlockSize;
    static const float          fMaxFilterDelayTime = 1.0f;

    clock_t                     time = 0;
    
    int                         iFilterType;
    float                       fFilterDelayTime;
    float                       fFilterGain;

    float                       **ppfAudioData = 0;
    float                       **ppfAudioOutputData = 0;

    CAudioFileIf                *phAudioFile = 0;
    CAudioFileIf                *phAudioOutputFile = 0;
    
    CAudioFileIf::FileSpec_t    stFileSpec;

    CCombFilterIf               *pInstance = 0;
    
    showClInfo();

    //////////////////////////////////////////////////////////////////////////////
    // parse command line arguments
    // arguments should be in this sequence: (input_file_path filter_type delay_time gain output_file_path)
    if (argc == 1)
    {
        cout << "No arguments given. Running tests on the comb filter design. Also check TestLog.txt for detailed information." << endl << endl;
        return testcases(pInstance);
    }
    if (argc != 6)
    {
        cout << "Missing arguments!" << endl;
        return -1;
    }
    if (strcmp(argv[2], "FIR")!=0 && strcmp(argv[2], "IIR")!=0)
    {
        cout << "Unexpected filter type!" << endl;
        return -1;
    }
    
    sInputFilePath = argv[1];
    iFilterType = strcmp(argv[2], "FIR") == 0 ? 0 : 1;
    fFilterDelayTime = atof(argv[3]);
    fFilterGain = atof(argv[4]);
    sOutputFilePath = argv[5];
    
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
        return -1;
    }
    else if (!phAudioOutputFile->isOpen())
    {
        cout << "Output file cannot be initialized!";
        return -1;
    }
    
    //////////////////////////////////////////////////////////////////////////////
    // Init Comb Filter
    CCombFilterIf::create(pInstance);
    pInstance->init((iFilterType ? CCombFilterIf::CombFilterType_t::kCombIIR : CCombFilterIf::CombFilterType_t::kCombFIR), fMaxFilterDelayTime, stFileSpec.fSampleRateInHz, stFileSpec.iNumChannels);
    
    // set parameters of the comb filter
    pInstance->setParam(CCombFilterIf::FilterParam_t::kParamDelay, fFilterDelayTime);
    pInstance->setParam(CCombFilterIf::FilterParam_t::kParamGain, fFilterGain);

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
        pInstance->process(ppfAudioData, ppfAudioOutputData, iNumFrames);
        phAudioOutputFile->writeData(ppfAudioOutputData, iNumFrames);
    }

    cout << "\nreading/writing done in: \t" << (clock() - time)*1.F / CLOCKS_PER_SEC << " seconds." << endl;

    //////////////////////////////////////////////////////////////////////////////
    // clean-up
    CAudioFileIf::destroy(phAudioFile);
    CAudioFileIf::destroy(phAudioOutputFile);
    CCombFilterIf::destroy(pInstance);
    
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

int testcases(CCombFilterIf *&pInstance)
{
    std::fstream fileStream;
    fileStream.open("./TestLog.txt", std::fstream::out);
    
    // Test case 1
    // Passing an impulse into an IIR filter without setting the delay duration.
    // Also setting gain out of bounds.
    {
        cout << "*********************************************************************************************" << endl;
        cout << "Test case 1: Passing an impulse into an IIR filter without setting the delay duration." << endl;
        cout << "Also setting gain out of bounds." << endl << endl;
        fileStream << "*********************************************************************************************" << endl;
        fileStream << "Test case 1: Passing an impulse into an IIR filter without setting the delay duration." << endl;
        fileStream << "Also setting gain out of bounds." << endl << endl;
        const int iSignalLength = 100;
        float fSignalLength = float(iSignalLength);
        float **ppfInputSignal = 0;
        float **ppfOutputSignal = 0;
        float fExpectedSignal[iSignalLength] = {0};
        
        ppfInputSignal = new float*[1];
        ppfOutputSignal = new float*[1];
        for (int i = 0; i < 1; i++)
        {
            ppfInputSignal[i] = new float[iSignalLength];
            memset (ppfInputSignal[i], 0, sizeof(float) * fSignalLength);
            ppfOutputSignal[i] = new float[iSignalLength];
            memset (ppfOutputSignal[i], 0, sizeof(float) * fSignalLength);
        }
        ppfInputSignal[0][0] = 1.0f;
        fExpectedSignal[0] = 1.0f;
        for (int i = 10; i < iSignalLength; i = i + 10)
            fExpectedSignal[i] = fExpectedSignal[i - 10] / 2.0f;
        
        CCombFilterIf::destroy(pInstance);
        CCombFilterIf::create(pInstance);
        pInstance->init(CCombFilterIf::CombFilterType_t::kCombIIR, 0.01f, 1000, 1);
        pInstance->setParam(CCombFilterIf::FilterParam_t::kParamGain, 2.0f);
        pInstance->process(ppfInputSignal, ppfOutputSignal, iSignalLength);
        
        cout << endl <<"Input    signal: ";
        fileStream << endl << "Input    signal:";
        for (int i = 0; i < iSignalLength; i++)
        {
            cout << ppfInputSignal[0][i] << " ";
            fileStream << ppfInputSignal[0][i] << " ";
        }
        cout << endl << "Output   signal: ";
        fileStream << endl << "Output   signal:";
        for (int i = 0; i < iSignalLength; i++)
        {
            cout << ppfOutputSignal[0][i] << " ";
            fileStream << ppfOutputSignal[0][i] << " ";
        }
        cout << endl << "Expected signal: ";
        fileStream << endl << "Expected signal:";
        for (int i = 0; i < iSignalLength; i++)
        {
            cout << fExpectedSignal[i] << " ";
            fileStream << fExpectedSignal[i] << " ";
        }
        cout << endl;
        fileStream << endl;
        
        for (int i = 0; i < 1; i++)
        {
            delete[] ppfInputSignal[i];
            delete[] ppfOutputSignal[i];
        }
        delete[] ppfInputSignal;
        delete[] ppfOutputSignal;
        ppfInputSignal = 0;
        ppfOutputSignal = 0;
    }
    
    // Test case 2
    // Passing an impulse train into a FIR filter. Setting gain to 2.0f and delay in frames to 1.0f.
    {
        cout << "*********************************************************************************************" << endl;
        cout << "Test case 2: Passing a unit train into an FIR filter. Setting gain to 2.0f and delay in frames to 10.0f." << endl << endl;
        fileStream << "*********************************************************************************************" << endl;
        fileStream << "Test case 2: Passing a unit train into an FIR filter. Setting gain to 2.0f and delay in frames to 10.0f." << endl << endl;
        const int iSignalLength = 20;
        float fSignalLength = float(iSignalLength);
        float **ppfInputSignal = 0;
        float **ppfOutputSignal = 0;
        float fExpectedSignal[iSignalLength] = {0};
        
        ppfInputSignal = new float*[1];
        ppfOutputSignal = new float*[1];
        for (int i = 0; i < 1; i++)
        {
            ppfInputSignal[i] = new float[iSignalLength];
            memset (ppfInputSignal[i], 0, sizeof(float) * fSignalLength);
            ppfOutputSignal[i] = new float[iSignalLength];
            memset (ppfOutputSignal[i], 0, sizeof(float) * fSignalLength);
            for (int j = 0; j < iSignalLength; j++)
                ppfInputSignal[i][j] = 1.0f;
        }
        for (int j = 0; j < iSignalLength; j++)
            fExpectedSignal[j] = (j < 10 ? 1.0f : 1.0f + fExpectedSignal[j - 10] * 2.0f);
        
        CCombFilterIf::destroy(pInstance);
        CCombFilterIf::create(pInstance);
        pInstance->init(CCombFilterIf::CombFilterType_t::kCombFIR, 1.0f, 1000, 1);
        pInstance->setParam(CCombFilterIf::FilterParam_t::kParamGain, 2.0f);
        pInstance->setParam(CCombFilterIf::FilterParam_t::kParamDelay, 0.01f);
        pInstance->process(ppfInputSignal, ppfOutputSignal, iSignalLength);
        
        cout <<"Input    signal: ";
        fileStream << "Input    signal:";
        for (int i = 0; i < iSignalLength; i++)
        {
            cout << ppfInputSignal[0][i] << " ";
            fileStream << ppfInputSignal[0][i] << " ";
        }
        cout << endl << "Output   signal: ";
        fileStream << endl << "Output   signal:";
        for (int i = 0; i < iSignalLength; i++)
        {
            cout << ppfOutputSignal[0][i] << " ";
            fileStream << ppfOutputSignal[0][i] << " ";
        }
        cout << endl << "Expected signal: ";
        fileStream << endl << "Expected signal:";
        for (int i = 0; i < iSignalLength; i++)
        {
            cout << fExpectedSignal[i] << " ";
            fileStream << fExpectedSignal[i] << " ";
        }
        cout << endl;
        fileStream << endl;
        
        for (int i = 0; i < 1; i++)
        {
            delete[] ppfInputSignal[i];
            delete[] ppfOutputSignal[i];
        }
        delete[] ppfInputSignal;
        delete[] ppfOutputSignal;
        ppfInputSignal = 0;
        ppfOutputSignal = 0;
    }
    
    // Test case 3
    // Passing a ramp signal with a slope of 1 into an IIR filter. Setting gain to -0.5f and delay in frames to 1.0f.
    {
        cout << "*********************************************************************************************" << endl;
        cout << "Test case 3: Passing a ramp signal with a slope of 1 into an IIR filter. Setting gain to -0.5f and delay in frames to 1.0f." << endl << endl;
        fileStream << "*********************************************************************************************" << endl;
        fileStream << "Test case 3: Passing a ramp signal with a slope of 1 into an IIR filter. Setting gain to -0.5f and delay in frames to 1.0f." << endl << endl;
        const int iSignalLength = 10;
        float fSignalLength = float(iSignalLength);
        float **ppfInputSignal = 0;
        float **ppfOutputSignal = 0;
        float fExpectedSignal[iSignalLength] = {0, 1, 1.5, 2.25, 2.875, 3.5625, 4.21875, 4.89062, 5.55469, 6.22266};
        
        ppfInputSignal = new float*[1];
        ppfOutputSignal = new float*[1];
        for (int i = 0; i < 1; i++)
        {
            ppfInputSignal[i] = new float[iSignalLength];
            memset (ppfInputSignal[i], 0, sizeof(float) * fSignalLength);
            ppfOutputSignal[i] = new float[iSignalLength];
            memset (ppfOutputSignal[i], 0, sizeof(float) * fSignalLength);
            for (int j = 0; j < iSignalLength; j++)
                ppfInputSignal[i][j] = float(j);
        }
        
        CCombFilterIf::destroy(pInstance);
        CCombFilterIf::create(pInstance);
        pInstance->init(CCombFilterIf::CombFilterType_t::kCombIIR, 1.0f, 1000, 1);
        pInstance->setParam(CCombFilterIf::FilterParam_t::kParamGain, -0.5f);
        pInstance->setParam(CCombFilterIf::FilterParam_t::kParamDelay, 0.001f);
        pInstance->process(ppfInputSignal, ppfOutputSignal, iSignalLength);
        
        cout <<"Input    signal: ";
        fileStream << "Input    signal:";
        for (int i = 0; i < iSignalLength; i++)
        {
            cout << ppfInputSignal[0][i] << " ";
            fileStream << ppfInputSignal[0][i] << " ";
        }
        cout << endl << "Output   signal: ";
        fileStream << endl << "Output   signal:";
        for (int i = 0; i < iSignalLength; i++)
        {
            cout << ppfOutputSignal[0][i] << " ";
            fileStream << ppfOutputSignal[0][i] << " ";
        }
        cout << endl << "Expected signal: ";
        fileStream << endl << "Expected signal:";
        for (int i = 0; i < iSignalLength; i++)
        {
            cout << fExpectedSignal[i] << " ";
            fileStream << fExpectedSignal[i] << " ";
        }
        cout << endl;
        fileStream << endl;
        
        for (int i = 0; i < 1; i++)
        {
            delete[] ppfInputSignal[i];
            delete[] ppfOutputSignal[i];
        }
        delete[] ppfInputSignal;
        delete[] ppfOutputSignal;
        ppfInputSignal = 0;
        ppfOutputSignal = 0;
    }
    
    cout << "*********************************************************************************************" << endl;
    cout << "Test Ended" << endl << endl;
    fileStream << "*********************************************************************************************" << endl;
    fileStream << "Test Ended" << endl << endl;
    
    CCombFilterIf::destroy(pInstance);
    fileStream.close();
    return 0;
}




