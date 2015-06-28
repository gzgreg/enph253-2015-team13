lb = [0 0 0 0 0];
ub = [Inf Inf 1 Inf 1];
x = [200 0 0.1 10 0.3]; %initial guess
%parameters: 1st is conduction constant, 2nd is convection inside tube,
%3rd is emissivity inside tube, 4th is convection outside tube, 5th is
%emissivity outside tube

tOffset = 59.1847;
reading1 = 220;
readingF = 4130;
%make sure readings are loaded here
lsqnonlin(@(x)transientFinDiffFuncNonLin(x, readings, tOffset, reading1, readingF), x, lb, ub)