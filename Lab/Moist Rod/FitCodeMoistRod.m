load('June12MoistRodHeating.mat');

readings(1, 3, :) = readings(1, 3, :)*2;

lb = [100 0 0.9999 0 0.9999 100 0];
ub = [270 Inf 1 Inf 1 10000 5.35];
x = [200 16 1 10 1 1600 5.35]; %initial guess
offsets2 = [0 1.2233 -8.7995 1.5927 8.7267 -0.8943];
%parameters: 1st is conduction constant, 2nd is convection inside tube,
%3rd is emissivity inside tube, 4th is convection outside tube, 5th is
%emissivity outside tube

tOffset = 8.6;
reading1 = 35;
readingF = 3838;
%make sure readings are loaded here
amb1 = 1;
Pin = 5.35;
eq = 1;
iceEnd = 0;
blackRod = 0;
moistRod = 1;
%make sure readings are loaded here
[x, errsum] = lsqnonlin(@(x)transientFinDiffFuncNonLin(x, readings, tOffset, reading1, readingF, offsets2, amb1, Pin, eq, iceEnd, blackRod, moistRod), x, lb, ub);
x
errsum