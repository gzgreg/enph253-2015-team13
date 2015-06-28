%% Data Calibration
%ENPH 257 - group 13
%2015.06.27
%Takes uncalibrated Arduino data and outputs calibrated temperature data 
%based off of calibration results for each temperature sensor

%%
function Tsensor = Calibrate(readings)

Tsensor(1,:) = (squeeze(readings(1,1,:)) - 4.14)/1.79;

Tsensor(2,:) = (squeeze(readings(1,2,:)) - 2.25)/1.81;

Tsensor(3,:) = (squeeze(readings(1,3,:)) - 0.16)/1.53;

Tsensor(4,:) = (squeeze(readings(1,4,:)) - 3.64)/1.46;

Tsensor(5,:) = (squeeze(readings(1,5,:)) + 0.35)/2.06;

Tsensor(6,:) = (500/1023)*squeeze(readings(1,6,:));
end