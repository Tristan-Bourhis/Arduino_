#include <HCSR04.h>  // Librairie capteur
#include <SoftwareSerial.h>  // Librairie RFID

String    bufferString=""; 
uint8_t   count=0;

int distance = 0; // Distance en centimétre renvoyé par le capteur
int compteur = 0;
const int delai = 1000;
int stationnement = 0; //0 personne n'est garé, 1 quelqu'un est garé
SoftwareSerial SoftSerial(2, 3);    // Broche 2 et 3 pour la caméra
UltraSonicDistanceSensor capteurUltrason(4, 5);   // trigger broche 4, echo broche 5

void setup() {
    Serial.begin(9600);
    SoftSerial.begin(9600); // initialisation RFID
}

void loop() {
  distance = capteurUltrason.measureDistanceCm();   // Récupère la distance en Cm
    Serial.print("Distance : ");
  Serial.println(distance);
  Serial.print("Compteur : ");
  Serial.println(compteur);
  if( distance <= 100) {
    compteur = compteur + delai/1000;
    if (compteur >= 30) {
      while(stationnement =0)
        if(SoftSerial.available()){
          bufferString="";
          count=0;
          while(SoftSerial.available()){  
            delay(5);
            char newChar=SoftSerial.read();
            if(count>2 && count<11){
              bufferString+=newChar;
            }     
          count++;
          if(bufferString.length()==14){ stationnement = 1; break; }
         }
      }     
     Serial.println(bufferString);
      }
  }else {
    compteur = 0;
    stationnement =0;
  }
  delay(delai);

}
