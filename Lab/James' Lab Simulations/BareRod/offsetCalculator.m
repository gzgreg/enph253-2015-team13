%%Post Calibration Offset Calculator
%Calculates an additional offset on the data based on the average
%temperature of all sensors before heating begins and all the temperature sensors
%are assumed to be at the same temperature

%Takes: transientDataFile: a string of the filename for the heating data
%       readRange: how many data points to average
%       ambPin: the number of the ambient temp in the readings
%Returns: a calibrated offset for each temp sensor

%%
function offset = offsetCalculator(transientDataFile,readRange,ambPin)
load(transientDataFile);
calibratedData = Calibrate(readings(1,[1:(ambPin-1) (ambPin+1):6 ambPin],:),1,readRange,ambPin);%calibrates data in readRange
offset1 = 1:6;
sensorDataC = 1:6;

for i = 1:6
    sensorDataC(i) = mean(calibratedData(i,:));%C, averages temperature at each sensor
end

averageTemp = mean(sensorDataC(:));

for i = 1:6
    offset1(i) = averageTemp - sensorDataC(i);
end

offset = offset1;
end
