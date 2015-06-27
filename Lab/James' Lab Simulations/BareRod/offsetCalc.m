%2015.06.10
%ENPH 257, Bare rod test, Offset calculation

load('June3TransientHeating-Test2-ALLSYSTEMSNOMINAL.mat');

offset = 1:6;
figure
for i = 1:6
    plot(squeeze(readings(1,i,1:80)),'color',rand(1,3));
    hold on;
    offset(i) = mean(squeeze(readings(1,6,1:80))) - mean(squeeze(readings(1,i,1:80)));
end
legend('read1','read2','read3','read4','read5','read6');
title('offset sample');

%--------------------------------------------------------------------------

figure
for i = 1:6
    plot(squeeze(readings(1,i,:))+offset(i),'color',rand(1,3));
    hold on;
end
legend('read1','read2','read3','read4','read5','read6');
title('transient with offset applied');

%--------------------------------------------------------------------------

%this load will overwrite 'readings' so make sure you're done with it by
%this point
load('C:\Users\James\Desktop\Robot\enph253-2015-team13\Lab\Bare_rod_test_June3\June3TransientSteadyState');

%verify that the pins are in the correct order before applying the offset
figure
for i = 1:6
    plot(squeeze(readings(1,i,:)),'color',rand(1,3));
    hold on;
end
legend('read1','read2','read3','read4','read5','read6');
title('Steady State - unoffset');

%--------------------------------------------------------------------------

figure
for i = 1:6
    plot(squeeze(readings(1,i,:))+offset(i),'color',rand(1,3));
    hold on;
end
legend('read1','read2','read3','read4','read5','read6');
title('Steady State - offset');





