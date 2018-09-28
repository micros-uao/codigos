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
  // Se inicializa la entrada a para los datos del encoder    
      pinMode(a, INPUT);
  // Se crea una interrupcion externa por caida
      attachInterrupt( a, ServicioBoton, FALLING);
       //pinMode(a, INPUT);
  // Se inicializa la entrada b para los datos del encoder     
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

// Se pone el cursor en la columna0 y la fila 1 del lcd  
  lcd.setCursor(0, 1);
//Se imprime por LCD el numero de revoluciones por minuto  
  lcd.print(rev);
//Se leen las entradas digitales para el inicio y la parada
  buttonState = digitalRead(pushButtonStart);
  buttonState1 = digitalRead(pushButtonStop);

//De acuerdo a la bandera f se determina en que sentido gira el motor
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


// Se utiliza un if con el que se enclava el estado del boton de inicio cuando este es oprimido una vez
  if(buttonState or clave==1){

// Se posiciona el cursor en en la columna 0 y fila 0
  lcd.setCursor(0, 0);
// Se imprime por LCD el mensaje Start para indicar que arranco el sistema  
  lcd.print("Start");
  
 //Se enclava el estado de inicio    
  clave=1;
  clave1=0;  
// Se convierte el valor de PID a un valor entre 0 y 255 para el PWM  
 outmotor=abs(pid)*(255)/(100); 
// Segun el sentido de giro del motor se envia la señal de PWM por el pin 6 o el pin 5     
   if(pid>0){
    
    analogWrite(6, outmotor);
    digitalWrite(5, LOW);
    };
   if(pid<0){
    
    analogWrite(5, outmotor);
    digitalWrite(6, LOW);
    };


  }

// Se utiliza un if con el que se enclava el estado del boton de parada cuando este es oprimido una vez
  if(buttonState1 or clave1==1){
    
// Se posiciona el cursor en en la columna 0 y fila 0
  lcd.setCursor(0, 0);
  lcd.print("Stop");
 
    
 //Se enclava el estado de parada    
  clave=0;
  clave1=1; 
// Se detiene el motor 
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  
  }
  

  delay(1);        // delay
}
//Funcion de la interrupcion externa
void ServicioBoton() 
   
   { 
    //Contador de pulsos del pin a
    contador++ ;
    //Con el estado del pin b se sabe el sentido de giro
    sentido= digitalRead(b);
    //Bandera f para determinar el sentido del giro
    f=1;
   }
//Funcion de la interrupcion cuando se desborda el temporizador
void myHandler(){
     //Se calcula el numero de revoluciones por minuto
     rev=sign*(contador/1600)*10*60;
     //Se resetea el contador   
     contador=0;
     //Se llama la funcion PID
     PID();
}

//Algoritmo de control PID
 void PID()
   {  
 //Se calculan el error actual y el integral. el derivativo es cero porque se esta usando una algoritmo PI
 ee = ref-rev;  // error actual
 ed = 0;// ee-last;   // error derivativo
 ei=  ee+acu;  // error integral

//Se calcula la señal de control
 pid = (kp*ee) + (ki*ei) + (kd*ed);  // P,I,D terminos
//Se satura el valor del PID entre 100 y -100
 if (pid>100 && pid>0){ pid=100;}
 if (pid<-100 && pid<0){ pid=-100;}
 //Se actualizan las variables del error pasado y el acumulador
 last=ee;
 acu=acu+ee;
 //velocidad(num);

 //Serial.println(pos); 

   
   }
   



