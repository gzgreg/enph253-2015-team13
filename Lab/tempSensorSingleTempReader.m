tmpAmb = [];%zeros(1,smoothFactor);
Tamb = [];
tmpTemp = [];
tmpPin = 1:5;
tmpAmbPin = 0; 
digiPin = 5;
readings = zeros(2, 5);
for i = 41:1000000000
    Tamb(i) = mean(analogRead(a, tmpAmbPin))*.5682-3.822;%C
    tmpTemp(i) = mean(analogRead(a,tmpPin(1)))*.5682-3.822;%C
    plot(Tamb, 'c')
    hold on;
    plot(tmpTemp, 'r')
    title(['Tmp Temp = ' num2str(tmpTemp(i)) 'C, Ambient temp = ' num2str(Tamb(i)) 'C']);
    hold off;
    pause(.01);
    if digitalRead(a,digiPin) == 1
        for j=tmpPin
            [reading, stddev] = takeReading(a, j);
            readings(:, j) = [reading stddev];
        end
    end
end