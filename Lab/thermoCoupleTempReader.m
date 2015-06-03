smoothFactor = 10;
thermocouple = [];%zeros(1,smoothFactor);
tmpAmb = [];%zeros(1,smoothFactor);
Tamb = [];
dT = [];
tmp = [];
tmpTemp = [];
tmpPin = 3;
TCpin = 5; %thermocouple pin
tmpAmbPin = 1; 

digiPin = 5;
readings = containers.Map();
for i = 41:1000000000
    
    thermocouple(i) = analogRead(a,TCpin);%mod(i,smoothFactor)
    tmpAmb(i) = analogRead(a, tmpAmbPin);%mod(i,smoothFactor)
    Tamb(i) = mean(tmpAmb(i-40:i))*.5682-3.822;%C
    dT(i) = mean(thermocouple(i-40:i))*.06192 - 6.576;%C
    tmp(i) = analogRead(a,tmpPin);
    tmpTemp(i) = mean(tmp(i-40:i))*.5682-3.822;%C
    plot(Tamb+dT, 'm')
    hold on;   
    plot(Tamb, 'c')
    plot(tmpTemp, 'r')
    title(['Temperature (TC) = ' num2str(Tamb(i)+dT(i)) 'C, Tmp Temp = ' num2str(tmpTemp(i)) 'C, Ambient temp = ' num2str(Tamb(i)) 'C, dT = ' num2str(dT(i)) ]);
    hold off;
    pause(.01);
    if digitalRead(a,digiPin) == 1
        [reading, stddev] = takeReading(a,tmpAmbPin,TCpin);
        name = inputdlg('Enter a reading name', 'Enter reading', 1);
        readings(name) = [reading, stddev];
    end
end