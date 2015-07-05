%2015.06.10
%ENPH 257 - Insulated Rod simulation

close all;
%load tmp sensor calibration file
run InsulatedRodOffset.m;

radius = 0.0111; %m, 
length = 0.305; %m
nstep = 50;
dx = length/nstep;%m

%Thermo constants
k = 205; %W / (m * K) - conduction
k_air = 0.026; %W / (m^2 * K^4) - radiation
sigma = 5.67e-8;%W / (m^2 * K^4) stefan-boltzman const
emsv = 0.210; % emissivity
insulFudge = 5; %fudge factor for power loss through insulation
alpha = 1.9e-5;  %m^2/s kinematic viscosity of air
g = 9.81; %m/s^2
kc = 12; %W / (m^2 * K)
pwrFrac = .8;%power fraction
%pwrR_Area = ((15.5*20.7) + 2*(15.5*2) + 2*(20.7*2))*10^-6;%m^2, area of pwr resistor
%emsvR = 0.8;%emsivity of power resistor
%measurement points
h5 = length - 0.006;%m, distance from power resistor
h4 = length - 0.105;%m, distance from power resistor
h3 = length - 0.163;%m, distance from power resistor
h2 = length - 0.207;%m, distance from power resistor
h1 = length - 0.298;%m, distance from power resistor


readRangeStart = 1;
readRangeEnd = 600;
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



%Start conditions
resistorPwr = 12*0.8;%W, 12V 0.8A
%P_conv = kc * pwrR_Area * (T(1) - Tamb);
%P_rad = sigma * emsvR * pwrR_Area * (T(1)^4 - Tamb^4);

P_in = resistorPwr*pwrFrac;
P_out = P_in;
x(1) = dx;


for i = 2:nstep
    x(i) = i*dx;
    T(i) = T(i-1);
    P_in(i) = P_out;
    P_loss = insulFudge*(2*pi*radius*dx)*(T(i) - Tamb);
    P_out = P_in(i) - P_loss;
    dT = -P_in(i) * dx/(k * pi * radius^2);
    T(i) = T(i) - dT;
end

plot(P_in)
figure
plot(x,T-273);
hold on
xlabel('{\it x} (m)')
ylabel('{\it T} (C)')
set(gca, 'FontSize', 16)
set(gca, 'FontName', 'TimesRoman')

plot(sensorPos,sensorDataC(1:5),'ro');
plot(x,Tamb-273,'r');

