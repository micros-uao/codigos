#include <FreeRTOS_ARM.h>

double gp=0,gpl=0;
double sensorValue=0;
double ts=0.001;
double m=0;
double t=0.083;
double k=0.97287;
int toggle=LOW;


void setup() {
  // put your setup code here, to run once:
analogWriteResolution(12); 
analogReadResolution(12);
pinMode(13,OUTPUT);
pinMode(DAC1,OUTPUT);
Serial.begin(115200);
xTaskCreate(salida, NULL,configMINIMAL_STACK_SIZE , NULL, 1, NULL);
vTaskStartScheduler(); 
}


static void salida(void*arg){

double ea=exp(-ts/t),eb=exp(-m*ts/t),u=0,ul=0,ull=0;
portTickType xLastWakeTime;
xLastWakeTime = xTaskGetTickCount();

  while (1) {                                                                                                                                                                                                                                                

      digitalWrite(13,HIGH);  
  
      u=(3.3)*analogRead(A0)/4095;
      
      u= (u-0.56)*(3.3-0)/(2.75-0.55);
      
   //   Serial.println(u);

 //     gp= gpl*ea+k*ul*(1-eb)-ull*ea+ull*eb;
    gp=ul*k*(ts/t)+gpl*ea;
    if (gp>2.75 ){ gp=2.75;}
    if (gp<0.55 ){ gp=0.55;}
  
    gpl=gp;
 //   ull=ul;
    ul=u;
  
   Serial.println(gp);
   
    gp=(4095-0)*(gp-0.55)/(2.75-0.55);
    
     analogWrite(DAC1,gp);
    
    digitalWrite(13,LOW);  
    
    vTaskDelayUntil(&xLastWakeTime,(ts*1000/portTICK_RATE_MS));
  
//    toggle=!toggle;
//    digitalWrite(13,toggle);
   
    delay(ts*1000*m);
    }
    
  }



void loop() {
  // put your main code here, to run repeatedly:

}
