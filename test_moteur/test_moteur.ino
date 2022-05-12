//  Demo function:The application method to drive the DC motor.
//  Author:Loovee (luweicong@seeed.cc)
//  2016-3-11

#include "MotorDriver.h"

static unsigned int moteur = MOTORB;
MotorDriver motor;

void setup()
{
    // initialize
    motor.begin();
    Serial.begin(9600);  // Initialiser le port série
}

void loop()
{
    Serial.println("En avant toute");
    motor.speed(moteur, 100);            // set motor0 to speed 100
    delay(1000);
    Serial.println("Pied sur le frein");
    motor.brake(moteur);                 // brake
    delay(1000);
    Serial.println("Marche Arriere");
    motor.speed(moteur, -100);           // set motor0 to speed -100
    delay(1000);
    Serial.println("Pied sur le frein");
    motor.brake(moteur);
    delay(1000);
    Serial.println("Stop");
    motor.stop(moteur);                  // stop
    delay(1000);
}
// END FILE
