#include <SoftwareSerial.h>
SoftwareSerial SoftSerial(2, 3);  


String    bufferString=""; 
uint8_t   count=0;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
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
          if(bufferString.length()==14){break; }
         }
      }     
     Serial.println(bufferString);
      }
