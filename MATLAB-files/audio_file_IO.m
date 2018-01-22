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


difference = input_wav_file - output_text_file;


fclose(output_text_file_id);