close all;
clear all;
load('firallpass.mat');
load('iirallpass.mat');
t=0:0.001:0.001*1000;
ys1=100*sin(2*pi*60*t);
ys2=100*sin(2*pi*120*t);
ys3=100*sin(2*pi*240*t);
ys4=ys1+ys2+ys3+250;

Fs = 1000;            % Sampling frequency                    
T = 1/Fs;             % Sampling period       
L = length(ys4);      % Length of signal
t1 = (0:L-1)*T;        % Time vector
Y=fft(ys4);
P2=abs(Y/L);
P1=P2(1:L/2+1);
P1(2:end-1) = 2*P1(2:end-1);
f=Fs*(0:(L/2))/L;


yfir=filter(firallpass.Numerator{1,1},firallpass.Denominator{1,1},ys4);

yiir=filter(iirallpass.Numerator{1,1},iirallpass.Denominator{1,1},ys4);
figure
subplot(2,2,1)
plot(t,ys4);
title('Result signal')
xlabel('Samples (n)')
ylabel('|A(mV)|')
subplot(2,2,2)
plot(f,P1) 
title('Single-Sided Amplitude Spectrum of X(t)')
xlabel('f (Hz)')
ylabel('|P1(f)|')
subplot(2,2,3)
plot(t,yfir);
title('FIR filter')
xlabel('Samples (n)')
ylabel('|A(mV)|')
subplot(2,2,4)
plot(t,yiir);
title('IIR filter')
xlabel('Samples (n)')
ylabel('|A(mV)|')

t=0:0.0001:0.0001*300;
ys1fi=250*sin(2*pi*60*t-9.45);
ys2fi=250*sin(2*pi*120*t-18.81);
ys3fi=250*sin(2*pi*240*t-37.7);
ys4fi=ys1fi+ys2fi+ys3fi+500;



figure
subplot(3,2,1)
plot(t,ys1fi,'Color',[0, 0.4470, 0.7410],'linewidth',3);
title('signal 60 Hz after filter phi=-9.45 rads')
xlabel('t (s)')
ylabel('A(mV)')

subplot(3,2,3)
plot(t,ys2fi,'Color',[0.4660, 0.6740, 0.1880],'linewidth',3);
title('signal 120 Hz after filter phi=-18.81 rads')
xlabel('t (s)')
ylabel('A(mV)')

subplot(3,2,5)
plot(t,ys3fi,'Color',[0.4940, 0.1840, 0.5560],'linewidth',3);
title('signal 240 Hz after filter phi=-37.7 rads')
xlabel('t (s)')
ylabel('A(mV)')

subplot(3,2,2)
plot(t,ys1fi,'Color',[0, 0.4470, 0.7410],'linewidth',3);
hold on
plot(t,ys2fi,'Color',[0.4660, 0.6740, 0.1880],'linewidth',3);
plot(t,ys1fi+ys2fi,'Color','cyan','linewidth',3);
title('signal 60,120,60+120 Hz after filter')
xlabel('t (s)')
ylabel('A(mV)')

subplot(3,2,4)
plot(t,ys1fi+ys2fi,'Color','cyan','linewidth',3);
hold on
plot(t,ys3fi,'Color',[0.4940, 0.1840, 0.5560],'linewidth',3);
plot(t,ys1fi+ys2fi+ys3fi,'Color',[0.25, 0.25, 0.25],'linewidth',3);
title('signal 60+120,240,60+120+240 Hz after filter')
xlabel('t (s)')
ylabel('A(mV)')

subplot(3,2,6)
plot(t,ys4fi,'Color',[0.25, 0.25, 0.25],'linewidth',3);
title('result signal after filter')
xlabel('t (s)')
ylabel('A(mV)')


t=0:0.0001:0.0001*300;
ys1fii=250*sin(2*pi*60*t-0.77);
ys2fii=250*sin(2*pi*120*t-1.476);
ys3fii=250*sin(2*pi*240*t-3.97);
ys4fii=ys1fii+ys2fii+ys3fii+500;



figure
subplot(3,2,1)
plot(t,ys1fii,'Color',[0, 0.4470, 0.7410],'linewidth',3);
title('signal 60 Hz after filter phi=-0.77 rads')
xlabel('t (s)')
ylabel('A(mV)')

subplot(3,2,3)
plot(t,ys2fii,'Color',[0.4660, 0.6740, 0.1880],'linewidth',3);
title('signal 120 Hz after filter phi=-1.476 rads')
xlabel('t (s)')
ylabel('A(mV)')

subplot(3,2,5)
plot(t,ys3fii,'Color',[0.4940, 0.1840, 0.5560],'linewidth',3);
title('signal 240 Hz after filter phi=-3.97 rads')
xlabel('t (s)')
ylabel('A(mV)')

subplot(3,2,2)
plot(t,ys1fii,'Color',[0, 0.4470, 0.7410],'linewidth',3);
hold on
plot(t,ys2fii,'Color',[0.4660, 0.6740, 0.1880],'linewidth',3);
plot(t,ys1fii+ys2fii,'Color','cyan','linewidth',3);
title('signal 60,120,60+120 Hz after filter')
xlabel('t (s)')
ylabel('A(mV)')

subplot(3,2,4)
plot(t,ys1fii+ys2fii,'Color','cyan','linewidth',3);
hold on
plot(t,ys3fii,'Color',[0.4940, 0.1840, 0.5560],'linewidth',3);
plot(t,ys1fii+ys2fii+ys3fii,'Color',[0.25, 0.25, 0.25],'linewidth',3);
title('signal 60+120,240,60+120+240 Hz after filter')
xlabel('t (s)')
ylabel('A(mV)')

subplot(3,2,6)
plot(t,ys1fii+ys2fii+ys3fii,'Color',[0.25, 0.25, 0.25],'linewidth',3);
title('result signal after filter')
xlabel('t (s)')
ylabel('A(mV)')











