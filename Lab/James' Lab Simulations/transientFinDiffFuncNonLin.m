function F = transientFinDiffFuncNonLin(param, readings, tOffset, reading1, readingF, amb1, Pin, eq, iceEnd) 
    %Finite difference calculations for aluminum rod transients
    %param meanings: 1 = k, 2 = kcIn, 3 = epsIn, 4 = kcOut, 5 = epsOut
    
    %Temp sensor calibration stuff
    factors = [1.79 1.81 1.53 1.46 2.06 2];
    offsets = [4.14 2.25 0.16 3.64 -0.35 0];
    sensorPos = [1 7 9 12 19];

    sigma = 5.670e-8; % W /^-2 K^-4
    c = 910; %specific heat capacity
    rho = 2700; %kg/m^3

    L = 0.305; %length
    r = 0.0111; %radius
    A = pi*r^2;

    if(amb1)
        Tamb = mean(squeeze(readings(1, 1, 1:500))) * 500 / 1024; %ambient temperature
    else
        Tamb = mean(squeeze(readings(1, 6, 1:500))) * 500 / 1024;
    end

    nstepsT = ceil(readings(3, 6, readingF));

    time = nstepsT; %seconds

    nstepsX = 19;

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
                if(eq)
                    eps = param(3);
                    kc = param(2);
                else
                    eps = param(5);
                    kc = param(4);
                end
                if(iceEnd) Tamb = 0; end;%(ice water)
            else
                S = dx*pi*r*2;
                Pcond = -param(1)*A*(T(i, j) - T(i, j+1))/dx;
                eps = param(3);
                kc = param(2); %represents heat loss through insulation: actually probably conduction
                if(iceEnd) Tamb = T(1, 1); end; %(ice water)
            end
            Pconv = -S * (T(i, j) - Tamb) * kc;
            Prad = -eps * S * sigma * ((T(i, j) + 273.15)^4 - (Tamb+273.15)^4);
            Ptot = Pcond + Pconv + Prad + P(i, j);        
            P(i, j+1) = -Pcond;
            T(i+1, j) = Ptot*dt/(c*m) + T(i, j);
        end
    end
    rng = reading1:readingF;
    for i = 1:5
        if(i == 4); j = 3; elseif(i == 3); j=4; else j = i; end
        F(j, :) = interp1((1:length(T(:, sensorPos(i))))+tOffset, T(:, sensorPos(i)), squeeze(readings(3, j, rng))) - (squeeze(readings(1, j, rng)) - offsets(i))/factors(i);
    end
end

