%2015.05.25
%scipt for interperting some data 
%you must load tmp tmpAmb and thermoCouple before running

coeff = ones(1,40)/40;
avgTmpAmb = tmpAmb;%(2000:4711);
avgTmpAmb = filter(coeff,1,avgTmpAmb);
ambientTemp = avgTmpAmb*.5682-3.822;%use calculated temperature conversion formula

avgTmp = filter(coeff,1,tmp);%(2000:4711));
probeTemp = avgTmp*.5682-3.822;

avgTC = filter(coeff,1,thermocouple);%(2000:4711));

dT = probeTemp - ambientTemp;

plot(avgTC,dT)