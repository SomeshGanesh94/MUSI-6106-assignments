
#include <iostream>
#include <ctime>

#include "MUSI6106Config.h"

#include "AudioFileIf.h"
#include "RingBuffer.h"

using std::cout;
using std::endl;

// local function declarations
void    showClInfo ();

/////////////////////////////////////////////////////////////////////////////////
// main function
int main(int argc, char* argv[])
{
    // You don't need to use an audio file this time, though you can try if you want.
    // std::string             sInputFilePath,                 //!< file paths
    //                         sOutputFilePath;
    // static const int        kBlockSize          = 1024;
    // float                   **ppfAudioData      = 0;
    // CAudioFileIf            *phAudioFile        = 0;
    // CAudioFileIf::FileSpec_t stFileSpec;
    // long long               iInFileLength       = 0;        //!< length of input file

    clock_t                 time                = 0;

    CRingBuffer<float>      *pCRingBuffer       = 0;
    float                   *pfTestSignal       = 0;
    float                   fGain               = 1.f;
    int                     buffer_length       = atoi(argv[1]);
    int                     test_signal_length  = atoi(argv[2]);
    int                     test_flag           = 0;
    int                     test_0(CRingBuffer<float> *pCRingBuffer, float *pfTestSignal, int buffer_length, int test_signal_length);
    int                     test_1(CRingBuffer<float> *pCRingBuffer, float *pfTestSignal, int buffer_length, int test_signal_length);
    int                     test_2(CRingBuffer<float> *pCRingBuffer, float *pfTestSignal, int buffer_length, int test_signal_length);
    int                     test_3(CRingBuffer<float> *pCRingBuffer, float *pfTestSignal, int buffer_length, int test_signal_length);
    int                     test_4(CRingBuffer<float> *pCRingBuffer, float *pfTestSignal, int buffer_length, int test_signal_length);
    int                     test_5(CRingBuffer<float> *pCRingBuffer, float *pfTestSignal, int buffer_length, int test_signal_length);
    
    showClInfo ();

    //////////////////////////////////////////////////////////////////////////////
    // allocate memory
    time                    = clock();
    pCRingBuffer = new CRingBuffer<float>(buffer_length);
    pfTestSignal = new float[test_signal_length];

    // fill the test signal (e.g., a unit impulse)
    // Test signal: Unit step
    for (int i=0; i < test_signal_length; i++)
    {
        pfTestSignal[i] = i;
    }
    cout << "The test signal is a ramp signal of length " << test_signal_length << endl;
    

    //////////////////////////////////////////////////////////////////////////////
    // do processing and tests
    
    // Test 0: Checks reset() & if read pointer tries to access data not written yet
    if(test_0(pCRingBuffer, pfTestSignal, buffer_length, test_signal_length) != 0)
    {
        cout << "Test 0 fails " << endl;
        test_flag = 1;
    }
    else
    {
        cout << "Test 0 passes " << endl;
    }
    
    // Test 1: Checks if value is correctly written and read
    if(test_1(pCRingBuffer, pfTestSignal, buffer_length, test_signal_length) != 0)
    {
        cout << "Test 1 fails " << endl;
        test_flag = 1;
    }
    else
    {
        cout << "Test 1 passes " << endl;
    }
    
    // Test 2: Checks if write pointer is correctly set/read
    if(test_2(pCRingBuffer, pfTestSignal, buffer_length, test_signal_length) != 0)
    {
        cout << "Test 2 fails " << endl;
        test_flag = 1;
    }
    else
    {
        cout << "Test 2 passes " << endl;
    }
    
    // Test 3: Checks if read pointer is correctly set/read
    if(test_3(pCRingBuffer, pfTestSignal, buffer_length, test_signal_length) != 0)
    {
        cout << "Test 3 fails " << endl;
        test_flag = 1;
    }
    else
    {
        cout << "Test 3 passes " << endl;
    }
    
    // Test 4: Checks if read and write pointers increment correctly
    if(test_4(pCRingBuffer, pfTestSignal, buffer_length, test_signal_length) != 0)
    {
        cout << "Test 4 fails " << endl;
        test_flag = 1;
    }
    else
    {
        cout << "Test 4 passes " << endl;
    }
    
    // Test 5: Offset value check for "T get (int iOffset = 0) const()"
    if(test_5(pCRingBuffer, pfTestSignal, buffer_length, test_signal_length) != 0)
    {
        cout << "Test 5 fails " << endl;
        test_flag = 1;
    }
    else
    {
        cout << "Test 5 passes " << endl;
    }
    
    
    if(test_flag == 0)
    {
        cout << "All functions for the ring buffer have been implemented correctly! " << "\n" <<endl;
    }

    cout << "processing done in: \t"    << (clock()-time)*1.F/CLOCKS_PER_SEC << " seconds." << endl;

    //////////////////////////////////////////////////////////////////////////////
    // clean-up
    delete pCRingBuffer;
    delete [] pfTestSignal;

    return 0;
}

int test_0(CRingBuffer<float> *pCRingBuffer, float *pfTestSignal, int buffer_length, int test_signal_length)
{
    pCRingBuffer->getPostInc();
    pCRingBuffer->reset();
    return 0;
}

int test_1(CRingBuffer<float> *pCRingBuffer, float *pfTestSignal, int buffer_length, int test_signal_length)
{
    float write_val = 257;
    pCRingBuffer->put(write_val);
    float read_val = pCRingBuffer->get();
    if(write_val != read_val)
    {
        cout << "Error in void put(T tValue) OR T get (int iOffset = 0) const " << endl;
        cout << "Value is either not correctly written or is not correctly read from the buffer " << endl;
        return -1;
    }
    return 0;
    
}

int test_2(CRingBuffer<float> *pCRingBuffer, float *pfTestSignal, int buffer_length, int test_signal_length)
{
    int write_idx_in[4] = {-25867, -367, 5, 4586};
    int write_idx_out[4] = {245, 145, 5, 490};
    int test_val = 0;
    
    for(int i = 0; i < 4; i++)
    {
        pCRingBuffer->setWriteIdx(write_idx_in[i]);
        test_val = pCRingBuffer->getWriteIdx();
        
        if(test_val != write_idx_out[i])
        {
            cout << "Error in void setWriteIdx (int iNewWriteIdx) OR int getWriteIdx () const " << endl;
            cout << "Write index not being assigned OR read properly " << endl;
            return -1;
        }
    }
    return 0;
}

int test_3(CRingBuffer<float> *pCRingBuffer, float *pfTestSignal, int buffer_length, int test_signal_length)
{
    int read_idx_in[4] = {-25867, -367, 5, 4586};
    int read_idx_out[4] = {245, 145, 5, 490};
    int test_val = 0;
    
    for(int i = 0; i < 4; i++)
    {
        pCRingBuffer->setReadIdx(read_idx_in[i]);
        test_val = pCRingBuffer->getReadIdx();
        
        if(test_val != read_idx_out[i])
        {
            cout << "Error in void setReadIdx (int iNewReadIdx) OR int getReadIdx () const " << endl;
            cout << "Read index not being assigned OR read properly " << endl;
            return -1;
        }
    }
    return 0;
}

int test_4(CRingBuffer<float> *pCRingBuffer, float *pfTestSignal, int buffer_length, int test_signal_length)
{
    for(int i = 0; i < test_signal_length; i++)
    {
        pCRingBuffer->putPostInc(pfTestSignal[i]);
        float read_val = pCRingBuffer->getPostInc();
        if(read_val != pfTestSignal[i])
        {
            cout << "Error in void putPostInc (T tNewValue) OR T getPostInc () " << endl;
            cout << "Read/write indices are not incrementing correctly OR values being written and read are not the same " << endl;
            return -1;
        }
    }
    return 0;
}

int test_5(CRingBuffer<float> *pCRingBuffer, float *pfTestSignal, int buffer_length, int test_signal_length)
{
    int read_offset_in[4] = {-25867, -367, 5, 4586};
    int read_offset_out[4] = {245, 145, 5, 490};
    float test_val = 0;
    
    for(int i = 0;i < buffer_length; i++)
    {
        pCRingBuffer->putPostInc(pfTestSignal[i]);
    }
    
    for(int i = 0; i < 4; i++)
    {
        test_val = pCRingBuffer->get(read_offset_in[i]);
        
        if(test_val != pfTestSignal[read_offset_out[i]])
        {
            cout << "Error in T get (int iOffset = 0) const " << endl;
            cout << "Wrong value read for given offset " << endl;
            return -1;
        }
    }
    return 0;
}

void     showClInfo()
{
    cout << "GTCMT MUSI6106 Executable" << endl;
    cout << "(c) 2014-2018 by Alexander Lerch" << endl;
    cout  << endl;

    return;
}

