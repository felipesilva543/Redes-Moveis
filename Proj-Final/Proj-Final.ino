
#define BOMBA       2   //PINO QUE ATIVA A BOMBA

void setup()
{
  Serial.begin(9600);
  pinMode (BOMBA, OUTPUT);  
  pinMode (13, OUTPUT); 
}

void loop()
{
    digitalWrite(13, HIGH); //... attiva la BOMBA - lOGICA INVERTITA
    delay(2000);
    digitalWrite(13, LOW); // Spegne SENSOR
    delay(2000);
}
