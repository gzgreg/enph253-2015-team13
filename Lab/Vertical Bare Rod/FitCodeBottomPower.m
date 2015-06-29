lb = [0 0 0 0 0];
ub = [250 Inf 1 Inf 1];
x = [0 100 0.1 0 0]; %initial guess
%parameters: 1st is conduction constant, 2nd is convection inside tube,
%3rd is emissivity inside tube, 4th is convection outside tube, 5th is
%emissivity outside tube

tOffset = -1;
reading1 = 1;
readingF = 1373;
%make sure readings are loaded here
lsqnonlin(@(x)transientFinDiffFuncNonLin(x, readings, tOffset, reading1, readingF, 1, 5.3, 1, 0), x, lb, ub)