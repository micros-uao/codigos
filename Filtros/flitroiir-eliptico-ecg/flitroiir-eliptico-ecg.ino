#include <FreeRTOS_ARM.h>
double a[7]={0.000164665140153674,-0.000639560586736878,0.00122936318781636,-0.00149075156970184,0.00122936318781636,-0.000639560586736878,0.000164665140153674};
double b[7]={1,-5.67117104703984,13.4990867996992,-17.2587125530599,12.4978910181202,-4.85975319091673,0.792679375882744};
float output = 0;
float output_x = 0;
float output_y = 0;
double x[7]={0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000};
double y[7]={0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000};
float sensorValue=0;

void setup() {

analogWriteResolution(12); 
analogReadResolution(12);
pinMode(DAC0,OUTPUT);
Serial.begin(115200);
xTaskCreate(salida, NULL,configMINIMAL_STACK_SIZE , NULL, 1, NULL);
vTaskStartScheduler(); 

}


//Tarea de filtrar la señal
static void salida(void*arg){

portTickType xLastWakeTime;
xLastWakeTime = xTaskGetTickCount();

  while (1) {
  
    sensorValue=analogRead(A0);
    
    x[0]=sensorValue;
      
    
    for (int k = 0; k < 7; k++){output_x=a[k]*x[k] + output_x;}

    for (int j = 1; j < 7; j++){output_y=b[j]*y[j] + output_y;}
      
  
    output = output_x - output_y;

     y[0]=output;
     //Serial.println(output); 
    analogWrite(DAC0,1.5*output);
   
    output_x = 0; output_y = 0;
    output=0; 

    for(int i=1;i<7;i++){    
      x[7-i]=x[6-i];
    }
    
    
     for(int i=1;i<7;i++){    
     y[7-i]=y[6-i];
    }
           
   


    vTaskDelayUntil(&xLastWakeTime,(1/portTICK_RATE_MS));
    
    }
    
  }
  
void loop() {}


