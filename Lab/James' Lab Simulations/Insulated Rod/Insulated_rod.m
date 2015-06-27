%2015.06.01
%Simulation of transient and steady state heat conduction in an aluminum
%rod wrapped in inslusation


Tamb = [0];%C , ambient temperature
T1 = [20];%C, tmp 1
T2 = [15];%C, tmp 2
T3 = [14];%C, tmp 3
T4 = [10];%C, tmp 4
T5 = [5];%C, tmp 5

inclineDataRange = [1 1];%range for data points for heating
steadyDataRange = [1 1];%range for data points for steady state
declineDataRange = [1 1];%range for data points for cooling

L1 = 0;%m, distance to tmp 1
L2 = 5;%m, distance to tmp 2
L3 = 6;%m, distance to tmp 3
L4 = 7;%m, distance to tmp 4
L5 = 10;%m, distance to tmp 5

radius = 1;%m, radius of rod
nArea = pi*radius^2;%m^2, area of slice of rod


%Steady State:

%-Ideal conditions (no convection or radiation)
T1ss = mean(T1(steadyDataRange(1):steadyDataRange(2)));
T2ss = mean(T2(steadyDataRange(1):steadyDataRange(2)));
T3ss = mean(T3(steadyDataRange(1):steadyDataRange(2)));
T4ss = mean(T4(steadyDataRange(1):steadyDataRange(2)));
T5ss = mean(T5(steadyDataRange(1):steadyDataRange(2)));

Qk = (nArea/(L5-L1))*(T5ss - T1ss);

x = linspace(L1,L5,100);
Tx = T1ss + (Qk/nArea)*x;

%plot(x,Tx,'b',[L1 L2 L3 L4 L5], [T1ss T2ss T3ss T4ss T5ss], 'o');


%-With convection and radiation
k = 10;%conduction constant
nStep = 50;
fudge = 1;%factor
P_in = 10;%W, guessed value, power resistor put in an upper bound of ~10W

T = zeros(1,nStep);
x = linspace(L1,L5,nStep+1);
dx = (L5-L1)/nStep;
aArea = 2*pi*radius*dx;%m^2, area of outer edge of rod
T(1) = T1ss;
for i = 1:nStep
   
   P_loss = fudge * aArea * T(i);%guessing that the lossed power is proportional to area and temperature  
   P_out = P_in - P_loss;
   dT = P_out * dx/(k*aArea);
   T(i+1) = T(i) + dT;
end

plot(x,T,[L1 L2 L3 L4 L5], [T1ss T2ss T3ss T4ss T5ss], 'o'); 



























