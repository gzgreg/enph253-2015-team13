figure
for i =1:6
   plot(squeeze(readings(1,i,:)),'color',rand(1,3)); 
   hold on;
   offset = moistOffsetCalculator('June12MoistRodHeating',20,1);   
    
end
title('uncalbrated');
figure
calibrated = moistCalibrate(readings(1,i,:),1,size(squeeze(readings(1,1,:))),1);
for i = 1:6
    plot(calibrated(i),'color',rand(1,3));    
    
end
title('calibrated')

figure
for i =1:6
    plot(calibrated(i) + offset(i),'color',rand(1,3));
end