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
Tsensor(1,:) = 0.4737*squeeze(readings(1,pin(1),rangeStart:rangeEnd)) - 1.02;

Tsensor(2,:) = 1.023*squeeze(readings(1,pin(2),rangeStart:rangeEnd)) + .8524;

Tsensor(3,:) = 0.8031*squeeze(readings(1,pin(3),rangeStart:rangeEnd)) + 1.111;

Tsensor(4,:) = 0.5179*squeeze(readings(1,pin(4),rangeStart:rangeEnd)) + 2.053;

Tsensor(5,:) = 0.4577*squeeze(readings(1,pin(5),rangeStart:rangeEnd)) - 2.049;

Tsensor(6,:) = (500/1023)*squeeze(readings(1,ambPin,rangeStart:rangeEnd));

end