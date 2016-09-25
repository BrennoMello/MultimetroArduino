//Programa : Multímetro
//Autor : Brenno Mello

//Biblioteca ADS
#include <Wire.h>
//Biblioteca ADS
#include <Adafruit_ADS1015.h>

//Biblioteca para comunicação Serial
#include <SoftwareSerial.h>

//Variável da comunicação serial HC-05
//Define os pinos para a serial  (RX, TX)
SoftwareSerial Bluetooth(6,7);

//Biblioteca do A/D 1115
Adafruit_ADS1115 Ads1115;

//Operações enviadas pelo APP
char funcionalidade = 'T';

//Fator utilizado para conversão da tensão
float fatorConversaoTensao=0.0019974364;

//Fator de conversão para Corrente
float fatorConversaoCorrente = 0.066;

float tensaoReferenciaACS712;

float valorLidoACS712;

int16_t leituraTensao;
long int acumulador;


float leituraVout;
float leituraVCC;
float fatorConversaoTensaoResistencia;

//Porta ligada ao pino IN2 do modulo
int portaRele2 = 5;
boolean estadoRele2 = true;

int portaRele1 = 4;
boolean estadoRele1 = true;


void setup() {
   
   Serial.begin(9600);//9600
   Ads1115.begin();
   Bluetooth.begin(9600); 
   
  //Define pinos para o rele como saida
  pinMode(portaRele2, OUTPUT); 

  pinMode(portaRele1, OUTPUT); 
   

}

void loop() {

  float tensao;
  float corrente;
  float resistencia;
  

  
  if(Bluetooth.available()>0){
    funcionalidade = Bluetooth.read();
    while (Bluetooth.available()) {
      Bluetooth.read();
    }
  }
  
  Serial.print(funcionalidade);
  switch (funcionalidade){
    case 'T':
         if(estadoRele2){
           digitalWrite(portaRele2, HIGH); //Desliga rele 2
           estadoRele2 = false;          
         }  
         if(estadoRele1){
          digitalWrite(portaRele1, HIGH);
          estadoRele1 = false;
        }
          
              
         Ads1115.setGain(GAIN_TWOTHIRDS);
         fatorConversaoTensao=0.0020105899;
         
         leituraTensao = Ads1115.readADC_SingleEnded(0);
         
         Serial.print("\tTensao inteiro = ");
         Serial.print(leituraTensao,4);         
                         
         tensao = leituraTensao*fatorConversaoTensao;
         Serial.print("\tTensao anterior = ");
         Serial.print(tensao,4);

          
          if(tensao>43.92 || tensao< -43.92){
              Ads1115.setGain(GAIN_TWOTHIRDS);
              fatorConversaoTensao=0.0020105899;
          }else if(tensao>21.96 || tensao < -21.96){
              Ads1115.setGain(GAIN_ONE);
              fatorConversaoTensao=0.0013403729;
          }else if(tensao>10.98 || tensao< -10.98){
              Ads1115.setGain(GAIN_TWO);
              fatorConversaoTensao=0.000679;
          }else if(tensao>5.49 || tensao< -5.49){
              Ads1115.setGain(GAIN_FOUR);
              fatorConversaoTensao=0.000342;
          }else if(tensao>2.74 || tensao< -2.74){
              Ads1115.setGain(GAIN_EIGHT);
              fatorConversaoTensao=0.000175;
          }else{
              Ads1115.setGain(GAIN_SIXTEEN);
              fatorConversaoTensao=0.000088;
          }
                
              
            
          
        acumulador=0;
        for(int i=0; i<10; i++){
          leituraTensao = Ads1115.readADC_SingleEnded(0); 
          acumulador += leituraTensao;
                    
        }
        leituraTensao = acumulador/10;
           
        tensao = leituraTensao*fatorConversaoTensao;

        if(tensao>0){
          if(tensao<0.0100){ 
            tensao = 0.0000;
          }  
        }else if(tensao>-0.0100){   
                tensao = 0.0000;
        }
        
          Bluetooth.print('&');
          Bluetooth.print('T');
          Bluetooth.print('#');
          Bluetooth.print('*');
          Bluetooth.print(tensao,4);
          Bluetooth.print('%');
          Serial.print("\tFator de conversao = ");
          Serial.print(fatorConversaoTensao,9);
          Serial.print("\tResultado = ");      
          Serial.println(tensao,4);
                   
          
          //talvez no final  
          delay(1000);
          
      break;
    case 'C':
        if(!estadoRele2){
          digitalWrite(portaRele2, LOW);  //Liga rele 2  
          estadoRele2 = true;
        }
        if(estadoRele1){
          digitalWrite(portaRele1, HIGH);
          estadoRele1 = false;
        }
        
        delay(100);
        
        Ads1115.setGain(GAIN_TWOTHIRDS);
        tensaoReferenciaACS712 = 0;
        for(int i=0; i<10; i++){                
          tensaoReferenciaACS712 += Ads1115.readADC_SingleEnded(1);
          delay(1); 
        }
        tensaoReferenciaACS712 = tensaoReferenciaACS712/10;
        tensaoReferenciaACS712 = tensaoReferenciaACS712/2;
        tensaoReferenciaACS712 = tensaoReferenciaACS712*0.0001875057;
        
        Serial.print("\tValor de referencia = ");
        Serial.print(tensaoReferenciaACS712,4);
                
        Ads1115.setGain(GAIN_ONE);
        valorLidoACS712=0;
               
        
        for(int i=0; i<10; i++){                
          valorLidoACS712 += Ads1115.readADC_SingleEnded(2);
          
          delay(1); 
        }  
        valorLidoACS712 = valorLidoACS712/10;
        valorLidoACS712 = valorLidoACS712*0.0001250038;
                
        Serial.print("\tValor inteiro = ");
        Serial.print(valorLidoACS712,4);
                     
        valorLidoACS712 =  valorLidoACS712 - (tensaoReferenciaACS712 - 0.0047);

        Serial.print("\tValor inteiro subtracao = ");
        Serial.print(valorLidoACS712, 4);
        
        
        corrente = valorLidoACS712/0.066;
        

          Bluetooth.print('&');
          Bluetooth.print('C');
          Bluetooth.print('#');
          Bluetooth.print('*');        
          Bluetooth.print(corrente,4);
          Bluetooth.print('%');

          Serial.print("\tResultado = ");      
          Serial.println(corrente, 4);
          
          //talvez no final
          delay(1000);

          
      break;
    case 'R':
      if(estadoRele2){
        digitalWrite(portaRele2, HIGH); //Desliga rele 2
        estadoRele2 = false;
      }
      if(!estadoRele1){
        digitalWrite(portaRele1, LOW);
        estadoRele1 = true;
      }  


    Ads1115.setGain(GAIN_TWOTHIRDS);
    fatorConversaoTensaoResistencia = 0.0001875057;
    leituraVCC =  Ads1115.readADC_SingleEnded(1)*fatorConversaoTensaoResistencia;
    Serial.print("\tLeitura VCC = ");
    Serial.print(leituraVCC,7);
          
    leituraVout = Ads1115.readADC_SingleEnded(3)*fatorConversaoTensaoResistencia;  
    Serial.print("\tLeitura Vout = ");
    Serial.print(leituraVout,7);
          
    if(leituraVout>4.096 || leituraVout<-4.096){
      Ads1115.setGain(GAIN_TWOTHIRDS);
      fatorConversaoTensaoResistencia = 0.0001875057;
    }else if(leituraVout>2.048 || leituraVout<-2.048){
      Ads1115.setGain(GAIN_ONE);
      fatorConversaoTensaoResistencia = 0.000125042;
    }else if(leituraVout>1.024 || leituraVout<-1.024){
      Ads1115.setGain(GAIN_TWO);
      fatorConversaoTensaoResistencia = 0.0000625012;
    }else if(leituraVout>0.512 || leituraVout<-0.512){
      Ads1115.setGain(GAIN_FOUR);
      fatorConversaoTensaoResistencia = 0.0000312509;
    }else if(leituraVout>0.256 || leituraVout<-0.256){
      Ads1115.setGain(GAIN_EIGHT);
      fatorConversaoTensaoResistencia = 0.0000156254;
    }else {
      Ads1115.setGain(GAIN_SIXTEEN);
      fatorConversaoTensaoResistencia = 0.0000078127;
    }  
      
      
    for(int i=0;i<10;i++){
       leituraVout += Ads1115.readADC_SingleEnded(3);
       delay(1);
    }
       leituraVout = leituraVout/10; 
       leituraVout = leituraVout*fatorConversaoTensaoResistencia;
   
      
      //r1=993 
      resistencia = (leituraVout*993)/(leituraVCC-leituraVout);
      
      if(resistencia<10 || resistencia>10500)
        resistencia = 0.0000;
      
      //Zerar leitura fora da faixa
      Bluetooth.print('&');
      Bluetooth.print('R');
      Bluetooth.print('#');
      Bluetooth.print('*');
      Bluetooth.print(resistencia,4);
      Bluetooth.print('%');
      Serial.print("\tSaida divisor = ");
      Serial.print(leituraVout,7);
      Serial.print("\tResultado = ");      
      Serial.println(resistencia, 4);
      
      delay(1000);
     break;
        
  }

}




