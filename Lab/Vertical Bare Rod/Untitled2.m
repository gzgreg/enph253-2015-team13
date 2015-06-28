load('June3TransientHeating-Test2-ALLSYSTEMSNOMINAL.mat');


figure
for i = 1:6
    plot(squeeze(readings(1,i,:)),'color',rand(1,3));
    hold on;
    
end
legend('read1','read2','read3','read4','read5','read6');
title('uncalibrated');
hold off;

figure
calib = Calibrate(readings);
for i = 1:6
    plot(squeeze(calib(i,:)),'color',rand(1,3));
    hold on;
    
end
legend('read1','read2','read3','read4','read5','read6');
title('calibrated');