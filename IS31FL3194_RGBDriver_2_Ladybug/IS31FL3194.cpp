#include "IS31FL3194.h"

  IS31FL3194::IS31FL3194(){
  }


  // Read the Chip ID register, this is a good test of communication
  uint8_t IS31FL3194::getChipID() 
  { 
  uint8_t c = readByte(IS31FL3194_ADDRESS, IS31FL3194_PRODUCT_ID); // Read PRODUCT_ID register for IS31FL3194
  return c;
  }


  void IS31FL3194::reset()
  {
  writeByte(IS31FL3194_ADDRESS, IS31FL3194_RESET, 0xC5);
  }


  void IS31FL3194::powerDown()
  {
  uint8_t d = readByte(IS31FL3194_ADDRESS, IS31FL3194_OP_CONFIG);
  writeByte(IS31FL3194_ADDRESS, IS31FL3194_OP_CONFIG, d & ~(0x01)); //clear bit 0 to shut down
  }


  void IS31FL3194::powerUp()
  {
  uint8_t d = readByte(IS31FL3194_ADDRESS, IS31FL3194_OP_CONFIG);
  writeByte(IS31FL3194_ADDRESS, IS31FL3194_OP_CONFIG, d | 0x01); //set bit 0 to enable
  }


  void IS31FL3194::init()// configure rgb led function
  {
  writeByte(IS31FL3194_ADDRESS, IS31FL3194_OP_CONFIG, 0x01);     // normal operation in current mode
  writeByte(IS31FL3194_ADDRESS, IS31FL3194_OUT_CONFIG, 0x07);    // enable all three ouputs
  writeByte(IS31FL3194_ADDRESS, IS31FL3194_CURRENT_BAND, 0x00);  // 10 mA max current
  writeByte(IS31FL3194_ADDRESS, IS31FL3194_HOLD_FUNCTION, 0x00); // hold function disable
  }


  void IS31FL3194::ledBlink(uint8_t color, uint32_t duration)
  {
   if(color == green) {
   _out1 = 0xFF;
   _out2 = 0x00;
   _out3 = 0x00;
   }

   if(color == blue) {
   _out1 = 0x00;
   _out2 = 0xFF;
   _out3 = 0x00;
  }

   if(color == red) {
   _out1 = 0x00;
   _out2 = 0x00;
   _out3 = 0xFF;
   }

   if(color == cyan) {
   _out1 = 0x20;
   _out2 = 0x20;
   _out3 = 0x00;
   }

   if(color == magenta) {
   _out1 = 0x00;
   _out2 = 0x20;
   _out3 = 0x20;
   }

   if(color == yellow) {
   _out1 = 0x20;
   _out2 = 0x00;
   _out3 = 0x20;
   }

   
   // set rgb led current
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_OUT1, _out1); //maximum current
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_OUT2, _out2);
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_OUT3, _out3);
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_COLOR_UPDATE, 0xC5); // write to color update register for changes to take effect
   delay(duration);
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_OUT1, 0x00); //maximum current
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_OUT2, 0x00);
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_OUT3, 0x00);
   writeByte(IS31FL3194_ADDRESS, IS31FL3194_COLOR_UPDATE, 0xC5); // write to color update register for changes to take effect
  }


  // I2C scan function
  void IS31FL3194::I2Cscan()
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
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
      
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


// I2C read/write functions  

  void IS31FL3194::writeByte(uint8_t address, uint8_t subAddress, uint8_t data)
{
  Wire.beginTransmission(address);  // Initialize the Tx buffer
  Wire.write(subAddress);           // Put slave register address in Tx buffer
  Wire.write(data);                 // Put data in Tx buffer
  Wire.endTransmission();           // Send the Tx buffer
}

  uint8_t IS31FL3194::readByte(uint8_t address, uint8_t subAddress)
{
  uint8_t data; // `data` will store the register data   
  Wire.beginTransmission(address);         // Initialize the Tx buffer
  Wire.write(subAddress);                  // Put slave register address in Tx buffer
  Wire.endTransmission(false);             // Send the Tx buffer, but send a restart to keep connection alive
  Wire.requestFrom(address, 1);            // Read one byte from slave register address 
  data = Wire.read();                      // Fill Rx buffer with result
  return data;                             // Return data read from slave register
}

  void IS31FL3194::readBytes(uint8_t address, uint8_t subAddress, uint8_t count, uint8_t * dest)
{  
  Wire.beginTransmission(address);   // Initialize the Tx buffer
  Wire.write(subAddress);            // Put slave register address in Tx buffer
  Wire.endTransmission(false);       // Send the Tx buffer, but send a restart to keep connection alive
  uint8_t i = 0;
  Wire.requestFrom(address, count);  // Read bytes from slave register address 
  while (Wire.available()) {dest[i++] = Wire.read(); } // Put read results in the Rx buffer
}

