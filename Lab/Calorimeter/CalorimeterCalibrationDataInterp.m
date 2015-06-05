%2015.06.05
%code to interperate calorimeter calibration data

load('C:\Users\James\Desktop\Robot\enph253-2015-team13\Lab\Calorimeter\calorimeterCalibration.mat')

time = squeeze(readings(3,1,:));
tmp = squeeze(readings(1,2,:));
tmp_std = squeeze(readings(2,2,:));
tmpWeights = 1./tmp_std.^2;

tmpAmb = squeeze(readings(1,1,:));
tmpAmb_std = squeeze(readings(2,1,:));

plot(time,tmpAmb,time,tmpAmb);
plot(time,tmp);

%do a curve fit once we have the correct equations for the tmp sensor to
%find out the heat loss rate of the calorimeter.