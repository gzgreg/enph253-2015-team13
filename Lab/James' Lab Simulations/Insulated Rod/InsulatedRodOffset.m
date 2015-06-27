%2015.06.10
%ENPH 257 Lab - Insulated rod simulation

clear;

%Change to local directory if running this on your machine
load('C:\Users\James\Documents\MATLAB\ENPH 257\Lab Simulations\Insulated Rod\June1TransientInsulatedHeating.mat')

offset = 1:6;

%figure
for i = 1:6
   % plot(squeeze(readings(1,i,:)),'color',rand(1,3));
    offset(i) = mean(squeeze(readings(1,6,1:200))) - mean(squeeze(readings(1,i,1:200)));
    hold on;
end
%legend('read1','read2','read3','read4','read5','read6');
%title('Transient - unoffset');

%--------------------------------------------------------------------------

%figure
for i = 1:6
   % plot(squeeze(readings(1,i,:)) + offset(i) ,'color',rand(1,3));     
    hold on;
end
%legend('read1','read2','read3','read4','read5','read6');
%title('Transient - offset');

%--------------------------------------------------------------------------

load('C:\Users\James\Documents\MATLAB\ENPH 257\Lab Simulations\Insulated Rod\June1SteadyStateInsulatedHeating.mat')

%{
figure
for i = 1:6
    %plot(squeeze(readings(1,i,:)),'color',rand(1,3));
    hold on;
end
%legend('read1','read2','read3','read4','read5','read6');
%title('Steady State - unoffset');

%--------------------------------------------------------------------------

%figure
for i = 1:6
   % plot(squeeze(readings(1,i,:)) + offset(i) ,'color',rand(1,3));     
    hold on;
end
%legend('read1','read2','read3','read4','read5','read6');
title('Steady State - offset');
%}