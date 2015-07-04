lb = [100 0 0 0 0.9999];
ub = [270 Inf 1 Inf 1];
x = [220 0 1 10 1]; %initial guess
offsets2 = [1.7317 2.4477 3.1061 3.9503 1.1222 0];
%parameters: 1st is conduction constant, 2nd is convection inside tube,
%3rd is emissivity inside tube, 4th is convection outside tube, 5th is
%emissivity outside tube

tOffset = 8.8;
reading1 = 35;
readingF = 3603;
%make sure readings are loaded here
amb1 = 1;
Pin = 5.35;
eq = 1;
iceEnd = 0;
%make sure readings are loaded here
[x, chi] = lsqnonlin(@(x)transientFinDiffFuncNonLin(x, readings, tOffset, reading1, readingF, offsets2, amb1, Pin, eq, iceEnd), x, lb, ub);
x
chi