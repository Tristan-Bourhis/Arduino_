#include <SPI.h>
#include <SD.h>

//==== Choisir carte
// RENOMMER LES BROCHES SI D1 !!!!!!!!
const int chipSelect = 10;     // Si UNO
File dataFile;

void setup() {
  Serial.begin(9600);

  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    delay(2000);
  }
dataFile = SD.open("datalog.txt");
  if (dataFile) {
    SD.remove("datalog.txt");
    }else {
    Serial.println("error opening datalog.txt for reading");
  }
  dataFile.close();
  Serial.println("card initialized.");
}

void loop() {
  String dataString = String(millis());
  dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.println(dataString);
    Serial.print("Ecriture : ");
    Serial.println(dataString);
    dataFile.close();
    }else {
    Serial.println("error opening datalog.txt");
  }
  if (millis() > 10000) {  
    dataFile = SD.open("datalog.txt", FILE_READ);
    if (dataFile) {
      Serial.print("Lecture : ");
      while(dataFile.available()) {
        Serial.write(dataFile.read());
      }
    }else {
      Serial.println("error opening datalog.txt for reading");
    }
  }
  dataFile.close();
  delay(2000);
}

    
