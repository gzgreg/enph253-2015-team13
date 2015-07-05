load('June10VertBareRodHeating-BottomPowered');
lb = [0 0 0.25 0 0];
ub = [Inf Inf 0.35 Inf 1];
x = [200 100 0.1 0 0]; %initial guess
offsets2 = [0 0.7631 -4.6494 3.6108 8.2347 -1.1878];

%parameters: 1st is conduction constant, 2nd is convection inside tube,
%3rd is emissivity inside tube, 4th is convection outside tube, 5th is
%emissivity outside tube

tOffset = -1;
reading1 = 1;
readingF = 3843;
%make sure readings are loaded here
amb1 = 1;
Pin = 5.35;
eq = 1;
iceEnd = 0;
%make sure readings are loaded here
[x, chi] = lsqnonlin(@(x)transientFinDiffFuncNonLin(x, readings, tOffset, reading1, readingF, offsets2, amb1, Pin, eq, iceEnd), x, lb, ub);
x
chi