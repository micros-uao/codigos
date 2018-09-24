close all;
clear all;
load('filtroecg.mat');

load('F1071230.mat');

ecg1=e1071230;
fid = fopen('ecg.txt', 'w');
fprintf(fid,'%6.4f \n', ecg1);
fclose(fid);
t=0:0.0018:0.0018*5999;
y=(1+sin(2*pi*60*t))*100;
ecgr=y+ecg1;
fid2=fopen('ecgr.txt', 'w');
fprintf(fid2,'%6.4f \n', ecgr);
fclose(fid2);


c=convmtx(Numfir',length(ecgr));
y1=c*ecgr';
y1=y1(1:6000)';

y2=filter(filt1.Numerator{1,1},filt1.Denominator{1,1},ecgr);

subplot(2,2,1)
plot(t,ecg1,'linewidth',2);
title('ECG pure signal')
xlabel('t (s)')
ylabel('A(mV)')
subplot(2,2,2)
plot(t,ecgr,'linewidth',2);
title('ECG + noise signal')
xlabel('t (s)')
ylabel('A(mV)')
subplot(2,2,3)
plot(t,y1,'linewidth',2);
title('FIR filter')
xlabel('t (s)')
ylabel('A(mV)')
subplot(2,2,4)
plot(t,y2,'linewidth',2);
title('IIR filter')
xlabel('t (s)')
ylabel('A(mV)')

