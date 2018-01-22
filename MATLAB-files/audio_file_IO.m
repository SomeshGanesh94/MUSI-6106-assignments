%% Somesh Ganesh
% MUSI 6106 
% In-class exercise 1: audio file IO

clc;
close all;
clear all;
%% Comparing wave file (input) and text file (output)

input_wav_file = audioread('/Users/someshganesh/Documents/GitHub/MUSI-6106-assignments/audio/sweep.wav');

output_text_file_id = fopen('/Users/someshganesh/Documents/GitHub/MUSI-6106-assignments/audio/sweep.txt','r');
output_text_file = fscanf(output_text_file_id, '%f');
output = zeros(132300,2);

for i=1:2:3*44100
    
    output(((i-1)/2)+1,1) = output_text_file(i);
    output(((i-1)/2)+1,2) = output_text_file(i+1);
    
end


difference = abs(input_wav_file - output);


fclose(output_text_file_id);