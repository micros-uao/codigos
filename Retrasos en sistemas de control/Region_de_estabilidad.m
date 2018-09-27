close all; 
figure
title('Complex plane poles')
xlabel('Real')
ylabel('Imag')
 r=1;
 t=0:pi/24:2*pi;
 x=r*cos(t);
 y=r*sin(t);
 plot(x,y,x,y)
 grid on
 hold on

k=0.97287;
t=0.083;
ts=0.02;
m=1;

yd=tf(k*[1 -(exp(-ts/t)+exp(-m*ts/t)) exp(-m*ts/t)],[1 -exp(-ts/t) 0],1);


kp=4.266;

ki=51.39;

pid=tf([kp ki*ts-kp],[1 -1],1);

%lc=(yd*pid)/(1+yd*pid);
%step(lc);
rel=zeros(1,999);
yts=zeros(1,955);
ym=zeros(1,955);
i=1;
j=1;
%y=zeros(1,955);
f=false;

 for Ts=0.001:0.001:1-0.001
   
    for m=0:0.01:1
        
           
        rel(i)=m/Ts;
        
        ydt=tf(k*[1-exp(-m*Ts/t) (-exp(-Ts/t)+exp(-m*Ts/t))],[1 -exp(-Ts/t) 0],Ts);
        
        pidt=tf([kp ki*Ts-kp],[1 -1],Ts);
        
        lcd=(ydt*pidt)/(1+ydt*pidt);
        
        
        
        [p,z]=pzmap(lcd);
        plot(real(p),imag(p),'*');
        
        l=round(abs(p),3)>1;
        
        for j=1:1:length(l)
        
          if(l(j))
             yts(i)=Ts;         
             ym(i)=m;
          f=true;   
          break;
          end
        %j=j+1;
        end
       
      if(f)
      f=false;           
      break;
      end   
        
    end
%   step(lcd);   
   i=i+1; 
 end
 
figure
Ts=0.001:0.001:1-0.001;
plot(Ts,rel)
title('Región de estabilidad')
xlabel('Ts (s)')
ylabel('m/Ts')
 