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
#include <Wire.h>

#define en_Pin   9

// Regsiter Map
// http://www.issi.com/WW/pdf/IS31FL3194.pdf
#define IS31FL3194_PRODUCT_ID           0x00  // should return 0xCE
#define IS31FL3194_OP_CONFIG            0x01
#define IS31FL3194_OUT_CONFIG           0x02
#define IS31FL3194_CURRENT_BAND         0x03
#define IS31FL3194_HOLD_FUNCTION        0x04

#define IS31FL3194_P1_STATE             0x0D
#define IS31FL3194_P2_STATE             0x0E
#define IS31FL3194_P3_STATE             0x0F

// Current Mode
#define IS31FL3194_OUT1                 0x10
#define IS31FL3194_OUT2                 0x21
#define IS31FL3194_OUT3                 0x32

//Pattern mode
// Colors 1, 2 and 3 of pattern 1
#define IS31FL3194_COL1_PATT1_R         0x10
#define IS31FL3194_COL1_PATT1_G         0x11
#define IS31FL3194_COL1_PATT1_B         0x12
#define IS31FL3194_COL2_PATT1_R         0x13
#define IS31FL3194_COL2_PATT1_G         0x14
#define IS31FL3194_COL2_PATT1_B         0x15
#define IS31FL3194_COL3_PATT1_R         0x16
#define IS31FL3194_COL3_PATT1_G         0x17
#define IS31FL3194_COL3_PATT1_B         0x18

// Colors 1, 2 and 3 of pattern 2
#define IS31FL3194_COL1_PATT2_R         0x20
#define IS31FL3194_COL1_PATT2_G         0x21
#define IS31FL3194_COL1_PATT2_B         0x22
#define IS31FL3194_COL2_PATT2_R         0x23
#define IS31FL3194_COL2_PATT2_G         0x24
#define IS31FL3194_COL2_PATT2_B         0x25
#define IS31FL3194_COL3_PATT2_R         0x26
#define IS31FL3194_COL3_PATT2_G         0x27
#define IS31FL3194_COL3_PATT2_B         0x28

// Colors 1, 2 and 3 of pattern 3
#define IS31FL3194_COL1_PATT3_R         0x30
#define IS31FL3194_COL1_PATT3_G         0x31
#define IS31FL3194_COL1_PATT3_B         0x32
#define IS31FL3194_COL2_PATT3_R         0x33
#define IS31FL3194_COL2_PATT3_G         0x34
#define IS31FL3194_COL2_PATT3_B         0x35
#define IS31FL3194_COL3_PATT3_R         0x36
#define IS31FL3194_COL3_PATT3_G         0x37
#define IS31FL3194_COL3_PATT3_B         0x38

#define IS31FL3194_P1_TS_T1_Time_SET    0x19
#define IS31FL3194_P1_T2_T3_Time_SET    0x1A
#define IS31FL3194_P1_TP_T4_Time_SET    0x1B
#define IS31FL3194_P2_TS_T1_Time_SET    0x29
#define IS31FL3194_P2_T2_T3_Time_SET    0x2A
#define IS31FL3194_P2_TP_T4_Time_SET    0x2B
#define IS31FL3194_P3_TS_T1_Time_SET    0x39
#define IS31FL3194_P3_T2_T3_Time_SET    0x3A
#define IS31FL3194_P3_TP_T4_Time_SET    0x3B

#define IS31FL3194_P1_COLOR_EN          0x1C
#define IS31FL3194_P2_COLOR_EN          0x2C
#define IS31FL3194_P3_COLOR_EN          0x3C

#define IS31FL3194_P1_COLOR_CYC_TIME    0x1D
#define IS31FL3194_P2_COLOR_CYC_TIME    0x2D
#define IS31FL3194_P3_COLOR_CYC_TIME    0x3D

#define IS31FL3194_P1_NXT               0x1E
#define IS31FL3194_P2_NXT               0x2E
#define IS31FL3194_P3_NXT               0x3E

#define IS31FL3194_P1_LOOP_TIMES        0x1F
#define IS31FL3194_P2_LOOP_TIMES        0x2F
#define IS31FL3194_P3_LOOP_TIMES        0x3F

#define IS31FL3194_COLOR_UPDATE         0x40

#define IS31FL3194_P1_UPDATE            0x41
#define IS31FL3194_P2_UPDATE            0x42
#define IS31FL3194_P3_UPDATE            0x43

#define IS31FL3194_RESET                0x4F

#define IS31FL3194_ADDRESS 0x53

#define Mode 1

// define times
#define t_0_03s 0x00
#define t_0_13s 0x01
#define t_0_26s 0x02
#define t_0_38s 0x03
#define t_0_51s 0x04
#define t_0_77s 0x05
#define t_1_04s 0x06
#define t_1_60s 0x07
#define t_2_10s 0x08
#define t_2_60s 0x09
#define t_3_10s 0x0A
#define t_4_20s 0x0B
#define t_5_20s 0x0C
#define t_6_20s 0x0D
#define t_7_30s 0x0E
#define t_8_30s 0x0F

// define pattern times
#define TS t_2_10s  // Start time
#define T1 t_2_10s  // Rise time
#define T2 t_1_04s  // Hold time
#define T3 t_2_10s  // Fall time
#define T4 t_2_10s  // Off time
#define TP t_2_10s  // Time between pulses

// define cycle times
#define endless 0x00
#define once 0x15
#define twice 0x2A
#define thrice 0x3F

// light intensity (fraction of current max)
#define Imax_frac  0x80 // Imax_frac/256 * Imax = current


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(4000);

  pinMode(en_Pin, OUTPUT);
  digitalWrite(en_Pin, HIGH); // enable IS31FL3194
  delay(100);
 
  Wire.begin(); // set master mode 
  Wire.setClock(400000); // I2C frequency at 400 kHz  
  delay(100); 

  I2Cscan();

  // Read the Chip ID register, this is a good test of communication
  Serial.println("IS31FL3194 RGB led driver...");
  byte c = readByte(IS31FL3194_ADDRESS, IS31FL3194_PRODUCT_ID); // Read PRODUCT_ID register for IS31FL3194
  Serial.print("IS31FL3194 "); Serial.print("I AM "); Serial.print(c, HEX); Serial.print(" I should be "); Serial.println(0xCE, HEX);
  Serial.println(" ");
  delay(1000); 

  if(c == 0xCE ) // check if IS31FL3194 has acknowledged
  {
   Serial.println("IS31FL3194 is online..."); Serial.println(" ");

   writeByte(IS31FL3194_ADDRESS, IS31FL3194_RESET, 0xC5);  // start by resetting device
   delay(100);
   
   // test of rgb led function
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_OP_CONFIG, 0x01);     // normal operation in current mode
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_OUT_CONFIG, 0x07);    // enable all three ouputs
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_CURRENT_BAND, 0x00);  // 10 mA max current
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_HOLD_FUNCTION, 0x00); // hold function disable

   // set rgb led current
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_OUT1, 0xFF); //maximum current
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_OUT2, 0x00);
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_OUT3, 0x00);
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_COLOR_UPDATE, 0xC5); // write to color update register for changes to take effect
   delay(1000);
   
   // set rgb led current
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_OUT1, 0x00); //maximum current
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_OUT2, 0xFF);
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_OUT3, 0x00);
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_COLOR_UPDATE, 0xC5); // write to color update register for changes to take effect
   delay(1000);
   
   // set rgb led current
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_OUT1, 0x00); //maximum current
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_OUT2, 0x00);
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_OUT3, 0xFF);
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_COLOR_UPDATE, 0xC5); // write to color update register for changes to take effect
   delay(1000);
   
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_OUT1, 0x00); // minimum current
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_OUT2, 0x00);
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_OUT3, 0x00);
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_COLOR_UPDATE, 0xC5); // write to color update register for changes to take effect
   delay(2000);
   
  }
  else 
  {
  if(c != 0xCE) Serial.println(" IS31FL3194 not functioning!");
  }

   // Setup Mode 1
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_OP_CONFIG, 0x71);     // normal operation in pattern mode
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_OUT_CONFIG, 0x07);    // enable all three ouputs
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_CURRENT_BAND, 0x00);  // 10 mA max current
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_HOLD_FUNCTION, 0x00); // hold function disable

   // Pattern 1 color
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_COL1_PATT1_R, Imax_frac);  // color 1
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_COL1_PATT1_G, Imax_frac);
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_COL1_PATT1_B, Imax_frac);
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_COL2_PATT1_R, Imax_frac);  // color 2
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_COL2_PATT1_G, Imax_frac);
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_COL2_PATT1_B, Imax_frac);
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_COL3_PATT1_R, Imax_frac);  // color 3
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_COL3_PATT1_G, Imax_frac);
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_COL3_PATT1_B, Imax_frac);

   // Pattern 2 color
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_COL1_PATT2_R, Imax_frac);  // color 1
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_COL1_PATT2_G, Imax_frac);
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_COL1_PATT2_B, Imax_frac);
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_COL2_PATT2_R, Imax_frac);  // color 2
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_COL2_PATT2_G, Imax_frac);
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_COL2_PATT2_B, Imax_frac);
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_COL3_PATT2_R, Imax_frac);  // color 3
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_COL3_PATT2_G, Imax_frac);
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_COL3_PATT2_B, Imax_frac);

    // Pattern 3 color
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_COL1_PATT3_R, Imax_frac);  // color 1
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_COL1_PATT3_G, Imax_frac);
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_COL1_PATT3_B, Imax_frac);
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_COL2_PATT3_R, Imax_frac);  // color 2
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_COL2_PATT3_G, Imax_frac);
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_COL2_PATT3_B, Imax_frac);
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_COL3_PATT3_R, Imax_frac);  // color 3
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_COL3_PATT3_G, Imax_frac);
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_COL3_PATT3_B, Imax_frac);

   // Pattern 1 timing
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_P1_TS_T1_Time_SET, T1 << 4 | TS); // Rise time (T1) and start time (TS)
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_P1_T2_T3_Time_SET, T3 << 4 | T2); // Hold time (T2) and fall time (T3)
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_P1_TP_T4_Time_SET, T4 << 4 | TP); // Off time (T4) and time between pulses (TP)
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_P1_COLOR_CYC_TIME, once);
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_P1_NXT, 0x00);
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_P1_LOOP_TIMES, 0x00);
   
   // Pattern 2 timing
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_P2_TS_T1_Time_SET, T1 << 4 | TS); // Rise time (T1) and start time (TS)
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_P2_T2_T3_Time_SET, T3 << 4 | T2); // Hold time (T2) and fall time (T3)
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_P2_TP_T4_Time_SET, T4 << 4 | TP); // Off time (T4) and time between pulses (TP)
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_P2_COLOR_CYC_TIME, once);
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_P2_NXT, 0x00);
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_P2_LOOP_TIMES, 0x00);

   // Pattern 2 timing
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_P3_TS_T1_Time_SET, T1 << 4 | TS); // Rise time (T1) and start time (TS)
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_P3_T2_T3_Time_SET, T3 << 4 | T2); // Hold time (T2) and fall time (T3)
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_P3_TP_T4_Time_SET, T4 << 4 | TP); // Off time (T4) and time between pulses (TP)
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_P3_COLOR_CYC_TIME, once);
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_P3_NXT, 0x00);
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_P3_LOOP_TIMES, 0x00);

   // Enable and update
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_P1_COLOR_EN, 0x01);
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_P2_COLOR_EN, 0x01);
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_P3_COLOR_EN, 0x01);
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_COLOR_UPDATE, 0xC5); // write to color update register for changes to take effect
  
} /* end of setup */


void loop() {
  
       writeByte(IS31FL3194_ADDRESS, IS31FL3194_P1_UPDATE, 0xC5);//update Pattern 1
       while(Mode==1)
       {
          if(readByte(IS31FL3194_ADDRESS, IS31FL3194_P1_STATE) == 0x91)//Pattern 1 Running at T1
          {
           break;
          }
       }
       
      writeByte(IS31FL3194_ADDRESS, IS31FL3194_P2_UPDATE, 0xC5);//update Pattern 2
      while(Mode==1)
      {
          if(readByte(IS31FL3194_ADDRESS, IS31FL3194_P2_STATE) == 0x91)//Pattern 2 Running at T1
          {
            break;
          }
      }
      
      writeByte(IS31FL3194_ADDRESS, IS31FL3194_P3_UPDATE, 0xC5);//update Pattern 3
      while(Mode==1)
      {
          if(readByte(IS31FL3194_ADDRESS, IS31FL3194_P3_STATE) == 0x91)//Pattern 3 Running at T1
          {
             break;
          }
      }
       
   while(Mode==1);
  
}  /* end of main loop */


// I2C scan function
void I2Cscan()
{
// scan for i2c devices
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for(address = 1; address < 127; address++ ) 
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmission to see if
    // a device did acknowledge to the address.
//    Wire.beginTransmission(address);
//    error = Wire.endTransmission();
      error = Wire.transfer(address, NULL, 0, NULL, 0);
      
    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error==4) 
    {
      Serial.print("Unknown error at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
    
}

// I2C read/write functions for the CaliPile sensor

  void writeByte(uint8_t address, uint8_t subAddress, uint8_t data)
{
  Wire.beginTransmission(address);  // Initialize the Tx buffer
  Wire.write(subAddress);           // Put slave register address in Tx buffer
  Wire.write(data);                 // Put data in Tx buffer
  Wire.endTransmission();           // Send the Tx buffer
}

  uint8_t readByte(uint8_t address, uint8_t subAddress)
{
  uint8_t data; // `data` will store the register data   
  Wire.beginTransmission(address);         // Initialize the Tx buffer
  Wire.write(subAddress);                  // Put slave register address in Tx buffer
  Wire.endTransmission(false);             // Send the Tx buffer, but send a restart to keep connection alive
  Wire.requestFrom(address, 1);            // Read one byte from slave register address 
  data = Wire.read();                      // Fill Rx buffer with result
  return data;                             // Return data read from slave register
}

  void readBytes(uint8_t address, uint8_t subAddress, uint8_t count, uint8_t * dest)
{  
  Wire.beginTransmission(address);   // Initialize the Tx buffer
  Wire.write(subAddress);            // Put slave register address in Tx buffer
  Wire.endTransmission(false);       // Send the Tx buffer, but send a restart to keep connection alive
  uint8_t i = 0;
  Wire.requestFrom(address, count);  // Read bytes from slave register address 
  while (Wire.available()) {dest[i++] = Wire.read(); } // Put read results in the Rx buffer
}
