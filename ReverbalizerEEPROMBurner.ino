/* 
  *  Use the I2C bus with EEPROM 24LC64 
  *   
  */

#include <Wire.h> //I2C library
#include <avr/pgmspace.h>
#include "ROM1.h"
#include "ROM2.h"
#include "ROM3.h"*/
#include "ROM4.h"
#include "ROM5.h"
#include "ROM6.h"
#include "ROM7.h"
#include "ROM8.h"

#define EEPROM_ADDR 0x50
#define MAX_LENGTH 512

byte* prog_addr[] = {(byte *)ROM_00, (byte *)ROM_01, (byte *)ROM_02, (byte *)ROM_03, (byte *)ROM_04,  (byte *)ROM_05, (byte *)ROM_06, (byte *)ROM_07 };

// data can be maximum of about 30 bytes, because the Wire library has a buffer of 32 bytes
void i2c_eeprom_write_page( int deviceaddress, unsigned int eeaddresspage, byte* data, unsigned int page ) {
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddresspage >> 8)); // MSB
    Wire.write((int)(eeaddresspage & 0xFF)); // LSB

    for ( int c = page; c < page+16; c++) {
      //Serial.print("-->");
      //Serial.println(pgm_read_byte_near(data+c));
      Wire.write(pgm_read_byte_near(data+c));
    }
    Wire.endTransmission();
}

byte i2c_eeprom_read_byte( int deviceaddress, unsigned int eeaddress ) {
    byte rdata = 0xFF;
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.endTransmission();
    Wire.requestFrom(deviceaddress,1);
    if (Wire.available()) rdata = Wire.read();
    return rdata;
}


void setup() {
  delay(1000);

  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);
  digitalWrite(16, HIGH);
  digitalWrite(17, LOW);
  Serial.begin(9600);
  while (!Serial) {
  ; // wait for serial port to connect. Needed for Leonardo only
  }
  
  Serial.println("Initialize EEPROM communication");
  Wire.begin(); // initialise the connection
 
  for (int program = 0; program<8; program++) {
    Serial.print("Writing program ");
    Serial.println(program);
    for (int page=0; page<MAX_LENGTH; page+=16) {
      // write 16 bytes at a time to not stress out Wire buffer
      //Serial.print((int)prog_addr[program]);
      //Serial.print(" ");
      //Serial.println(page+program*512);
      i2c_eeprom_write_page(EEPROM_ADDR, page+program*512, prog_addr[program], page); // write to EEPROM 
      delay(10); //add a small delay
    }
  }
  Serial.println("Memory written");
}

void loop() {
  int addr=0; //first address
  delay(10);
    while (addr<512) {
      for (int program = 0; program<8; program++) {
        Serial.print(addr+program*512);
        Serial.print(":");
       
        Serial.print(i2c_eeprom_read_byte(EEPROM_ADDR, addr+program*512), HEX); 
        Serial.print(" ");
        Serial.print(i2c_eeprom_read_byte(EEPROM_ADDR, addr+program*512+1), HEX);
        Serial.print(" ");
        Serial.print(i2c_eeprom_read_byte(EEPROM_ADDR, addr+program*512+2), HEX);
        Serial.print(" ");
        Serial.print(i2c_eeprom_read_byte(EEPROM_ADDR, addr+program*512+3), HEX);
        Serial.print("|");
      }
      addr+=4; 
      Serial.println();
      delay(10);
  }
  Serial.println("-----------------");
  while(1) {}
}



