#include <Arduino.h>
#include <EEPROM.h>
#include <U8g2lib.h>
#include <Wire.h>

#define RelayPin A1
#define PotentionmeterPin A0
#define PedalPin A5

int weldTime;
int weldCounter;

unsigned long lastBeat = 0;

/*
  U8g2lib Example Overview:
    Frame Buffer Examples: clearBuffer/sendBuffer. Fast, but may not work with all Arduino boards because of RAM consumption
    Page Buffer Examples: firstPage/nextPage. Less RAM usage, should work with all Arduino boards.
    U8x8 Text Only Example: No RAM usage, direct communication with display controller. No graphics, 8x8 Text only.
    
  This is a page buffer example.    
*/

// Please UNCOMMENT one of the contructor lines below
// U8g2 Contructor List (Picture Loop Page Buffer)
// The complete list is available here: https://github.com/olikraus/u8g2/wiki/u8g2setupcpp
// Please update the pin numbers according to your setup. Use U8X8_PIN_NONE if the reset pin is not connected
//U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

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
  pinMode(PotentionmeterPin, INPUT);
  pinMode(PedalPin, INPUT_PULLUP);
  
  pinMode(RelayPin, OUTPUT);
  digitalWrite(RelayPin, HIGH);  

  Serial.begin(115200);

  weldTime = readIntFromEEPROM(0);
  weldCounter = readIntFromEEPROM(2);
  
  if (weldCounter < 0) weldCounter = 1;

  delay(100);
  Serial.println("Start");

  u8g2.begin(); 
}

void weld() {
  lastBeat = millis();
  Serial.println("Weld");
  
  digitalWrite(LED_BUILTIN, HIGH);  
  digitalWrite(RelayPin, LOW);  
  delay(weldTime);
  digitalWrite(RelayPin, HIGH);  
  digitalWrite(LED_BUILTIN, LOW);
  
  weldCounter++;
  
  writeIntIntoEEPROM(2, weldCounter);

  int buttonValue = analogRead(PedalPin);

  if (buttonValue <300) {    
    delay(200);
  }
}

void printWeldTime(){
  Serial.print("Weld time: ");
  Serial.print(weldTime);
  Serial.print(" ");
  Serial.println(weldCounter);
  Serial.println(analogRead(PedalPin));

  u8g2.firstPage();
  do {
    //u8g2.drawHLine(0,0,10);
    //u8g2.drawHLine(0,31,10);
    //u8g2.clearBuffer();
    //u8g2.clearDisplay();

    u8g2.setFont(u8g2_font_7x13_tf); //u8g2_font_ncenB10_tr u8g2_font_6x10_tf

    u8g2.drawStr(0,12,"Weld time:");
    u8g2.setCursor(90,12);        
    u8g2.print(weldTime);

    u8g2.drawStr(0,28,"Counter:");
    u8g2.setCursor(90,28);
    u8g2.print(weldCounter);

  } while ( u8g2.nextPage() );

}

void loop() {
  // put your main code here, to run repeatedly:
  
  int buttonValue = analogRead(PedalPin);

  if (buttonValue <300) {
    weld();
    printWeldTime();  

    buttonValue = analogRead(PedalPin);
    while (buttonValue<500) {
      delay(50);
      buttonValue = analogRead(PedalPin);      
    }
    delay(500);
  }

  
  //Serial.println("Set Weld time");
  weldTime = analogRead(PotentionmeterPin) / 2;
  //printWeldTime();   
  //writeIntIntoEEPROM(0, weldTime);

  
  
  if (millis() % 1000 == 0 ) {
    
    printWeldTime();    
    delay(10);

  }

  //Serial.println("Test");       
  //delay(500);
}


