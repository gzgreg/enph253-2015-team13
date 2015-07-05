lb = [0 0 0 0 0 100];
ub = [Inf Inf 1 Inf 1 10000];
x = [212 47 0 155.7 1 900]; %initial guess
offsets2 = [1.0373 3.4564 1.1484 3.4479 -0.3448 0];

tOffset = -1;
reading1 = 1;
readingF = 2676;
amb1 = 0;
Pin = 5.35;
eq = 1;
iceEnd = 0;
blackRod = 0;
%make sure readings are loaded here
[x, errsum] = lsqnonlin(@(x)transientFinDiffFuncNonLin(x, readings, tOffset, reading1, readingF, offsets2, amb1, Pin, eq, iceEnd, blackRod), x, lb, ub);
x
errsum