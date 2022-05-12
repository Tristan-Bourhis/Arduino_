// Récepteur
#include <Servo.h>
Servo servo1; Servo servo2;
int ledReception = 8;

int donneeRecueX = -1; 
int donneeRecueY = -1; 

#include <SoftwareSerial.h>
SoftwareSerial SerialVirtuel1(2,3);
#define SERIAL1 SerialVirtuel1 


void setup() {
  SERIAL1.begin(9600);
  Serial.begin(9600);
  Serial.println("Recepteur");
  //servo1.attach(2);
 // servo2.attach(3);
  //pinMode(ledReception, OUTPUT);

  // Si utilisation de la radio à la place du câble croisé
  //*//===== Changement des paramètres internes du HC12 (C010=canal 10)
    int setPin = 7;
    pinMode(setPin, OUTPUT);    
    pinMode(setPin,OUTPUT);
    digitalWrite(setPin,LOW);
    SERIAL1.print(F("AT+DEFAULT"));delay(100);
    SERIAL1.print(F("AT+C010"));
    delay(100);
    while(SERIAL1.available()){Serial.print((char)SERIAL1.read());delay(2);}
    digitalWrite(setPin,HIGH);
  //*/

}

void loop() {
  //== Récupération de la donnée  
   if(SERIAL1.available()>1){
     // digitalWrite(ledReception, HIGH);
      int donneeRecue = SERIAL1.parseInt();
      //if(donneeRecue>=1000 && donneeRecue<2000 ){
        donneeRecueX = donneeRecue;
    //  }
    //  if(donneeRecue>=2000 && donneeRecue<3000 ){
        donneeRecueY = donneeRecue;
    //  }     
    
      //== Affichage dans le moniteur série
      Serial.print("RECU: ");
      Serial.print(donneeRecueX);
      Serial.print("\t");
      Serial.println(donneeRecueY);
    }  
    digitalWrite(ledReception, LOW);

  //== Pilotage des servos
   /* int consigneX = donneeRecueX-1000;
    int consigneY = donneeRecueY-2000;
    servo1.write(consigneX);
    servo2.write(consigneY);*/
}
