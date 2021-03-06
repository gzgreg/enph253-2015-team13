%% Insulated Rod Simulation
%ENPH 257 -group 13

%%
close all;
%load data
load('June1SteadyStateInsulatedHeating.mat')
%%
radius = 0.0111; %m, 
length = 0.305; %m
nstep = 50;
dx = length/nstep;%m
%%
%Thermo constants
k = 180; %W / (m * K) - conduction
insulFudge = 4; %fudge factor for power loss through insulation
pwrFrac = .9;%power fraction
%%
%measurement points
h5 = length - 0.006;%m, distance from power resistor
h4 = length - 0.105;%m, distance from power resistor
h3 = length - 0.163;%m, distance from power resistor
h2 = length - 0.207;%m, distance from power resistor
h1 = length - 0.298;%m, distance from power resistor

%% Calibration
readRangeStart = 1;
readRangeEnd = 50;
sensorDataC = 1:6;
offset = offsetCalculator('June1TransientInsulatedHeating.mat',200,6);
calibratedData = Calibrate(readings,readRangeStart,readRangeEnd,6);%calibrates data in reading range
for i = 1:6
    sensorDataC(i) = mean(calibratedData(i,:)) + offset(i);%C, averages temperature at each sensor and applies additional offset
end
sensorPos = [h1 h2 h3 h4 h5]; %from end hole

x = 1:nstep;%just placeholder data
T = 1:nstep;%just placeholder data

Tamb = sensorDataC(6)+273;%K
T(1) = sensorDataC(1)+273;%K



%%
%Start conditions
resistorPwr = 12*0.8;%W, 12V 0.8A
P_in = resistorPwr*pwrFrac;
P_out = P_in;
x(1) = dx;


for i = 2:nstep
    x(i) = i*dx;
    T(i) = T(i-1);
    P_in(i) = P_out;
    P_loss = insulFudge*(2*pi*radius*dx)*(T(i) - Tamb);
    P_out = P_in(i) - P_loss;
    dT = P_in(i) * dx/(k * pi * radius^2);
    T(i) = T(i) - dT;
end
display(P_out);%power going out heat sink
%%
figure
plot(x,T-273);
hold on
errorbar(sensorPos,sensorDataC(1:5),[3 3 3 3 3],'ro');
plot(x,Tamb-273,'r');
title('Steady State Heat Transfer in an Insulated Aluminum Rod');
xlabel('{\it x} (m)')
ylabel('{\it T} (C)')
set(gca, 'FontSize', 16)
set(gca, 'FontName', 'TimesRoman')

legend('Model','Data','Ambient temp');
