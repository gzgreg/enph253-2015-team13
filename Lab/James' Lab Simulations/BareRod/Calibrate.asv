%% Data Calibration
%ENPH 257 - group 13
%2015.06.27
%Takes:     filename: The file name of the data you want to calibrate
%           rangeStart: start of data range
%           rangeEnd: end of data Range
%           ambPin: the ambient Temperature pin
%Returns: The temperature of each sensor

%%
function Tsensor = Calibrate(readings,rangeStart,rangeEnd,ambPin)

pin = [1:(ambPin-1) (ambPin+1):6];%pins in order that are NOT the ambient temp pin
Tsensor(pin(1),:) = 0.4737*squeeze(readings(1,1,rangeStart:rangeEnd)) - 1.02;

Tsensor(pin(2),:) = 0.4525*squeeze(readings(1,2,rangeStart:rangeEnd)) + .8524;

Tsensor(pin(3),:) = 0.5492*squeeze(readings(1,3,rangeStart:rangeEnd)) + 1.111;

Tsensor(pin(4),:) = 0.5179*squeeze(readings(1,4,rangeStart:rangeEnd)) + 2.053;

Tsensor(pin(5),:) = 0.4577*squeeze(readings(1,5,rangeStart:rangeEnd)) - 2.049;

Tsensor(ambPin,:) = (500/1023)*squeeze(readings(1,6,rangeStart:rangeEnd));

end