%% Somesh Ganesh
% MUSI 6106 
% In-class exercise 1: audio file IO

clc;
close all;
clear all;
%% Comparing wave file (input) and text file (output)

input_wav_file = audioread('/Users/someshganesh/Documents/GitHub/MUSI-6106-assignments/audio/sweep.wav');
output_text_file = importdata('/Users/someshganesh/Documents/GitHub/MUSI-6106-assignments/audio/sweep.txt');

total_difference = sum(abs(input_wav_file - output_text_file));