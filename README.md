# Arduino_

Depot de sketches Arduino orientes apprentissage/prototypage:

- I/O de base (LED, bouton, entree analogique)
- Actionneurs (servo, moteur DC)
- Capteurs (distance, debit)
- Communication serie (SoftwareSerial, radio HC-12)
- Stockage SD
- Camera serie + enregistrement sur SD
- Affichage LCD

## Sommaire

1. [Vue d'ensemble](#vue-densemble)
2. [Structure du depot](#structure-du-depot)
3. [Prerequis](#prerequis)
4. [Libraries utilisees](#libraries-utilisees)
5. [Procedure d'utilisation](#procedure-dutilisation)
6. [Catalogue des sketches](#catalogue-des-sketches)
7. [Problemes connus](#problemes-connus)
8. [Depannage](#depannage)
9. [Pistes d'amelioration](#pistes-damelioration)

## Vue d'ensemble

Chaque dossier contient un sketch autonome (`.ino`) cible sur un cas d'usage.
Le depot n'est pas une application unique, mais une collection de demonstrations.

## Structure du depot

```text
Arduino_/
|- Analog_input/            -> lecture analogique A0
|- LED/                     -> variation PWM LED
|- Button/                  -> lecture bouton + LED built-in
|- Servo_moteur/            -> balayage servo
|- test_moteur/             -> tests marche/frein moteur via MotorDriver
|- Arret_moteur_obstacle/   -> adaptation vitesse moteur selon capteur distance
|- Capteur_IR/              -> logique stationnement + capteur ultrason + lecture serial
|- Mesure_debit/            -> affichage debit/volume sur LCD
|- Ecran_lcd/               -> demo LCD 16x2
|- RFID/                    -> lecture trame RFID via SoftwareSerial
|- Carte__emettrice/        -> emission de donnees via SoftwareSerial/HC-12
|- Carte_recepteur/         -> reception de donnees via SoftwareSerial/HC-12
|- Carte_SD/                -> datalogging sur carte SD
|- SD_and_Cam/              -> capture camera serie + sauvegarde JPEG sur SD
```

## Prerequis

- Arduino IDE 1.8+ ou 2.x
- Carte compatible Arduino (UNO/Nano/Mega selon les broches utilisees)
- Cables, alimentation et modules materiels associes

Recommandation:

- Commencer avec une Arduino UNO (pins utilises dans les exemples compatibles UNO).

## Libraries utilisees

### Libraries Arduino standard (incluses)

- `Servo.h`
- `LiquidCrystal.h`
- `SoftwareSerial.h`
- `SPI.h`
- `SD.h`

### Libraries externes (a installer)

- `MotorDriver.h` (utilisee dans `test_moteur` et `Arret_moteur_obstacle`)
- `HCSR04.h` (utilisee dans `Capteur_IR`)

Sans ces librairies, certains sketches ne compilent pas.

## Procedure d'utilisation

1. Ouvrir un sketch (fichier `.ino`) dans l'IDE Arduino.
2. Selectionner la carte et le port serie (`Tools > Board`, `Tools > Port`).
3. Installer les librairies manquantes via Library Manager.
4. Verifier/adapter le cablage (broches dans le sketch).
5. Compiler puis televerser.
6. Ouvrir le Moniteur Serie si le sketch affiche des logs (`Serial.begin(9600)`).

Important:

- Plusieurs sketches utilisent les memes broches (2, 3, 9, 10, etc.).
- Ne pas brancher tous les modules en meme temps sans revoir le mapping des pins.

## Catalogue des sketches

### `Analog_input/Analog_input.ino`

- Role: lit `A0` et affiche la valeur analogique sur le port serie.
- Pins:
  - entree analogique `A0`
- Serial: `9600 bauds`

### `LED/LED.ino`

- Role: effet "fade in/fade out" sur une LED PWM.
- Pins:
  - sortie PWM `D9`

### `Button/Button.ino`

- Role: allume la LED integree quand le bouton sur `D2` est a `HIGH`.
- Pins:
  - bouton `D2`
  - LED interne `LED_BUILTIN`

### `Servo_moteur/Servo_moteur.ino`

- Role: balayage d'un servo de 0 a 180 degres puis retour.
- Libraries: `Servo`
- Pins:
  - signal servo `D9`

### `test_moteur/test_moteur.ino`

- Role: sequence moteur DC (avant, frein, arriere, frein, stop).
- Libraries: `MotorDriver`
- Materiel: shield/driver moteur compatible
- Serial: traces d'etat a `9600 bauds`

### `Arret_moteur_obstacle/Arret_moteur_obstacle.ino`

- Role: ajuste la vitesse du moteur selon une mesure analogique (capteur distance type Sharp).
- Libraries: `MotorDriver`
- Pins:
  - capteur analogique `A0`
- Logique:
  - filtrage moyenne glissante (`NB_ECH_FILTRE = 10`)
  - freinage si obstacle detecte

### `Capteur_IR/Capteur_IR.ino`

- Role: logique de detection de stationnement avec capteur ultrason + lecture d'une trame serie.
- Libraries: `HCSR04`, `SoftwareSerial`
- Pins:
  - ultrason trigger `D4`, echo `D5`
  - serial logiciel `D2` (RX), `D3` (TX)
- Serial:
  - `Serial` a `9600`
  - `SoftSerial` a `9600`

### `Mesure_debit/Mesure_debit.ino`

- Role: simule un debit a partir d'une entree analogique et affiche debit/volume sur LCD 16x2.
- Libraries: `LiquidCrystal`
- Pins LCD:
  - RS=12, E=11, D4=5, D5=4, D6=3, D7=2
- Pin capteur:
  - `A0`

### `Ecran_lcd/Ecran_lcd.ino`

- Role: demo LCD 16x2 (message + compteur de secondes).
- Libraries: `LiquidCrystal`
- Pins LCD:
  - RS=12, E=11, D4=5, D5=4, D6=3, D7=2

### `RFID/RFID.ino`

- Role: lecture d'une trame RFID via `SoftwareSerial`.
- Libraries: `SoftwareSerial`
- Pins:
  - serial logiciel `D2` (RX), `D3` (TX)

### `Carte__emettrice/Carte__emettrice.ino`

- Role: emetteur de donnees (X/Y) vers une autre carte via `SoftwareSerial` / module HC-12.
- Libraries: `SoftwareSerial`
- Pins:
  - serial logiciel `D2`/`D3`
  - `SET` HC-12 sur `D7` (configuration canal `C010`)
- Frequence d'envoi: ~50 ms

### `Carte_recepteur/Carte_recepteur.ino`

- Role: recepteur des donnees emises (X/Y), affichage sur moniteur serie.
- Libraries: `SoftwareSerial` (+ `Servo` present mais pilotage commente)
- Pins:
  - serial logiciel `D2`/`D3`
  - `SET` HC-12 sur `D7`

### `Carte_SD/Carte_SD.ino`

- Role: ecrit periodiquement `millis()` dans `datalog.txt`, puis relit le contenu.
- Libraries: `SPI`, `SD`
- Pins:
  - CS SD `D10` (UNO)

### `SD_and_Cam/SD_and_Cam.ino`

- Role: capture image d'une camera serie et sauvegarde en `picNN.jpg` sur SD.
- Libraries: `SPI`, `SD`
- Pins:
  - CS SD `D10`
- Notes:
  - protocole bas niveau camera implemente dans le sketch
  - format image configure en VGA (`PIC_FMT_VGA`)

## Problemes connus

1. `RFID/RFID.ino`
   - `setup()` n'initialise ni `Serial.begin(...)` ni `SoftSerial.begin(...)`.
2. `Capteur_IR/Capteur_IR.ino`
   - condition `while(stationnement =0)` utilise une affectation au lieu d'une comparaison.
3. `SD_and_Cam/SD_and_Cam.ino`
   - la logique de declenchement `if(millis() < 5000)` limite les captures au debut d'execution.
4. Divers fichiers contiennent des portions commentees (servos, lectures analogiques) a adapter selon le montage reel.

## Depannage

### Erreur "No such file or directory" pour `MotorDriver.h` ou `HCSR04.h`

- Installer la librairie manquante via Library Manager ou depuis le fournisseur du module.

### Echec d'initialisation SD

- Verifier:
  - format de la carte (FAT/FAT32)
  - cablage SPI correct
  - pin `CS` (`D10` sur UNO)

### Pas de donnees dans le Moniteur Serie

- Verifier le baud rate (`9600` dans la majorite des sketches).
- Verifier le port COM selectionne.
- Verifier que `Serial.begin(...)` est bien present dans le sketch charge.

### Moteur/servo ne reagit pas

- Verifier alimentation externe suffisante (ne pas alimenter un moteur directement depuis la pin Arduino).
- Verifier masse commune entre driver et carte.
- Verifier que la broche/pin definie dans le code correspond au cablage reel.

## Pistes d'amelioration

1. Ajouter un schema de cablage par sketch (images + pinout).
2. Uniformiser les conventions de nommage des dossiers/fichiers.
3. Corriger les bugs mineurs (comparaison/affectation, initialisations serie manquantes).
4. Factoriser les constantes (pins, baud rate) en tete de fichier avec documentation.
5. Ajouter un tableau de compatibilite cartes Arduino (UNO/Nano/Mega/ESP32).

