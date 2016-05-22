//Programa : Multímetro
//Autor : Brenno Mello

//Biblioteca ADS
#include <Wire.h>
#include <Adafruit_ADS1015.h>

//Biblioteca para comunicação Serial
#include <SoftwareSerial.h>

//Variável da comunicação serial HC-05
//Define os pinos para a serial  (RX, TX)
SoftwareSerial Bluetooth(6,7);

//Biblioteca do A/D 1115
Adafruit_ADS1115 Ads1115;

//Operações enviadas pelo APP
char funcionalidade = 'A';

//Fator utilizado para conversão da tensão /
float fatorConversao=0.00762;

//Fator de conversão para Corrente
float fatorConversaoCorrente = 0.0001875;

float fatorConversaoCorrenteAlternada = 0.0001875;

float tensaoReferenciaACS712;

float valorLidoACS712;

float valorLidoADTensaoAlternada;

float valorLidoAlternadaACS712;

int numero = 33;


void setup() {
   
   Serial.begin(9600);//9600
   Ads1115.begin();
   Bluetooth.begin(9600); 
   
   //Ads1115.setGain(GAIN_ONE);
   Ads1115.setGain(GAIN_TWOTHIRDS);
   
   for(int i=0;i<15;i++){ 
     tensaoReferenciaACS712 += Ads1115.readADC_SingleEnded(2);
   }
   tensaoReferenciaACS712 /= 15; 
   
   //talves ajustar pra default 
   //fatorConversaoCorrente = 0.0001250;
         
   
  // tensaoReferenciaACS712 = Ads1115.readADC_SingleEnded(2);
}

void loop() {

  float tensao;
  float corrente;
  float resistencia;
  float correnteAlternada;
  float tensaoAlternada=0;
  
  /*
  if(Serial.available()>0){
    funcionalidade = Serial.read();
  }
  */

  if(Bluetooth.available()>0){
    funcionalidade = Bluetooth.read();
  }
  
  switch (funcionalidade){
    case 'T':
         
         //Se realmente funciona
         Ads1115.setGain(GAIN_TWOTHIRDS);
         
         tensao = Ads1115.readADC_Differential_0_1(); 
         //Serial.print("Valor inteiro = ");
         //Serial.print(tensao);
         
         //Se realmente funciona
         tensao *= fatorConversao;
         

            /*
            while(){
              
            }*/
          
          if(tensao>202.87 || tensao< -202.87){
              Ads1115.setGain(GAIN_TWOTHIRDS);
              fatorConversao=0.00762;
          }else{
            if(tensao>101.43 || tensao< -101.43){
              Ads1115.setGain(GAIN_ONE);
              fatorConversao=0.00619;
            }else{
              if(tensao>50.71 || tensao< -50.71){
                Ads1115.setGain(GAIN_TWO);
                fatorConversao=0.00309;
              }else{
                if(tensao>25.35 || tensao< -25.35){
                  Ads1115.setGain(GAIN_FOUR);
                  fatorConversao=0.00154;
                }else{
                  if(tensao>12.68 || tensao< -12.68){
                    Ads1115.setGain(GAIN_EIGHT);
                    fatorConversao=0.00077;
                  }else{
                    Ads1115.setGain(GAIN_SIXTEEN);
                    fatorConversao=0.00038;
                  }
                }
              }
            }
          }

        for(int i=0; i<20; i++){
          tensao += Ads1115.readADC_Differential_0_1(); 
        }
        tensao /= 20;
        tensao *= fatorConversao;

        
          //Bluetooth.write(tensao);
          //Bluetooth.println(tensao);
          //Serial.print("\tFator de conversao = ");
          //Serial.print(fatorConversao,5);
          //Serial.print("\tResultado = ");      
          Serial.println(tensao,4);
          
          //Serial.write(numero);

          

            
          
          //talvez no final  
          delay(1000);
          
      break;
    case 'C':

        //Ads1115.setGain(GAIN_TWOTHIRDS);
        //tempo de espera para leitura
        
        Serial.print("\tValor de referencia = ");
        Serial.print(tensaoReferenciaACS712);
        valorLidoACS712=0;
        
        for(int i=0; i<15; i++)
          valorLidoACS712 += Ads1115.readADC_SingleEnded(2);
        
        valorLidoACS712/=15;
        Serial.print("\tValor inteiro = ");
        Serial.print(valorLidoACS712);

        valorLidoACS712 = valorLidoACS712 - tensaoReferenciaACS712;
                
        Serial.print("\tValor inteiro subtracao = ");
        Serial.print(valorLidoACS712);
         
        
        //Se realmente funciona
        corrente = valorLidoACS712*fatorConversaoCorrente;

        //ajustarEscalaAdACS712(corrente);

        
        //Se realmente funciona
        //Ads1115.setGain(GAIN_TWOTHIRDS);

        
        
        /*
        while(){
          
        }*/

                
          //repetir leitura da tensão 
          
          corrente = corrente/0.066; 

        
          Bluetooth.write(corrente);
          Serial.print("\tFator de conversao = ");
          Serial.print(fatorConversaoCorrente,7);
          Serial.print("\tResultado = ");      
          Serial.println(corrente, 4);
          
          //talvez no final
          delay(1000);
          
      break;
    case 'R':

      break;
    case 'A':
        //fatorConversaoCorrente para default
        Ads1115.setGain(GAIN_TWOTHIRDS);
        /*
        valorLidoAlternadaACS712=0;
        
        Serial.print("\tLeitura Tensao = ");
        Serial.print(Ads1115.readADC_SingleEnded(2));

        Serial.print("\tSubtracao Leitura Tensao - Referencia = ");
        Serial.print(tensaoReferenciaACS712,3);
        */
        
        for(int i=0;i<200;i++){
          valorLidoAlternadaACS712 += pow (Ads1115.readADC_SingleEnded(2) - tensaoReferenciaACS712, 2);
          //Serial.println(Ads1115.readADC_SingleEnded(2)-tensaoReferenciaACS712);
          //Serial.println((Ads1115.readADC_SingleEnded(2)-tensaoReferenciaACS712)*0.0048828125);
          //delay(75);
        }
          valorLidoAlternadaACS712 = sqrt(valorLidoAlternadaACS712/200)*fatorConversao;

          //ajustarEscalaAdACS712(valorLidoAlternadaACS712);

          //repetir leitura de tensão
          
          correnteAlternada = valorLidoAlternadaACS712/0.066;

          
          Bluetooth.write(correnteAlternada);
          Serial.print("\tFator de conversao = ");
          Serial.print(fatorConversaoCorrente,7);
          Serial.print("\tValor lido Raiz = ");
          Serial.print(valorLidoAlternadaACS712,7);
          Serial.print("\tResultado = ");      
          Serial.println(correnteAlternada, 4);
          

          //delay(5);
      break;
      case 'V':
        //fatorConversao para default
        Ads1115.setGain(GAIN_TWOTHIRDS);
        valorLidoADTensaoAlternada=0;
        
        for(int i=0; i <100;i++){
          valorLidoADTensaoAlternada += pow (Ads1115.readADC_Differential_0_1(),2);
        }
         tensaoAlternada = sqrt(valorLidoADTensaoAlternada/100)*fatorConversao;
          
        //ajustarEscalaAdTensao(valorLidoADTensaoAlternada);

        //fazer a leitura da tensão novamente

          Bluetooth.write(tensaoAlternada);
          Serial.print("\tFator de conversao = ");
          Serial.print(fatorConversao,7);
          Serial.print("\tValor lido Raiz = ");
          Serial.print(valorLidoADTensaoAlternada,7);
          Serial.print("\tResultado = ");      
          Serial.println(tensaoAlternada, 4);

          delay(10);
          
        break;
        
  }
  
}

//verificar uso
void SerialPrintFloat(float f){
    byte* a = (byte*)&f;
    
    Serial.write(*a++);
    Serial.write(*a++);
    Serial.write(*a++);
    Serial.write(*a);
    
}

void serialFloatPrint(float f) {
  byte * b = (byte *) &f;
  Serial.print("f:");
  //Serial.write(b[0]);
  //Serial.write(b[1]);
  //Serial.write(b[2]);
  //Serial.write(b[3]);
  /* DEBUG */
  Serial.println();
  Serial.println(b[0], BIN);
  Serial.println(b[1], BIN);
  Serial.println(b[2], BIN);
  Serial.println(b[3], BIN);
  //*/
}

void ajustarEscalaAdACS712(float tensao){
      
      /*
        if(corrente>4.096 || corrente< -4.096){
          Ads1115.setGain(GAIN_TWOTHIRDS);
          fatorConversaoCorrente = 0.0001875;
        }else{
          if(corrente>2.048 || corrente< -2.048){
            Ads1115.setGain(GAIN_ONE);
            fatorConversaoCorrente = 0.0001250;
          }
        }*/
}

void ajustarEscalaAdTensao(float tensao){
  
}

/*
float getVPP()
{
  float result;
  
  int readValue;             //value read from the sensor
  int maxValue = 0;          // store max value here
  int minValue = 1024;          // store min value here
  
   uint32_t start_time = millis();
   while((millis()-start_time) < 1000) //sample for 1 Sec
   {
       readValue = analogRead(sensorIn);
       // see if you have a new maxValue
       if (readValue > maxValue) 
       {
           //record the maximum sensor value
           maxValue = readValue;
       }
       if (readValue < minValue) 
       {
           //record the maximum sensor value
           minValue = readValue;
       }
   }
   
   // Subtract min from max
   result = ((maxValue - minValue) * 5.0)/1024.0;
      
   return result;
 }*/
