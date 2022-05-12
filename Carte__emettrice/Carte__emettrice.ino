// Emetteur 
int donneeEnvoyeeX = 15;
int donneeEnvoyeeY = 15;

#include <SoftwareSerial.h>
SoftwareSerial SerialVirtuel1(2,3);
#define SERIAL1 SerialVirtuel1 

void setup() {
  SERIAL1.begin(9600);
  Serial.begin(9600);
  Serial.println("Emetteur");
  //pinMode(A0, INPUT);
 // pinMode(A1, INPUT);
  
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
  static uint32_t lastTime1;
  if(millis()>lastTime1+50){
    lastTime1 = millis();
    //== Lecture des valeurs analogiques
   /* donneeEnvoyeeX = analogRead(A0)/6;
    donneeEnvoyeeY = analogRead(A1)/6;

    //== Ajout d'un offset
    donneeEnvoyeeX=donneeEnvoyeeX+1000;
    donneeEnvoyeeY=donneeEnvoyeeY+2000;
*/  
    //== Envoi vers l'autre carte
    static int lastX =0;
    static int lastY =0;
    if(donneeEnvoyeeX!=lastX){
      SERIAL1.println(donneeEnvoyeeX);
      lastX=donneeEnvoyeeX;
    }
    if(donneeEnvoyeeY!=lastY){
      SERIAL1.println(donneeEnvoyeeY);
      lastY=donneeEnvoyeeY;
    }

    //== Affichage dans le moniteur série
    Serial.print("ENV: ");
    Serial.print(donneeEnvoyeeX);
    Serial.print("\t");
    Serial.println(donneeEnvoyeeY);
    donneeEnvoyeeX=donneeEnvoyeeX+1;
    donneeEnvoyeeY=donneeEnvoyeeY+1;
  }// Fin limiteur de fréquence 
}
