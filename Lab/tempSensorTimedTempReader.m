tmpAmb = [];
Tamb = [];
tmpTemp = [];
tmpPin = 0:5;
tmpAmbPin = 0; 
digiPin = 5;
readingHolder = zeros(3, 6);
readings = zeros(3, 6, 1);
elapsTime = tic();
i = 1;
while ~digitalRead(a, digiPin)
    for j=tmpPin
        [reading, stddev] = takeReading(a, j);
        readingHolder(:, j+1) = [reading stddev toc(elapsTime)];
    end
    
    readings(:, :, i) = readingHolder;
    plot(squeeze(readings(1, 1, :)), 'r')
    hold on;
    plot(squeeze(readings(1, 2, :)), 'b')
    plot(squeeze(readings(1, 3, :)), 'm')
    plot(squeeze(readings(1, 4, :)), 'g')
    plot(squeeze(readings(1, 5, :)), 'k')
    plot(squeeze(readings(1, 6, :)), 'y')
    i = i+1;
    digitalRead(a, digiPin);
    pause(0.01);
end
hold off;

%{
description of readings:
First dimension is stats for an individual reading. 1 is reading value, 2
is reading standard deviation, 3 is reading time.
Second dimension is the particular temp sensor. 1-5 are sensors from
closest to farthest from power resistor: 6 is ambient temp.
Third dimension is a particular reading set taken within one loop run
above.
%}