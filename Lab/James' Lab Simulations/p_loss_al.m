function P = p_loss_al(T,dx,kc,radius)
P_conv = kc * 2 * pi * radius * dx *(T - Tamb);
P_rad = emsv * sigma * (2*pi*radius)*dx*(T^4-Tamb^4);
P = P_conv + P_rad;
end