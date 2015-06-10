function P = p_loss_elec_tape(T,dx,kc,radius)
P_conv = kc * 2 * pi * radius * dx *(T - Tamb);
P_rad = emsv_elec_tape * sigma * (2*pi*radius)*dx*(T^4-Tamb^4);
P = P_conv + P_rad;
end