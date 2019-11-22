#include <SoftwareSerial.h>

SoftwareSerial serial1(10, 11); // RX, TX
String strAux = "1";
String textoRecebido = "Ola! Novo Aqui? \n Me informe o percentual de umidade: ";
String strAux2 = "";
unsigned long delay1 = 0;

void setup() {

  serial1.begin(9600);
  Serial.begin(9600);

}
String lero = "";
char *auxLero;
int teste;
void loop() {
  char caracter = "";
  
  //serial1.print("Informe a Umidade (0 - 100%):");   
//  if (Serial.available()) {
//     caracter = Serial.read();
//     textoRecebido += caracter;
//     delay1 = millis();
//  }
  // MSG inicial
  if (((millis() - delay1) > 10) && (strAux != "")) {
     serial1.print(textoRecebido);   
     strAux = "";
  }
  // END MSG inicial

  
  while(serial1.available()){
    caracter = serial1.read();
    Serial.println(caracter);
    lero += String(caracter);
  }

  if (caracter != "") {  
     caracter = "";
  }
  if(lero != ""){
    Serial.println("aaaa " + lero);
    auxLero = lero.c_str();
    teste = atoi(auxLero);

  if(lero == "Felipe"){
      Serial.println("Amem senhor!! String");
      
    }
    
    Serial.println(teste);
    Serial.println(teste, DEC);
    if(teste == 50){
      Serial.println("Amem senhor!!");
      
    }
    lero = "";
  }
  
}
