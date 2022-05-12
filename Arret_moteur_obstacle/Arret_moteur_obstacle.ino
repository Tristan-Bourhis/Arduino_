#include "MotorDriver.h"

const int SENSORPIN = 0;  // Broche analogique utilisée pour connecter le capteur sharp
const int DELAY = 10;
const int NB_ECH_FILTRE = 10;
int buffer_filtre[NB_ECH_FILTRE];

static unsigned int moteur = MOTORB;
MotorDriver motor;

// Determination de la plage de valeurs du capteur:
//int valmin = 65534;
//int valmax = 0;

void setup()
{
  int i;
  for (i = 0; i < NB_ECH_FILTRE; i = i + 1)
  {
    // On initialise le buffer du filtre en mettant tout à zero
    buffer_filtre[i] = 0;
  }
  Serial.begin(9600);  // Initialiser le port série
  motor.begin();       // Initialiser le driver de la carte MotorShield
}

int filtre(int valeur)
{
  int i;
  unsigned int somme = 0;
  for (i = 0; i < NB_ECH_FILTRE - 1; i = i + 1)
  {
    // On décale à gauche
    buffer_filtre[i] = buffer_filtre[i + 1];
    somme += buffer_filtre[i];
  } 
  buffer_filtre[i] = valeur; // En sortie de boucle, i est égal à NB_ECH_FILTRE - 1
  somme += valeur;
  return int(somme / NB_ECH_FILTRE);
}

void loop()
{
  unsigned int val = 0;  // Variable pour stocker les valeurs du capteur (initiallement zero)
  unsigned int val_filtre = 0;
  unsigned int vitesse = 0;
  val = analogRead(SENSORPIN);  // Lire la valeur du capteur sharp et stockage de la valeur dans val

  // Determination de la plage de valeurs du capteur:
  //if (val < valmin) {valmin = val;}
  //if (val > valmax) {valmax = val;}
  //Serial.println(valmin);  // on a mesuré 0
  //Serial.println(val);
  //Serial.println(valmax);  // On a mesuré 624

  val_filtre = filtre(val);  // Filtrage du bruit de val
  //Serial.println(val_filtre);  // Envoi la valeur vers le port série
  vitesse = 0;

  if (val_filtre > 100) {
    vitesse = 0;
  } else {
    if (val_filtre < 50) {vitesse = 100;}
    else {vitesse = 90;}
  }

  
  if (vitesse){
    motor.speed(moteur, vitesse);
  } else {
    motor.brake(moteur);
  }
  
  delay(DELAY);  // Attente avant la saise de la valeur suivante
}

