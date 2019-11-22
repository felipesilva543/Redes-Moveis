int LEITURA = 0;   //PINO DE LEITURA DO SENSOR
#define BOMBA       2   //PINO QUE ATIVA A BOMBA
#define SENSOR      3  //PINO DE ALIMENTAÇÃO DO SENSOR
#define PIN_SENSOR  A0

void setup()
{
  Serial.begin(9600);
  pinMode (BOMBA, OUTPUT);  
  pinMode (SENSOR, OUTPUT); 
}

void loop()
{
  digitalWrite(SENSOR, HIGH); // Accende SENSOR
  LEITURA = analogRead(PIN_SENSOR);
  Serial.print("Valor Umidade':");
  Serial.println(LEITURA);
//  if (LEITURA > 200 ) //inizia la LEITURA: se il valore del SENSOR Ã¨ inferiore a "200", allora...
//  {
//    digitalWrite(BOMBA, LOW); //... attiva la BOMBA - lOGICA INVERTITA
//  }
//  else //..altrimenti...
//  {
//    digitalWrite(BOMBA, HIGH); //...disattiva la BOMBA  - lOGICA INVERTITA
//  }
  digitalWrite(SENSOR, LOW); // Spegne SENSOR
  delay(1000);
}
