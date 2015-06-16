%Finite difference calculations for aluminum rod transients

eps = 0.1; %emissivity
sigma = 5.670e-8; % W /^-2 K^-4
kc = 20; %convection constant
k = 205; %conduction constant
c = 910; %specific heat capacity
rho = 2700; %kg/m^3

Pin = 10; %input power from resistor
L = 0.305; %length
r = 0.0111; %radius
A = pi*r^2;

Tamb = mean(squeeze(readings(1, 6, 1:1000))) * 500 / 1024; %ambient temperature

nstepsT = 6000;

time = 6000; %seconds

nstepsX = 10;

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
            kc = 10;
        else
            S = dx*pi*r*2;
            Pcond = -k*A*(T(i, j) - T(i, j+1))/dx;
            eps = 0.1;
            kc = 6; %represents heat loss through insulation: actually probably conduction
        end
        Pconv = -S * (T(i, j) - Tamb) * kc;
        Prad = -eps * S * sigma * ((T(i, j) + 273.15)^4 - (Tamb+273.15)^4);
        Ptot = Pcond + Pconv + Prad + P(i, j);        
        P(i, j+1) = -Pcond;
        T(i+1, j) = Ptot*dt/(c*m) + T(i, j);
    end
end
plot((1:length(T(:, 1))) + 68, T(:, 1))
for i=1:6
hold on;
plot(squeeze(readings(3, i, 220:4130)), (squeeze(readings(1, i, 220:4130)) + offset(i)) / 2)
end
hold off;