%% Vertical Bare Rod - Top Heating Steady State simulation
%2015.06.03
%ENPH 257 Lab - Group 13

%%

clear all;
close all;
%Load the results 
load('june10VertBareRodSteadyState-TopPowered');

%%

radius = 0.0111; %m
length = 0.305; %m
nstep = 50;
dx = length/nstep;%m
%%

%Thermo constants
k = 200; %W / (m * K) - conduction
sigma = 5.67e-8;%W / (m^2 * K^4) stefan-boltzman const
emsv = 0.2; % emissivity
fudge = 1.0; %fudge factor for convection
alpha = 1.9e-5;  %m^2/s kinematic viscosity of air
g = 9.81; %m/s^2
kc = 20; %W / (m^2 * K)
emsv_elec_tape = 0.95;
width_tape = .020;%m, width of the electrical tape
pwrR_Area = ((.0155*.0207) + 2*(.0155*.002) + 2*(.0207*.002));%m^2, area of pwr resistor
emsvR = 0.8;%emsivity of power resistor
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

readRangeStart = 1;
readRangeEnd = 100;
sensorDataC = 1:6;

offset = offsetCalculator('June10VertBareRodHeating-TopPower',20,1);
calibratedData = Calibrate(readings,readRangeStart,readRangeEnd,1);%calibrates data in reading range
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
%% $P_{conv} = k_c  (Area) dx (T(1) - T_{amb})$
P_conv_end = kc * pi * radius^2 * (T(1) - Tamb);
P_conv_cyl = kc * 2 * pi * radius * dx * (T(1) - Tamb);%convection power loss for the cylindrical part of the end of the rod
%% $P_{rad} = \sigma  (Area) dx (T(1)^4 - T_{amb}^4)$
P_rad_end = sigma * emsv * pi * radius^2 *(T(1)^4 - Tamb^4);
P_rad_cyl = sigma * emsv_elec_tape * 2 * pi * radius * dx *(T(1)^4 - Tamb^4);

P_out = P_conv_end + P_conv_cyl + P_rad_end + P_rad_cyl;
P_in = P_out;
x(1) = dx;

%%

for i = 2:nstep
   x(i) = i * dx;
   P_out = P_in;
   T(i) = T(i-1);
   
   %is the slice covered in electrical tape or not?
   if (x < t5end) | (x > t4st & x < t4end) | (x > t3st & x < t3end) | (x > t2st & x < t2end) | (x > t1st) 
       P_conv_cyl = kc * 2 * pi * radius * dx *(T(i) - Tamb);
       P_rad = emsv_elec_tape * sigma * (2*pi*radius)*dx*(T(i)^4-Tamb^4);
       P_loss = P_conv_cyl + P_rad;
   else
       P_conv_cyl = kc * 2 * pi * radius * dx *(T(i) - Tamb);
       P_rad = emsv * sigma * (2*pi*radius)*dx*(T(i)^4-Tamb^4);
       P_loss = P_conv_cyl + P_rad;
   end
   
   P_in = P_out + P_loss;
   dT = P_in * dx/(k * pi * radius^2);
   T(i) = T(i) + dT;
end

%%
%pwrR_loss = emsvR * sigma * pwrR_Area *dx*(T(nstep)^4-Tamb^4) + kc * pwrR_Area * dx *(T(nstep) - Tamb);%power loss from power resistor
pwrR_rod = P_in;%the power going into the rod is the power going into the last slice (which is the slice adjancent to the power resistor)
pwrR_tot = 9*.6;%W, 9V*0.6A, this should equal the power loss plus the power in
pwrFract  = pwrR_rod/(pwrR_tot);%fraction of power going into rod



%%

figure
plot(length - x,T-273);
hold on
errorbar(length - sensorPos,sensorDataC(1:5),[3 3 3 3 3],'ro');
plot(x,Tamb-273,'r');
title('Simulation of Vertical Bare - Top Heating Rod');
legend('Model','data','Ambient temp');
xlabel('{\it x} (m)')
ylabel('{\it T} (C)')
set(gca, 'FontSize', 16)
set(gca, 'FontName', 'TimesRoman')