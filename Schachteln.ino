#include <Servo.h>

Servo servoHuelle;  // Huelle auf das Foerderband bewegen
Servo servoSchSch;  // Schachtel in Huelle schieben + Schranke bedienen

int servoHuellePin = 9;
int servoSchSchPin = 10;

int vHuelleStop = 93;
int vSchSchStop = 92;

int vHuelle = 105;
int vSchSch = 45;

int pauseHuelle = 1000; // Einheit ms
int pauseSchSch = 1500; 
int pauseAb = 5000; 
int pause = 1000;

//=====================================================================

void setup() {
  servoHuelle.attach(servoHuellePin);
  servoSchSch.attach(servoSchSchPin);
  pinMode(A0, INPUT);
  Serial.begin(115200);
}

//=====================================================================

void loop() {

  // sicherstellen, dass Servos am Anfang nicht rotieren
  servoHuelle.write(vHuelleStop);
  servoSchSch.write(vSchSchStop);

  // Schranke schließen
  servoSchSch.write(vSchSch); // Schub fuer gewisse Zeit aktivieren und Schranke schließen
  delay(2*pauseSchSch); // eine ganze Umdrehung des Fließbands abwarten
  servoSchSch.write(vSchSchStop); // Fließband stoppen

  // Huelle einlegen
  servoHuelle.write(vHuelle); // Huelle einlegen/Backe ausfahren
  delay(pauseHuelle); // Zeit, die Servo braucht bis Huelle auf Endposition
  servoHuelle.write(vHuelleStop); // Huelle festhalten

  while(analogRead(A0) < 1010) // Sensorsignal ablesen
  {
    delay(pause); // Schachtel nicht vorhanden
  }

  // Schachtel vorhanden
  servoSchSch.write(vSchSch); // Schub fuer gewisse Zeit aktivieren und mit Schranke Gegendruck ausueben
  delay(pauseSchSch); // eine halbe Umdrehung des Fließbands abwarten
  servoSchSch.write(vSchSchStop); // Fließband stoppen

  delay(pause);
    
  servoSchSch.write(180-vSchSch); // Schub zurueckfahren und dabei Schranke oeffnen
  delay(pauseSchSch); // eine halbe Umdrehung des Fließbands
  servoSchSch.write(vSchSchStop); // Fließband stoppen

  delay(pause);

  servoHuelle.write(180-vHuelle); // Backe einfahren
  delay(pauseHuelle); // Zeit, die Backe fuer einen Weg braucht
  servoHuelle.write(vHuelleStop); // stoppen
  
  delay(pauseAb); // Zeit fuer Abtransport der Schachtel
}
