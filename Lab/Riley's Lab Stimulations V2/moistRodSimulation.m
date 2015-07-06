%% Horizontal Moist rod Steady State simulation
%2015.06.03
%ENPH 257 Lab - Group 13

%%

clear all;
close all;
%Load the results 
load('June12MoistRodSteadyState.mat');

%%

radius = 0.0111; %m
length = 0.305; %m
nstep = 50;
dx = length/nstep;%m

%%

%Thermo constants
k = 200; %W / (m * K) - conduction
sigma = 5.67e-8;%W / (m^2 * K^4) stefan-boltzman const
emsv = 0.2; %emissivity 
moist_kc = 5.0; %W / (m^2 * K)
fudgeE = 1.5; %fudge factor for evaporation
alpha = 1.9e-5;%m^2/s kinematic viscosity of air
g = 9.81; %m/s^2
kc = 20.0; %W / (m^2 * K)
emsv_elec_tape = 0.95;
emsv_cloth = 0.9;
width_tape = .020;%m, width of the electrical tape
pwrR_Area = ((15.5*20.7) + 2*(15.5*2) + 2*(20.7*2))*10^-6;%m^2, area of pwr resistor
emsvR = 0.8;%emsivity of power resistor
latentHeatWaterEvap = 2260; %J/g
totalEvapPowerLoss = 0.0; %0.0 Value is a placeholder until loop is exicuted
%%

%measurement points
h5 = 0.006;%m, distance from endhole
h4 = 0.105;%m, distance from endhole
h3 = 0.163;%m, distance from endhole
h2 = 0.207;%m, distance from endhole
h1 = 0.298;%m, distance from endhole

t1st = h1 - width_tape/2;
t2st = h2 - width_tape/2;
t2end = h2 + width_tape/2;
t3st = h3 - width_tape/2;
t3end = h3 + width_tape/2;
t4end = h4 + width_tape/2;
t4st = h4 - width_tape/2;
t5end = h5 + width_tape/2;

%%
ambPin = 1;
readRangeStart = 1;
readRangeEnd = 400;
sensorDataC = 1:6;

offset = moistOffsetCalculator('June12MoistRodHeating.mat',120,ambPin);
calibratedData = moistCalibrate(readings,readRangeStart,readRangeEnd,ambPin);%calibrates data in reading range
for i = 1:6
    sensorDataC(i) = mean(calibratedData(i,:)) + offset(i);%C, averages temperature at each sensor and applies additional offset
end

sensorPos = [h1 h2 h3 h4 h5]; %from end hole

x = 1:nstep;%just placeholder data
T = 1:nstep;%just placeholder data

Tamb = sensorDataC(6)+273;%K
T(1) = sensorDataC(5)+273;%K


%%

%End conditions

P_conv_end = kc * pi * radius^2 * (T(1) - Tamb);
% $P_{conv} = k_c  (2 \pi r) dx (T(1)^4 - T_{amb}^4)$
P_conv_an = kc * 2 * pi * radius * dx * (T(1) - Tamb);%convection power loss for the annulus of the end of the rod
P_rad_end = sigma * emsv * pi * radius^2 *(T(1)^4 - Tamb^4);
P_rad_an = sigma * emsv_elec_tape * 2 * pi * radius * dx *(T(1)^4 - Tamb^4);

P_out = P_conv_end + P_conv_an + P_rad_end + P_rad_an;
P_in = P_out;
x(1) = dx;

%%

for i = 2:nstep
   x(i) = i * dx;
   P_out = P_in;
   T(i) = T(i-1);
   
  
   
   %is the slice on the tape or not?
 if (x(i) < t5end) || (x(i) > t4st && x(i) < t4end) || (x(i) > t3st && x(i) < t3end) || (x(i) > t2st && x(i) < t2end) || (x(i) > t1st) 
       P_conv = kc * 2 * pi * radius * dx *(T(i) - Tamb);
       P_rad = emsv_elec_tape * sigma * (2*pi*radius)*dx*(T(i)^4-Tamb^4);
       P_loss = P_conv + P_rad;
 else
     
     if (x(i) < t4st && x(i) > t5end )
           P_conv = moist_kc * 2 * pi * radius * dx *(T(i) - Tamb);
           P_rad = emsv_cloth * sigma * (2*pi*radius) * dx * (T(i)^4-Tamb^4);
           P_evap = fudgeE * dx * (T(i)-Tamb);
           P_loss = P_conv + P_rad + P_evap;
           
           totalEvapPowerLoss = totalEvapPowerLoss + P_evap;
           
       else
   
           P_conv = kc * 2 * pi * radius * dx *(T(i) - Tamb);
           P_rad = emsv * sigma * (2*pi*radius)*dx*(T(i)^4-Tamb^4);
           P_loss = P_conv + P_rad;
       end
   end
   
   P_in = P_out + P_loss;
   dT = P_in * dx/(k * pi * radius^2);
   T(i) = T(i) + dT;
end

%%
pwrR_loss = emsvR * sigma * pwrR_Area *dx*(T(nstep)^4-Tamb^4) + kc * pwrR_Area * dx *(T(nstep) - Tamb);%power loss from power resistor
pwrR_rod = P_in;%the power going into the rod is the power going into the last slice (which is the slice adjancent to the power resistor)
pwrR_tot = 9*.6;%W, 9V*0.6A, this should equal the power loss plus the power in
pwrFract  = pwrR_rod/(pwrR_tot);%fraction of power going into rod
display(pwrR_tot)
display(pwrR_loss+pwrR_rod);


%%

figure
plot(length - x,T-273);
hold on
errorbar(length - sensorPos,sensorDataC(1:5),[3 3 3 3 3],'ro');
plot(x,Tamb-273,'r');
title('Simulation of Horizontal Moist Rod');
legend('Model','data','Ambient temp');
xlabel('{\it x} (m)')
ylabel('{\it T} (C)')
set(gca, 'FontSize', 16)
set(gca, 'FontName', 'TimesRoman')

waterEvapPerSec = totalEvapPowerLoss / latentHeatWaterEvap %g/s
display(totalEvapPowerLoss)%W


