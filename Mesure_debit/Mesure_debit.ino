#include <LiquidCrystal.h>

const unsigned int BAUD_RATE = 9600;
const int POTPIN = 0;
int debit = 0;
float volume = 0;
String message;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  Serial.begin(BAUD_RATE);
  lcd.begin(16, 2);
  lcd.print("TPE Mesure Debit");
}

void loop() {
  debit = analogRead(POTPIN);
  volume += (float)debit / float(10000);
  message = String(debit, DEC) + "ml/s " + String(volume, 2) + "L";
  while (message.length() < 16) {
    message += " ";
  }
  lcd.setCursor(0, 1);
  lcd.print(message);
  Serial.println(message);
  delay(100);
}
