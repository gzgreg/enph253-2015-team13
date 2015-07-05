%% Data Calibration
%ENPH 257 - group 13
%2015.06.27
%Takes uncalibrated Arduino data and outputs calibrated temperature data 
%based off of calibration results for each temperature sensor

%%
function Tsensor = Calibrate(readings)
%figure
%plot(squeeze(readings(1,:,:))');
Tsensor(1,:) = 0.4737*squeeze(readings(1,1,:)) - 1.02;

Tsensor(2,:) = 0.4525*squeeze(readings(1,2,:)) + .8524;

Tsensor(3,:) = 0.5492*squeeze(readings(1,3,:)) + 1.111;

Tsensor(4,:) = 0.5179*squeeze(readings(1,4,:)) + 2.053;

Tsensor(5,:) = 0.4577*squeeze(readings(1,5,:)) - 2.049;

Tsensor(6,:) = (500/1023)*squeeze(readings(1,6,:));
%figure
%plot(Tsensor');
end