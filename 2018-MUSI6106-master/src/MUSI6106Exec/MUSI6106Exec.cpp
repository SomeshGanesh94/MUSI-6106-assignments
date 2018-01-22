
#include <iostream>
#include <ctime>
#include <fstream>

#include "MUSI6106Config.h"

#include "AudioFileIf.h"

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

    long long               iInFileLength       = 0;        //!< length of input file

    clock_t                 time                = 0;

    float                   **ppfAudioData      = 0;

    CAudioFileIf            *phAudioFile        = 0;

    showClInfo ();

    //////////////////////////////////////////////////////////////////////////////
    // parse command line arguments
    
    // argv[1] will contain the complete path to the input audio file
    sInputFilePath = argv[1];
    // argv[2] will contain the complete path to the output text file
    sOutputFilePath = argv[2];
    
    //////////////////////////////////////////////////////////////////////////////
    // open the input wave file
    CAudioFileIf::create(phAudioFile);
    phAudioFile->openFile(sInputFilePath, CAudioFileIf::kFileRead);

    //////////////////////////////////////////////////////////////////////////////
    // allocate memory
    time                    = clock();
    
    CAudioFileIf::FileSpec_t file_spec;
    phAudioFile->getFileSpec(file_spec);
    
    long long block_size = 1024;
    ppfAudioData = new float* [file_spec.iNumChannels];
    for (int i = 0; i < file_spec.iNumChannels; i++)
    {
        ppfAudioData[i] = new float [block_size];
    }

    // get audio data and write it to the output file
    phAudioFile->getLength(iInFileLength);
    
    // int num_frames = iInFileLength / block_size;
    
    std::ofstream outfile;
    outfile.open((sOutputFilePath));
    
    int frame_no = 0;
    while(!phAudioFile->isEof())
    {
        phAudioFile->readData(ppfAudioData, block_size);
        
        for (int j = 0; j < int(block_size); j++)
        {
            outfile << ppfAudioData[0][j] << "\t" << ppfAudioData[1][j] << endl;
        }
        frame_no++;
    }
    

    cout << "reading/writing done in: \t"    << (clock()-time)*1.F/CLOCKS_PER_SEC << " seconds." << endl;
    //////////////////////////////////////////////////////////////////////////////
    // get audio info and print it to stdout

    //////////////////////////////////////////////////////////////////////////////
    // do processing
    cout << "Hello there!" << endl << endl;

    //////////////////////////////////////////////////////////////////////////////
    // clean-up
    delete ppfAudioData;
    outfile.close();
    

    return 0;
    
}


void     showClInfo()
{
    cout << "GTCMT MUSI6106 Executable" << endl;
    cout << "(c) 2014-2018 by Alexander Lerch" << endl;
    cout  << endl;

    return;
}

