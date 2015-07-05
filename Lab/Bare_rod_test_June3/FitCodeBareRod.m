lb = [0 0 0 0 0];
ub = [Inf Inf 1 Inf 1];
x = [212 47 0 155.7 1]; %initial guess
offsets2 = [1.0373 3.4564 1.1484 3.4479 -0.3448 0];
%parameters: 1st is conduction constant, 2nd is convection inside tube,
%3rd is emissivity inside tube, 4th is convection outside tube, 5th is
%emissivity outside tube

tOffset = -1;
reading1 = 1;
readingF = 2676;
amb1 = 0;
Pin = 5.35;
eq = 1;
iceEnd = 0;
%make sure readings are loaded here
[x, chi] = lsqnonlin(@(x)transientFinDiffFuncNonLin(x, readings, tOffset, reading1, readingF, offsets2, amb1, Pin, eq, iceEnd), x, lb, ub);
x 
chi