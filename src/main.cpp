#include <Arduino.h>
#include <EEPROM.h>
#define RelayPin 7

int weldTime;
unsigned long lastBeat = 0;

void writeIntIntoEEPROM(int address, int number)
{ 
  EEPROM.write(address, number >> 8);
  EEPROM.write(address + 1, number & 0xFF);
}

int readIntFromEEPROM(int address)
{
  return (EEPROM.read(address) << 8) + EEPROM.read(address + 1);
}

void setup() {
  
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(A7, INPUT);
  pinMode(RelayPin, OUTPUT);
  digitalWrite(RelayPin, HIGH);  

  Serial.begin(115200);

  weldTime = readIntFromEEPROM(0);

  delay(100);
  Serial.println("Start");
}

void weld() {
  lastBeat = millis();
  Serial.println("Weld");
  

  digitalWrite(LED_BUILTIN, HIGH);  
  digitalWrite(RelayPin, LOW);  
  delay(weldTime);
  digitalWrite(RelayPin, HIGH);  
  digitalWrite(LED_BUILTIN, LOW);
  
  int buttonValue = analogRead(A7);

  if (buttonValue <300) {    
    delay(200);
  }
}
void printWeldTime(){
  Serial.print("Weld time: ");
  Serial.println(weldTime);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  int buttonValue = analogRead(A7);

  if (buttonValue <300) {
    weld();
    delay(500);
  }

  if (buttonValue >300 && buttonValue <500) {
    Serial.println("Set Weld time");
    weldTime = analogRead(A0);
    printWeldTime();   
    writeIntIntoEEPROM(0, weldTime);

    delay(1000);
  }
  
  if (millis() % 1000 == 0 ) {
    
    printWeldTime();    
    delay(10);

  }

  //Serial.println("Test");       
  //delay(500);
}


