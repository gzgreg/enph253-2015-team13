function F = transientFinDiffFuncNonLin(param, readings, tOffset, reading1, readingF) 
    %Finite difference calculations for aluminum rod transients
    %param meanings: 1 = k, 2 = kcIn, 3 = epsIn, 4 = kcOut, 5 = epsOut
    
    %Temp sensor calibration stuff
    factors = [1.79 1.81 1.53 1.46 2.06 2];
    offsets = [4.14 2.25 0.16 3.64 -0.35 0];

    sigma = 5.670e-8; % W /^-2 K^-4
    c = 910; %specific heat capacity
    rho = 2700; %kg/m^3

    Pin = 9.9; %input power from resistor
    L = 0.305; %length
    r = 0.0111; %radius
    A = pi*r^2;

    Tamb = 26.9195; %ambient temperature

    nstepsT = 6420;

    time = 6420; %seconds

    nstepsX = 22;

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
                eps = param(5);
                kc = param(4);
            else
                S = dx*pi*r*2;
                Pcond = -param(1)*A*(T(i, j) - T(i, j+1))/dx;
                eps = param(3);
                kc = param(2); %represents heat loss through insulation: actually probably conduction
            end
            Pconv = -S * (T(i, j) - Tamb) * kc;
            Prad = -eps * S * sigma * ((T(i, j) + 273.15)^4 - (Tamb+273.15)^4);
            Ptot = Pcond + Pconv + Prad + P(i, j);        
            P(i, j+1) = -Pcond;
            T(i+1, j) = Ptot*dt/(c*m) + T(i, j);
        end
    end
    rng = reading1:readingF;
    F(1, :) = interp1((1:length(T(:, 1)))+tOffset, T(:, 1), squeeze(readings(3, 1, rng))) - (squeeze(readings(1, 1, rng)) - offsets(1))/factors(1);
    F(2, :) = interp1((1:length(T(:, 8)))+tOffset, T(:, 8), squeeze(readings(3, 2, rng))) - (squeeze(readings(1, 2, rng)) - offsets(2))/factors(2);
    F(4, :) = interp1((1:length(T(:, 10)))+tOffset, T(:, 10), squeeze(readings(3, 3, rng))) - (squeeze(readings(1, 3, rng)) - offsets(3))/factors(3);
    F(3, :) = interp1((1:length(T(:, 14)))+tOffset, T(:, 14), squeeze(readings(3, 4, rng))) - (squeeze(readings(1, 4, rng)) - offsets(4))/factors(4);
    F(5, :) = interp1((1:length(T(:, 22)))+tOffset, T(:, 22), squeeze(readings(3, 5, rng))) - (squeeze(readings(1, 5, rng)) - offsets(5))/factors(5);
end

