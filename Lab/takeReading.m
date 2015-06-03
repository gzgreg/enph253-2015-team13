function [reading, stddev] = takeReading(arduino, tmpPin)
numReadings = 5;
tmpReading = zeros(1,numReadings);

for i = 1:numReadings
    tmpReading(i) = analogRead(arduino,tmpPin);%*5/1023*113.636363 - 18.181818;
end
    reading = mean(tmpReading);
    stddev = std(tmpReading);
end