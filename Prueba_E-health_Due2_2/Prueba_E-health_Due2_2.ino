
//Variables para contadores 
int cont=0;
int conta=0;
//Variables Fisiologicas
float SPO2=0;
float BPM=0;
float ECG=0;
float EMG=0;
float galv=0;
float temp=0;
float sp=0;
float pulse=0;
//Arreglo utilizado para leer el pulsioximetro
uint8_t digito[41]={0};
//Variables para indicar los estados del 7 Segmentos del pulsioximetro
uint8_t A = 0;
uint8_t B = 0;
uint8_t C = 0;
uint8_t D = 0;
uint8_t E = 0;
uint8_t F = 0;
uint8_t G = 0;

void setup() {
  //Se inicializan el puerto serial del DUE
  Serial.begin(115200);
  //Se elige la resolucion para la salida analogica
  analogWriteResolution(12);
  //Se elige la resolucion para la entrada analogica
  analogReadResolution(12);
  //Se inician los pines digitales para la lectura del pulsioximetro
  initPulsioximeter();
  
  pinMode(6, INPUT);
  //Se configura el pin 6 para recibir interrupciones del pulsioximetro y se relaciona al metodo readPulsioximeter
  attachInterrupt(6, readPulsioximeter, RISING);

}

//Se lee el valor analogico relacionado al sensor de flujo de aire
int getAirFlow(void)
  {
    int airFlow = analogRead(A1);
    return airFlow; 
  }

 void airFlowWave(int air)
  {
    for (int i=0; i < (air / 5) ; i ++) {  
        Serial.print("..");  
    }

    Serial.print("..");
    Serial.print("\n");
    delay(25);
  }

//Metodo para obtener el valor de la temperatura
float getTemperature(void)
  { 
    //Variables locales
    float Temperature; //Temperatura Corporal 
    float Resistance;  //Sensor resistivo.
    float ganancia=5.0;
    float Vcc=3.3;
    float RefTension=3.0; // Voltaje de referencia para el puente de wheastone.
    float Ra=4700.0; //Resistencia del puente Wheatstone.
    float Rc=4700.0; //Resistencia del puente Wheatstone.
    float Rb=769.0; //Resistencia del puente Wheatstone.
    int sensorValue = analogRead(A3);
    
    float voltage2=((float)sensorValue*Vcc)/4095; // Conversion de binario a voltaje

    // Salida de voltaje del puente de Wheatstone.
    voltage2=voltage2/ganancia;
    // Calculo del sensor resistivo usando el voltaje
    float aux=(voltage2/RefTension)+Rb/(Rb+Ra);
    Resistance=Rc*aux/(1-aux);

    //De acuerdo al rango del valor de la resistencia se halla la temperatura    
    if (Resistance >=1822.8) {
      // if temperature between 25ÂºC and 29.9ÂºC. R(tÂª)=6638.20457*(0.95768)^t
      Temperature=log(Resistance/6638.20457)/log(0.95768);  
    } else {
      if (Resistance >=1477.1){
          // if temperature between 30ÂºC and 34.9ÂºC. R(tÂª)=6403.49306*(0.95883)^t
          Temperature=log(Resistance/6403.49306)/log(0.95883);  
      } else {
        if (Resistance >=1204.8){
          // if temperature between 35ÂºC and 39.9ÂºC. R(tÂª)=6118.01620*(0.96008)^t
          Temperature=log(Resistance/6118.01620)/log(0.96008); 
        }
        else{
          if (Resistance >=988.1){
            // if temperature between 40ÂºC and 44.9ÂºC. R(tÂª)=5859.06368*(0.96112)^t
            Temperature=log(Resistance/5859.06368)/log(0.96112); 
          }
          else {
            if (Resistance >=811.7){
              // if temperature between 45ÂºC and 50ÂºC. R(tÂª)=5575.94572*(0.96218)^t
              Temperature=log(Resistance/5575.94572)/log(0.96218); 
            }
          }
        }
      }  
    }
   
    return Temperature;
  }


  //Metodo para leer la conductancia de la piel 
  float getSkinConductance(void)
  {
    // Variables locales.   
    float resistance;
    float conductance;
    
    // Se lee el voltaje analogico del pin 2
    float sensorValue = analogRead(A2);
    float voltage = (float)sensorValue*(3.3/4095);
    //Se calcula la conductancia
    conductance = 2*((voltage - 0.5) / 100000);

    // Conductance calculation
    resistance = 1 / conductance;
    //Se multiplica por una ganancia debido a que el valor es muy bajo 
    conductance = conductance * 1000000;
   
    if (conductance > 1)  return conductance;
    else return -1.0;
  }



  float getSkinResistance(void)
  { 
    // Local variable declaration.   
    float resistance;
    float conductance;
  
    // Read an analogic value from analogic2 pin.
    float sensorValue = analogRead(A2);
    float voltage = (sensorValue * 3.3) / 4095; 
  
    //delay(2);
    conductance = 2*((voltage - 0.497) / 100000);
   
    //Conductance calcultacion
    resistance = 1 / conductance;
    //delay(2);
  
    if (resistance > 1.0 ) return resistance;
    else return -1.0;
  }

  


  float getSkinConductanceVoltage(void)
  {
    //delay(2);
  
    //Read analogic value from analogic2 pin.
    int sensorValue = analogRead(A2);
  
    //Convert the readed value to voltage.
    float voltage = ( sensorValue * 3.3 ) / 4095;

    //delay(2);
    return voltage;
  }

//Metodo para declarar los pines del pulsioximetro
void initPulsioximeter(void){
  // Configuring digital pins like INPUTS
    pinMode(13, INPUT);   pinMode(12, INPUT);
    pinMode(11, INPUT);   pinMode(10, INPUT);
    pinMode( 9, INPUT);   pinMode( 8, INPUT);
    pinMode( 7, INPUT);   pinMode( 6, INPUT);
   
  
  }

void leerPulsioximeter(void){
 
  cont ++;
  
  if (cont == 50) { //Get only of one 50 measures to reduce the latency
    //readPulsioximeter();  
  //Se reinicia el contador 
  cont = 0;
  //Se envia el dato
  Serial.print("PRbpm : "); 
  Serial.print(getBPM());

  Serial.print("    %SPo2 : ");
  Serial.print(getOxygenSaturation());

  Serial.print("\n");  
  Serial.println("============================="); 
    
  }
  }  

int getOxygenSaturation(void)
  {
    return SPO2;
  }


int getBPM(void)
  {
    return BPM;
  }
  
//Metodo para leer el pulsioximetro
void readPulsioximeter(void){
    
    // Condicion para enviar los datos del pulsioximetro
    if(conta>50){
    //digito[41]={0};
    
    for (int i = 0; i<41 ; i++) { // Se leen los estados de los leds del dispositivo
      A = !digitalRead(13);
      B = !digitalRead(12);
      C = !digitalRead(11);
      D = !digitalRead(10);
      E = !digitalRead(9);
      F = !digitalRead(8);
      G = !digitalRead(7);

      //Convierte los estados de los leds al numero que estan mostrando
      digito[i] = segToNumber(A, B, C ,D ,E, F,G);  
     //delay minimo para que el sistema pueda leer los leds
      delayMicroseconds(300); //300 microseconds 
      //if (digito[i]=9) Serial.println(i);     
      
    }

   //Se extrae la informacion del arreglo leido y se convierte a un numero  
   SPO2=10 * digito[27] + digito[22]+digito[9];//Concentracion de oxigeno
   BPM=100 * digito[19] + 10 * digito[2] + digito[0];//Pulso
   //Se reinicia el contador
   conta=0;  
    }
  
   }

/*******************************************************************************************************/

  //! Convierte de 7 segmentos a numero

  uint8_t segToNumber(uint8_t A, uint8_t B, uint8_t C, uint8_t D, uint8_t E, uint8_t F, uint8_t G )
  {
    if ((A == 1) && (B == 1) && (C == 1) && (D == 0) && (E == 1) && (F == 1) && (G == 1)) {
      return 0;
     
    } else if ((A == 0) && (B == 1) && (C == 0) && (D == 0) && (E == 1) && (F == 0) && (G == 0)) {  
      return 1;
    
    } else if ((A == 1) && (B == 1) && (C == 0) && (D == 1) && (E == 0) && (F == 1) && (G == 1)) { 
      return 2;
    
    } else if ((A == 1) && (B == 1) && (C == 0) && (D == 1) && (E == 1) && (F == 0) && (G == 1)) { 
      return 3;
    
    } else if ((A == 0) && (B == 1) && (C == 1) && (D == 1) && (E == 1) && (F == 0) && (G == 0)) { 
      return 4;
    
    } else if ((A == 1) && (B == 0) && (C == 1) && (D == 1) && (E == 1) && (F == 0) && (G == 1)) { 
      return 5;
    
    } else if ((A == 1) && (B == 0) && (C == 1) && (D == 1) && (E == 1) && (F == 1) && (G == 1)) { 
      return 6;
    
    } else if ((A == 1) && (B == 1) && (C == 0) && (D == 0) && (E == 1) && (F == 0) && (G == 0)) {
      return 7;  
    
    } else if ((A == 1) && (B == 1) && (C == 1) && (D == 1) && (E == 1) && (F == 1) && (G == 1)) { 
      return 8;
    
    } else if ((A == 1) && (B == 1) && (C == 1) && (D == 1) && (E == 1) && (F == 0) && (G == 1)) { 
      return 9;
      
    } else  {
      return 0;
    }
  }



void loop() {
          //Contador para que se envie el dato del pulsioximetro despues de 50 interrupciones
          conta=conta+1;
          //Se almacena el dato ECG
          ECG = getECG()-1.5;
          //Se almacena el dato EMG
          EMG= getEMG();
          //Se almacena el dato Galvanico 
          galv=getSkinConductance();
          //Se almacena el dato de temperatura
          temp=getTemperature()/50.0;
          //Envio de datos, se especifica la cantidad de bytes por dato y una letra asociada a cada dato
          Serial.print('a');
          Serial.print(temp,3);

          Serial.print('b');
          Serial.print(SPO2/100,3);
          
          Serial.print('c');
          Serial.print(BPM/200,3);

          //Los datos que tienen valores negativos se envian con una letra mayuscula primero
          if(ECG<0){
          Serial.print('D');
          Serial.print(abs(ECG),3);
          }else{
          Serial.print('d');
          Serial.print(ECG,3);
          }

          
          Serial.print('e');
          Serial.print(EMG,3);

          if(galv<0){
          Serial.print('F');
          Serial.print(abs(galv),3);
          }else{
          Serial.print('f');
          Serial.print(galv/10,3);
          }
          

          //retraso para volver a enviar datos
          delay(50);

  
} 

//Metodo para leer el voltaje analogico del EMG
 float  getEMG(void)
  {
    float analog0;
    // Read from analogic in. 
    analog0=analogRead(A8);
    // binary to voltage conversion
    return analog0 = analog0 * (3.3 / 4095.0); 
  }

//Metodo para leer el voltaje analogico del ECG
float getECG(void)
  {
    float analog0;
    // Read from analogic in. 
    analog0=analogRead(A0);
    // binary to voltage conversion
    return analog0 = analog0 * 3.3 / 4095.0;   
    //return analog0;
  }

