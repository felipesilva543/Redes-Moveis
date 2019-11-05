/*
  Attivazione_pompa.ino
  Il programma attiva un rele' a cui puÃ² essere collegata una pompa
  oppure un'elettrovalvola per irrigare per esempio dei vasi quando siete in vacanza.
  L'attivazione viene fatta in base alla lettura dell'umiditÃ  del terreno eseguita
  da un sensore dotato di due terminali, in base alla conducibilitÃ  del terreno,
  in valore analogico in uscita varia.  Impostando la soglia il relÃ¨ viene eccitato.

  Sono utilizzati i seguenti pin
  Pin +5V           -> Alimentazione rele'
  Pin GND           -> GND sensore e relÃ¨
  Pin Analogico 0   -> Uscita sensore
  Pin Digitale 2    -> Rele'
  Pin Digitale 3    -> Sensore

  Creato il 22/1/2017 da Adriano Gandolfo
  Sito web http://www.adrirobot.it
  Blog http://it.emcelettronica.com/author/adrirobot
  Pagina Facebook https://www.facebook.com/Adrirobot-318949048122955
  Istagram https://www.instagram.com/adrirobot/
  This example code is in the public domain.
*/

int lettura = 0;
int pompa = 2; //Pin a cui Ã¨ collegato i rele' per attivare al pompa
int sensore = 3; //Pin di alimentazione del sensore

void setup()
{
  Serial.begin(9600);
  pinMode (pompa, OUTPUT); // Imposta il pin 2 POMPA come uscita.
  pinMode (sensore, OUTPUT); //Imposta il pin 2 SENSORE come uscita.
}

void loop()
{
  digitalWrite(sensore, HIGH); // Accende sensore
  lettura = analogRead(A0);
  Serial.print("Valore umidita':");
  Serial.println(lettura);
  if (lettura < 200 ) //inizia la lettura: se il valore del sensore Ã¨ inferiore a "200", allora...
  {
    digitalWrite(pompa, LOW); //... attiva la pompa - lOGICA INVERTITA
  }
  else //..altrimenti...
  {
    digitalWrite(pompa, HIGH); //...disattiva la pompa  - lOGICA INVERTITA
  }
  digitalWrite(sensore, LOW); // Spegne sensore
  delay(1000);
}
