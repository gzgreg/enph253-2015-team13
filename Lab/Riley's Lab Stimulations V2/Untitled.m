clear;
close all;

load('June12MoistRodHeating');
figure
for i =1:6
   plot(squeeze(readings(1,i,:)),'color',rand(1,3)); 
   hold on;
      
    
end
title('uncalbrated');
figure
offset = moistOffsetCalculator('June12MoistRodHeating',20,1);
calibrated = moistCalibrate(readings(1,:,:),1,3500,1);
for i = 1:6
    plot(calibrated(i,:),'color',rand(1,3));    
    hold on;
end
title('calibrated')

figure
for i =1:6
    plot(calibrated(i,:) + offset(i),'color',rand(1,3));
    hold on;
end
legend('1','2','3','4','5','6')