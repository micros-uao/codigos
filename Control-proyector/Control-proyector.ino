// include the library code:
#include <LiquidCrystal.h>
#include <DueTimer.h>
 
// Inicializar libreria LCD con sus pines
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

int pushButtonStart =22; // Boton de inicio
int pushButtonStop = 24; // Boton de parada

int clave=0;
int clave1=0;

int buttonState=0;  // Estado del boton de inicio
int buttonState1=0; // Estado del boton de parada
int sentido=0;

float contador = 0;  
//int n = contador ;

int a = 26;     //  Pin A para el bus de pulsos del encoder
int b = 28;     //  Pin B para el bus de pulsos del encoder
int f=0;        // Bandera para determinar el sentido de giro 
float rev=0;    // Numero de revoluciones  
int sign=1;     // Sentido de giro
float ref=90,kp=0.15,ki=0.1,kd=0; // Parametros del PID
float pos=0,last=0,acu=0;     // Variables auxiliares
float ee,ed,ei;  // Error actual, error derivativo , error integral
double pid;      // Señal de control
//int sensorValue=0;  
//float outsensor=0;
double outmotor=0; // Salida al motor


void setup() {
// Se inicializa el puerto serial
Serial.begin(115200);
// Se inicializa el numero de columnas y filas
lcd.begin(16, 2);
// Se imprime un mensaje por el LCD
lcd.print("Press");


  // Se inicializan las entradas digitales
      pinMode(pushButtonStart, INPUT);
      pinMode(pushButtonStop, INPUT);
      pinMode(23, INPUT);
      pinMode(25, INPUT);
      pinMode(27, INPUT);
      pinMode(29, INPUT);
  // Se inicializan las entradas para los datos del encoder    
      pinMode(a, INPUT);
  // Se crea una interrupcion externa por caida
      attachInterrupt( a, ServicioBoton, FALLING);
       //pinMode(a, INPUT);
      pinMode(b, INPUT);
  
  // Se crea una temporizador que cada 100 ms llame la funcion myHandler   
      Timer3.attachInterrupt(myHandler);
      // Timer3.setFrequency(10);
       Timer3.start(100000); // Calls every 100ms

  // Salidas digitales para el motor
       pinMode(6, OUTPUT);
       pinMode(5, OUTPUT);

  // Salidas digitales para el led de potencia
       pinMode(3, OUTPUT);
       pinMode(2, OUTPUT);

  // Se enciende el led de potencia
       digitalWrite(3, LOW);
       digitalWrite(2, HIGH);   
}
 
void loop() {

  lcd.setCursor(0, 1);
  lcd.print(rev);

  buttonState = digitalRead(pushButtonStart);
  buttonState1 = digitalRead(pushButtonStop);

   if(f==1){
     if(sentido==0){
    Serial.println('-');
    sign=-1;
    }else{
      Serial.println('+');
      sign=1;
      };
      f=0;
   };



  if(buttonState or clave==1){


  lcd.setCursor(0, 0);
  lcd.print("Start");
  
    
  clave=1;
  clave1=0;  
 // lcd.clear();


  
 outmotor=abs(pid)*(255)/(100); 
   if(pid>0){
    
    analogWrite(6, outmotor);
    digitalWrite(5, LOW);
    };
   if(pid<0){
    
    analogWrite(5, outmotor);
    digitalWrite(6, LOW);
    };


  }

  if(buttonState1 or clave1==1){
    
 
  lcd.setCursor(0, 0);
  lcd.print("Stop");
 
    
    
  clave=0;
  clave1=1; 
  //lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Stop");
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  
  }
  
  // print out the state of the button:
  Serial.print(buttonState);
  Serial.println(buttonState);
//  Serial.println(1);

  delay(1);        // delay in between reads for stability
}

void ServicioBoton() 
   
   {  contador++ ;
      sentido= digitalRead(b);
      f=1;
   }

void myHandler(){
     
     rev=sign*(contador/1600)*10*60;
     
     
     contador=0;

     PID();
}

 void PID()
   {  
 
 ee = ref-rev;  // error actual
 ed = 0;// ee-last;   // error derivativo
 ei=  ee+acu;  // error integral

 pid = (kp*ee) + (ki*ei) + (kd*ed);  // P,I,D terminos
 
 if (pid>100 && pid>0){ pid=100;}
 if (pid<-100 && pid<0){ pid=-100;}
 last=ee;
 acu=acu+ee;
 //velocidad(num);

 //Serial.println(pos); 

   
   }
   



