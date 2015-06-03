
time = [ ];
thermocouple = [];
tmp = [];
tmpAmb = [];

TCpin = 5; %thermocouple pin
tmpPin = 3;
tmpAmbPin = 1; 
elapsTime = tic();
for i = 1:1000000000
    
    thermocouple(i) = analogRead(a,TCpin);
    tmp(i) = analogRead(a, tmpPin);
    tmpAmb(i) =analogRead(a, tmpAmbPin);
    time(i) = toc(elapsTime);
    plot(time,thermocouple, 'm')
    hold on;   
    plot( time, tmp, 'g')
    plot( time, tmpAmb, 'y')
    title([num2str(toc(elapsTime))]);
    hold off;
    pause(.05);
    
end