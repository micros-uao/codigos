#include <FreeRTOS_ARM.h>

double ref=1,kp=4.266,ki=51.39,kd=0;
//double ref=1.2,kp=10,ki=51.4,kd=0;
double pos=0,last=0,acu=0,pidl=0;
double ee,ed,ei;
double pid;
double sensorValue=0;
double ts=0.022;

float m=0.9;
int toggle=LOW;

void setup() {
  // put your setup code here, to run once:
analogWriteResolution(12); 
analogReadResolution(12);

//pinMode(A0,INPUT);
pinMode(DAC1,OUTPUT);
Serial.begin(115200);
//digitalWrite(13,LOW);
pinMode(13,OUTPUT);
xTaskCreate(salida, NULL,configMINIMAL_STACK_SIZE , NULL, 1, NULL);
vTaskStartScheduler(); 

}


static void salida(void*arg){

portTickType xLastWakeTime;
xLastWakeTime = xTaskGetTickCount();

//digitalWrite(DAC1,LOW);
//delay(3000);
   
  while (1) {                                                                                                                                                                                                                                                
   digitalWrite(13,HIGH);
   sensorValue=((3.3)*analogRead(A0)/4095)+0.17;
   
   sensorValue= ((sensorValue-0.5)*(3.3-0)/(2.75-0.5));
   Serial.println(sensorValue);
  
    ee = ref-sensorValue;  // error actual
  
 
    pid = (kp*ee) + (ki*last)*ts -kp*last + pidl ;  // P,I,D terminos 
      
    if (pid>2.75 ){ pid=2.75;}
    if (pid<0.55 ){ pid=0.55;}
    
    pidl=pid;
    last=ee;
    
  //  Serial.println(pid);
    pid=(4095-0)*(pid-0.55)/(2.75-0.55);
    analogWrite(DAC1,pid);

    //delay(1);
   digitalWrite(13,LOW); 
    
    vTaskDelayUntil(&xLastWakeTime,(ts*1000/portTICK_RATE_MS));
    
    
    
    
    
    
  //  toggle=!toggle;
  //  digitalWrite(13,toggle);
    delay(ts*1000*m);
  
    }
    
  }



void loop() {
  // put your main code here, to run repeatedly:

}
