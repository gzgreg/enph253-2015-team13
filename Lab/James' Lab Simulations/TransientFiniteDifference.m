%Finite difference calculations for aluminum rod transients

%Temp sensor calibration stuff
factors = [1.79 1.81 1.53 1.46 2.06 2];
offsets = [4.14 2.25 0.16 3.64 -0.35 0];
colors = 'kbmgrk';

eps = 0.1; %emissivity
sigma = 5.670e-8; % W /^-2 K^-4
kc = 3; %convection constant
k = 130; %conduction constant
c = 910; %specific heat capacity
rho = 2700; %kg/m^3

Pin = 5.3; %input power from resistor
L = 0.305; %length
r = 0.0111; %radius
A = pi*r^2;

Tamb = mean(squeeze(readings(1, 1, 1:500))) * 500 / 1024; %ambient temperature

nstepsT = 4000;

time = 4000; %seconds

nstepsX = 20;

dt = time / nstepsT;
dx = L / nstepsX;

m = dx*A*rho;

T = zeros(nstepsT + 1, nstepsX + 1); %K
P = zeros(nstepsT + 1, nstepsX + 1); %W

T(1, :) = Tamb;
P(:, 1) = Pin;

for i = 1:(nstepsT)
    for j = 1:(nstepsX + 1)
        if j == nstepsX + 1
            S = dx*pi*r*2 + A;
            Pcond = 0;
            eps = 1;
            kc = 29;
            %Tamb = 0;
        else
            S = dx*pi*r*2;
            Pcond = -k*A*(T(i, j) - T(i, j+1))/dx;
            eps = 1;
            kc = 10; %represents heat loss through insulation: actually probably conduction
            %Tamb = T(1, 1);
        end
        Pconv = -S * (T(i, j) - Tamb) * kc;
        Prad = -eps * S * sigma * ((T(i, j) + 273.15)^4 - (Tamb+273.15)^4);
        Ptot = Pcond + Pconv + Prad + P(i, j);        
        P(i, j+1) = -Pcond;
        T(i+1, j) = Ptot*dt/(c*m) + T(i, j);
    end
end
plot((1:length(T(:, 1))), T(:, 1), 'c')
hold on
plot((1:length(T(:, 14))), T(:, 14), 'c')
plot((1:length(T(:, 8))), T(:, 8), 'c')
plot((1:length(T(:, 10))), T(:,10), 'c')
plot((1:length(T(:, 20))), T(:, 20), 'c')
for i=1:6
plot(squeeze(readings(3, i, :)), (squeeze(readings(1, i, :)) - offsets(i)) / factors(i), colors(i))
end
hold off;