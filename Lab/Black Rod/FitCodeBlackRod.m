lb = [0 0 0.9999 0 0.9999];
ub = [230 Inf 1 Inf 1];
x = [220 30 1 10 1]; %initial guess
%parameters: 1st is conduction constant, 2nd is convection inside tube,
%3rd is emissivity inside tube, 4th is convection outside tube, 5th is
%emissivity outside tube

tOffset = -1;
reading1 = 1;
readingF = 3603;
%make sure readings are loaded here
lsqnonlin(@(x)transientFinDiffFuncNonLin(x, readings, tOffset, reading1, readingF, 0, 9.9, 1, 0), x, lb, ub)