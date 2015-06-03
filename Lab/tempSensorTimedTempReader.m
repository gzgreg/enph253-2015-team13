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