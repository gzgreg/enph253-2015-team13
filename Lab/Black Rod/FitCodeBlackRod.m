load('June17BlackRodHorizontal-Heating.mat');

readings = readings(:, [2 3 5 4 1 6], :); %mixed up sensors

lb = [100 0 0.9999 0 0.9999 100];
ub = [270 Inf 1 Inf 1 10000];
x = [220 0 1 10 1 10000]; %initial guess
offsets2 = [2.4477 3.1061 1.1222 3.9503 1.7317 0];
%parameters: 1st is conduction constant, 2nd is convection inside tube,
%3rd is emissivity inside tube, 4th is convection outside tube, 5th is
%emissivity outside tube

tOffset = 8.8;
reading1 = 35;
readingF = 3603;
%make sure readings are loaded here
amb1 = 0;
Pin = 5.35;
eq = 1;
iceEnd = 0;
blackRod = 1;
%make sure readings are loaded here
[x, errsum] = lsqnonlin(@(x)transientFinDiffFuncNonLin(x, readings, tOffset, reading1, readingF, offsets2, amb1, Pin, eq, iceEnd, blackRod), x, lb, ub);
x
errsum