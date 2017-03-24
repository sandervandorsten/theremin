#include <TimerOne.h>
#include <Volume3.h>
#include "pitches.h"

        // INSTELLINGEN //
const bool sensor2isvolume = true; // true = volume, false = bpm

const int maxDistance = 60;        // max afstand voor de afstandsensor, in Centimeter
const int defaultVolume = 500;
const int defaultDelay = 200;
     
// c major blues toonladder

const int aantal_tonen = 15;
int scale[aantal_tonen] = {
  NOTE_C3, NOTE_DS3, NOTE_F3, NOTE_G3, NOTE_AS3,
  NOTE_C4, NOTE_DS4, NOTE_F4, NOTE_G4, NOTE_AS4,
  NOTE_C5, NOTE_DS5, NOTE_F5, NOTE_G5, NOTE_AS5
  };
        // --------- //

        // CONSTANTE PINNEN //
const int soundGround = 8;
const int speakerpin = 9;
const int afstandGround = 10;
const int Echo = 11;
const int Trigger = 12;
const int Anode = 13;
        // --------- //

        //  GLOBALE VARIABELEN  //
float DISTANCE;
int VOLUME;
int DELAY;
        // --------- //

//------------------------------ CODE SCHRIJVEN ------------------------------ //
void setTheVolume() {     
// -------- code hieronder invoeren

// -------- einde code 
  int laagste_waarde = 0;
  int hoogste_waarde = 1023;
  VOLUME = map(input, laagste_waarde, hoogste_waarde, 1023, 0);
}

void setDelay() {
  // functie die de pauze tussen 2 tonen berekend voor de theremin
  // wordt alleen geactiveerd als de variabele 'sensor2isvolume' op false staat
  // Dit betekent dat de 2e sensor niet de volume, maar de delay (vertraging) bediend
  // de waarde van de input kan net als bij de volume varieren tussen de 0 en de 1023.
  // 0     = 500 miliseconde pauze  (bij veel licht)
  // 1023   = 50 miliseconde pauze  (bij weinig licht)
  int input = analogRead(A0);

  int laagste_waarde = 0;
  int hoogste_waarde = 1023;
  DELAY = map(input, laagste_waarde, hoogste_waarde, 500, 50);
}
// ------------------------------------------------

        //   CODE    //

void setup() {
  Serial.begin(9600);
  pinMode(Anode, OUTPUT);
  digitalWrite(Anode, HIGH);
  pinMode(Trigger, OUTPUT);
  digitalWrite(Trigger, LOW);
  pinMode(Echo, INPUT);
  
  pinMode(soundGround,OUTPUT);
  digitalWrite(soundGround,LOW);
  pinMode(afstandGround,OUTPUT);
  digitalWrite(afstandGround,LOW);

  VOLUME = defaultVolume;
  DELAY = defaultDelay;
}

void loop() {
  DISTANCE = readUltrasoundDistance();
  if (sensor2isvolume == true) {
    setTheVolume();
  } else {
    setDelay();
  }
  playSound();
}

void testprint(int laagste_waarde){
  Serial.println("uitgelezen waarde  :" + String(laagste_waarde));
}

void printValues() {
//  Serial.println("distance :" + String(DISTANCE));
  Serial.println("Volume   :" + String(VOLUME));
//  Serial.println("Delay    :" + String(DELAY));
  Serial.println("-----------------------------");
}

void playSound() {
  if (DISTANCE < maxDistance) {
    int tonechoice = map(DISTANCE, 0, maxDistance, 0, (aantal_tonen - 1));
    vol.tone(speakerpin, scale[tonechoice], VOLUME);
  }
  delay(DELAY);
}

float readUltrasoundDistance() {
  // deze functie berekend de afstand tussen de afstandsensor en jouw hand
  // met behulp van onhoorbaar geluid bepaald deze de afstand. 
  // de afstand berekenen doet hij in 50 microseconden. 
  digitalWrite(Trigger, HIGH);
  delayMicroseconds (50);
  digitalWrite(Trigger, LOW);
  
  return pulseIn(Echo, HIGH) / 58.0;
}
