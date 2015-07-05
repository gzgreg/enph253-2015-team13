lb = [150 0 0 0 0];
ub = [270 Inf 1 Inf 1];
x = [270 0 0.1 1000 0.3]; %initial guess
offsets2 = [0 1.9253 -2.2982 4.9571 5.6170 -2.9881];
%parameters: 1st is conduction constant, 2nd is convection inside tube,
%3rd is emissivity inside tube, 4th is convection outside tube, 5th is
%emissivity outside tube

tOffset = -1;
reading1 = 1;
readingF = 1777;
%make sure readings are loaded here
amb1 = 1;
Pin = 5.35;
eq = 0;
iceEnd = 1;
%make sure readings are loaded here
[x, chi] = lsqnonlin(@(x)transientFinDiffFuncNonLin(x, readings, tOffset, reading1, readingF, offsets2, amb1, Pin, eq, iceEnd), x, lb, ub);
x
chi