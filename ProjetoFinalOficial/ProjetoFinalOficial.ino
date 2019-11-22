#include <EEPROM.h>
#include <SoftwareSerial.h>
#include "DHT.h"


#define BOMBA       2   //PINO QUE ATIVA A BOMBA
#define SENSOR      3  //PINO DE ALIMENTAÇÃO DO SENSOR
#define PIN_SENSOR  A0

#define DHTPIN       A1       // PORTA ANALÓGICA QUE CONECTA O SENSOR DHT11
#define DHTTYPE      DHT11    // DHT 11

// Variáveis sensor DHT11
float h = 0;  // Umidade
float t = 0;  // Temperatura

int leitura = 0;
float porcetConv = 0;

float converterLeitura(int x){
  float teste = (float) 730*x/100;
  Serial.println(teste);
  return teste;
}

float converterPorcent(int y){
  float valor = (float) 100*y/730;
  return valor;
}

#define espacoEEPROM 1000   //1Kb para todos os Arduinos com ATMega328P

byte   variavelByte = 0;     // Controle de acesso
int    variavelInt = 0;      // Porcentagem de Umidade
String variavelString = "";   // Armazenar o nome da planta


SoftwareSerial serial1(10, 11); // RX, TX
DHT dht(DHTPIN, DHTTYPE);

String textInicial0 = "\n\nInicie com OK.";
String textoInicial = "Ola! Bem Vindo! \nMe informe qual a planta ira germinar: \n";
String textoInicial2 = "Agora me informe a porcentagem de umidade mínima para o solo da planta (0 - 100): \n";
unsigned long delay1 = 0;

void escreverB(String texto){
  if (((millis() - delay1) > 10)) {
     serial1.print(texto);
  }
}

String lerB(){
  String textoRecebido = "";
  char caracter = "";
  while(serial1.available()){
    caracter = serial1.read();
    textoRecebido += String(caracter);
  }
  return textoRecebido;
}

void EEPROMWriteInt(int address, int value); //2 Bytes
int  EEPROMReadInt(int address);

void EEPROMWriteLong(int address, long value); //4 Bytes
long EEPROMReadLong(int address);

void EEPROMWriteStr(int address, String value); //n Bytes
void EEPROMConcatStr(int address, String value);
String EEPROMReadStr(int address);

void setup() {
  Serial.begin(9600);
  serial1.begin(9600);
  dht.begin();      // Init DHT11


  pinMode (BOMBA, OUTPUT);  
  pinMode (SENSOR, OUTPUT); 

// Limpeza da EEPROM (Executado apenas para limpar a memoria. IMPORTANTE: Apos a primeira carga, comentar este trecho e efetuar a segunda carga em seguida.)
 // for (int nL = 0; nL < espacoEEPROM; nL++) { 
 //     EEPROM.write(nL, 0);
 // }  //Comentar até aqui.

  variavelByte = EEPROM.read(0);        //Posicao 0 (zero) da EEPROM
  variavelInt  = EEPROMReadInt(1);      //Posicao 1 e 2 da EEPROM
  variavelString = EEPROMReadStr(100);  //A Partir da Posicao 100 da EEPROM
}
String auxPor = "";
bool estado = false;
bool estadoAnterior;


void loop() {
  while(EEPROM.read(0) == 0){ // Verifica se já houve conexão
    escreverB(textInicial0);
    String str = "";
    while(str == ""){
      str = lerB();
    }
    str.toUpperCase();
    if(str == "OK"){
        escreverB(textoInicial);
        while(variavelString == ""){
          Serial.println("Esperando TIPO:");
          variavelString = lerB();
        }
        Serial.println("Tipo: " + variavelString);
        escreverB(textoInicial2);
        auxPor = "";
        while(auxPor == ""){
          Serial.println("Esperando PORC:");
          auxPor = lerB();
        }
        Serial.println("Porcent: " + auxPor);
        int variavelInt = atoi((auxPor.c_str()));
            if((variavelInt >= 0) && (variavelInt <= 100)){
              EEPROM.write(0, 1);
              EEPROMWriteInt(1, variavelInt);
              EEPROMWriteStr(100, variavelString);
              variavelByte = EEPROM.read(0);         //Posicao 0 (zero) da EEPROM
              variavelInt  = EEPROMReadInt(1);       //Posicao 1 e 2 da EEPROM
              variavelString = EEPROMReadStr(100);   //A Partir da Posicao 100 da EEPROM
              porcetConv = converterLeitura(variavelInt);
            }
    }
    delay(2000);
  }
  variavelByte = EEPROM.read(0);        //Posicao 0 (zero) da EEPROM
  variavelInt  = EEPROMReadInt(1);      //Posicao 1 e 2 da EEPROM
  variavelString = EEPROMReadStr(100);  //A Partir da Posicao 100 da EEPROM


  /*Leitura sensor Temperatura e Umidade.*/
//  float auxH = dht.readHumidity();
//  float auxT = dht.readTemperature();
//  if (isnan(auxT) || isnan(auxH)){
//    Serial.println("Failed");
//  } else{  
//    Serial.println("T: " + String(auxT));
//    Serial.println("U: " + String(auxH));
//    t = auxT;
//    h = auxH;
//  }
/*End Leitura sensor Temperatura e Umidade.*/

  digitalWrite(SENSOR, HIGH); // Accende SENSOR
  leitura = analogRead(PIN_SENSOR);
  digitalWrite(SENSOR, LOW); // Spegne SENSOR
  leitura = 500;
  Serial.print("Valor Umidade do solo: " + String(leitura));

  if(leitura < porcetConv){
    estado = true;      // Informa que o solo está Seco
  }else{
    estado = false;     // Informa que o solo está Umido
  }

    if(estado && !estadoAnterior){
      delay(5000); // Evita que entre no loop se estiver na transição de umido para seco
      while(estado){
        digitalWrite(BOMBA, HIGH);
        delay(500);
        digitalWrite(BOMBA, LOW);
        delay(10000);
        digitalWrite(SENSOR, HIGH); // Accende SENSOR
        leitura = analogRead(PIN_SENSOR);
        digitalWrite(SENSOR, LOW); // Spegne SENSOR
        if(leitura >= porcetConv){
          estado = false;      // Informa que o solo está Seco
        }
    }
  }
  estadoAnterior = estado;
  
  float valorEmPorcent = converterPorcent(leitura);
  String textoFinal = "\n -----------------";
  escreverB(textoFinal);  
  textoFinal = "\nPlanta: " + variavelString;
  escreverB(textoFinal);
  Serial.println(textoFinal);
  textoFinal = "\nUmidade Min. Solo: " + String(variavelInt) + "% " + "Atual: " + String(valorEmPorcent) + "%";
  escreverB(textoFinal);
  Serial.println(textoFinal);
  textoFinal = "\nTemperatura:  X - Umidade do AR: Y";
  escreverB(textoFinal);
  Serial.println(textoFinal);
  textoFinal = "\n'Reset' para replantar :D";
  escreverB(textoFinal);
  Serial.println(textoFinal);
  
  String acao = lerB();
  acao.toUpperCase();
  if(acao == "RESET"){
    EEPROM.write(0, 0);
    variavelByte = EEPROM.read(0);
    variavelString = "";
    EEPROMWriteStr(100, variavelString);
  }
  delay(5000);
}



/*##################################################################################
 ###################################################################################
 ########################## FUNÇÕES PARA CONTROLAR EEPROM ##########################
 ###################################################################################
 ###################################################################################*/

void EEPROMWriteInt(int address, int value) {
   byte hiByte = highByte(value);
   byte loByte = lowByte(value);

   EEPROM.write(address, hiByte);
   EEPROM.write(address + 1, loByte);   
}

int EEPROMReadInt(int address) {
   byte hiByte = EEPROM.read(address);
   byte loByte = EEPROM.read(address + 1);
   
   return word(hiByte, loByte); 
}

void EEPROMWriteLong(int address, long value) {
   byte four = (value & 0xFF);
   byte three = ((value >> 8) & 0xFF);
   byte two = ((value >> 16) & 0xFF);
   byte one = ((value >> 24) & 0xFF);

   EEPROM.write(address, four);
   EEPROM.write(address + 1, three);
   EEPROM.write(address + 2, two);
   EEPROM.write(address + 3, one);
}

long EEPROMReadLong(int address) {
   long four = EEPROM.read(address);
   long three = EEPROM.read(address + 1);
   long two = EEPROM.read(address + 2);
   long one = EEPROM.read(address + 3);

   return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}

void EEPROMWriteStr(int address, String value) {
int nL;

   for (nL=0; nL < value.length(); nL++)  {
      EEPROM.write(address + nL, value.charAt(nL) );

      if (nL >= 1000) {
         break;
      }
   }

   if (nL < 1000) {
      EEPROM.write(address + nL, (char)0 );
   }
}

void EEPROMConcatStr(int address, String value) {
int nL;
char readByte;
bool nullFound = false;
int strPos = 0;

   for (nL=0; nL < 1000; nL++)  {

      if (!nullFound) {
         readByte = EEPROM.read(address + nL);

         if (readByte == (char)0) {
            nullFound = true;
         }
      } 
      
      if (nullFound) {
         EEPROM.write(address + nL, value.charAt(strPos) );          
         strPos++;

         if ((strPos + 1) > value.length()) {
            EEPROM.write(address + nL + 1, (char)0 );
            break;
         }
      }
   }     
}

String EEPROMReadStr(int address) {
String readStr = "";
char readByte;
int readAddress = address;

   do {
      readByte = EEPROM.read(readAddress);
      readStr += readByte;
      readAddress++;
   } while ( (readByte != (char)0) && (readAddress < (address + 1000)) );
   
   return readStr; 
}
