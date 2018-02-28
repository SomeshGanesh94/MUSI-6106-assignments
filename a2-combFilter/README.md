# MUSI6106 Assignment2: Comb Filter

The executable requires five parameters in order to function properly: input_file_path, filter_type, delay_time, gain and output_file_path. If no argument is passed into the main function. It will run the tests within.

### Comparing with MATLAB implementation results

Two different audio input files are included in ./etc. name_output.wav is the result generated from C++ implementation and name_matlab_output.wav is the result generated from MATLAB using test.m.

The sum of the absolute difference between two results are different for these two files. One is 230.8751 for ahh.wav and 1.9628 for sweep.wav.

### Test cases for the Comb Filter

Test case 1: Passing an impulse into an IIR filter without setting the delay duration.
Also setting gain out of bounds.

```
Input    signal:1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 

Output   signal:1 0 0 0 0 0 0 0 0 0 0.5 0 0 0 0 0 0 0 0 0 0.25 0 0 0 0 0 0 0 0 0 0.125 0 0 0 0 0 0 0 0 0 0.0625 0 0 0 0 0 0 0 0 0 0.03125 0 0 0 0 0 0 0 0 0 0.015625 0 0 0 0 0 0 0 0 0 0.0078125 0 0 0 0 0 0 0 0 0 0.00390625 0 0 0 0 0 0 0 0 0 0.00195312 0 0 0 0 0 0 0 0 0 

Expected signal:1 0 0 0 0 0 0 0 0 0 0.5 0 0 0 0 0 0 0 0 0 0.25 0 0 0 0 0 0 0 0 0 0.125 0 0 0 0 0 0 0 0 0 0.0625 0 0 0 0 0 0 0 0 0 0.03125 0 0 0 0 0 0 0 0 0 0.015625 0 0 0 0 0 0 0 0 0 0.0078125 0 0 0 0 0 0 0 0 0 0.00390625 0 0 0 0 0 0 0 0 0 0.00195312 0 0 0 0 0 0 0 0 0 
```

Test case 2: Passing a unit train into an FIR filter. Setting gain to 2.0f and delay in frames to 10.0f.

```
Input    signal:1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 

Output   signal:1 1 1 1 1 1 1 1 1 1 3 3 3 3 3 3 3 3 3 3 

Expected signal:1 1 1 1 1 1 1 1 1 1 3 3 3 3 3 3 3 3 3 3 
```

Test case 3: Passing a ramp signal with a slope of 1 into an IIR filter. Setting gain to -0.5f and delay in frames to 1.0f.

```
Input    signal:0 1 2 3 4 5 6 7 8 9 

Output   signal:0 1 1.5 2.25 2.875 3.5625 4.21875 4.89062 5.55469 6.22266 

Expected signal:0 1 1.5 2.25 2.875 3.5625 4.21875 4.89062 5.55469 6.22266 
```

The details of the tests are also documented in the TestLog.txt under the executable file's directory.