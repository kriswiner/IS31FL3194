/* 12/23/2017 Copyright Tlera Corporation
 *  
 *  Created by Kris Winer
 *  
 This basic sketch is to operate the IS31FL3194 3-channel led driver
 http://www.issi.com/WW/pdf/IS31FL3194.pdf
 
 The sketch uses default SDA/SCL pins on the Ladybug development board 
 but should work with almost any Arduino-based board.

 Library may be used freely and without limit only with attribution.
 
  */
  
#include "IS31FL3194.h"

IS31FL3194 IS31FL3194;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(4000);
 
  Wire.begin(); // set master mode 
  Wire.setClock(400000); // I2C frequency at 400 kHz  
  delay(1000); 

  IS31FL3194.I2Cscan();

  Serial.println("IS31FL3194 RGB led driver...");
  byte c = IS31FL3194.getChipID();
  Serial.print("IS31FL3194 "); Serial.print("I AM "); Serial.print(c, HEX); Serial.print(" I should be "); Serial.println(0xCE, HEX);
  Serial.println(" ");
  delay(1000); 

  if(c == 0xCE ) // check if IS31FL3194 has acknowledged
  {
   Serial.println("IS31FL3194 is online..."); Serial.println(" ");

   IS31FL3194.reset();  // start by resetting device
   delay(100);
   
   IS31FL3194.init(); // initialize IS31FL3194
   
  }
  else 
  {
  if(c != 0xCE) Serial.println(" IS31FL3194 not functioning!");
  }

 
} /* end of setup */


void loop() 
{
  
   IS31FL3194.powerUp();
   
   // set led colr, possible choices are red, green, blue, yellow, cyan, magenta
   IS31FL3194.ledBlink(red, 10);
   delay(1000);
   IS31FL3194.ledBlink(green, 10);
   delay(1000);
   IS31FL3194.ledBlink(blue, 10);
   delay(1000);
   IS31FL3194.ledBlink(cyan, 10);
   delay(1000);
   IS31FL3194.ledBlink(magenta, 10);
   delay(1000);
   IS31FL3194.ledBlink(yellow, 10);
   delay(1000);

   IS31FL3194.powerDown();
   
   // set rgb led current
   IS31FL3194.ledBlink(red, 10);
   delay(1000);
   IS31FL3194.ledBlink(green, 10);
   delay(1000);
   IS31FL3194.ledBlink(blue, 10);
   delay(1000);
   IS31FL3194.ledBlink(cyan, 10);
   delay(1000);
   IS31FL3194.ledBlink(magenta, 10);
   delay(1000);
   IS31FL3194.ledBlink(yellow, 10);
   delay(1000);
  
}  /* end of main loop */
