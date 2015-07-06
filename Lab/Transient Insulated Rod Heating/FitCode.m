load('june1TransientInsulatedHeating.mat');
lb = [0 0 0 0 0 100];
ub = [Inf Inf 1 Inf 1 10000];
x = [190 0 0 375 1 988]; %initial guess
offsets2 = [0.3312 2.9439 0.75 3.3023 -1.0658 0];
%parameters: 1st is conduction constant, 2nd is convection inside tube,
%3rd is emissivity inside tube, 4th is convection outside tube, 5th is
%emissivity outside tube

tOffset = 59.1847;
reading1 = 220;
readingF = 4130;
amb1 = 0;
Pin = 9.9;
eq = 0;
iceEnd = 0;
blackRod = 0;
%make sure readings are loaded here
[x, errsum] = lsqnonlin(@(x)transientFinDiffFuncNonLin(x, readings, tOffset, reading1, readingF, offsets2, amb1, Pin, eq, iceEnd, blackRod), x, lb, ub);
x
errsum