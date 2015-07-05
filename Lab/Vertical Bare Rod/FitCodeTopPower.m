load('June10VertBareRodHeating-TopPower');

lb = [0 0 0.25 0 0 100];
ub = [Inf Inf 0.35 Inf 1 10000];
x = [200 2000 0.1 10 0.3 1000]; %initial guess
offsets2 = [0 4.6599 -1.4521 8.2331 12.8798 1.9197];
%parameters: 1st is conduction constant, 2nd is convection inside tube,
%3rd is emissivity inside tube, 4th is convection outside tube, 5th is
%emissivity outside tube

tOffset = -1;
reading1 = 1;
readingF = 2467;
amb1 = 1;
Pin = 5.35;
eq = 1;
iceEnd = 0;
blackRod = 0;
%make sure readings are loaded here
[x, errsum] = lsqnonlin(@(x)transientFinDiffFuncNonLin(x, readings, tOffset, reading1, readingF, offsets2, amb1, Pin, eq, iceEnd, blackRod), x, lb, ub);
x
errsum