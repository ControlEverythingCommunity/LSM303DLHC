// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// LSM303DLHC
// This code is designed to work with the LSM303DLHC_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/products

#include<Wire.h>

// LSM303DLHC acceerometer I2C address is 0x19(25)
#define Addr_Accl 0x19
// LSM303DLHC magnetometer I2C address is 0x1E(30)
#define Addr_Mag 0x1E


void setup()
{
  // Initialise I2C communication as MASTER
  Wire.begin();
  // Initialise Serial Communication, set baud rate = 9600
  Serial.begin(9600);
  delay(300);
}

void loop()
{
  unsigned int data[6];
  // Start I2C transmission
  Wire.beginTransmission(Addr_Accl);
  // Select control register 1
  Wire.write(0x20);
  // Enable X, Y, Z-Axis
  Wire.write(0x27);
  // Stop I2C transmission
  Wire.endTransmission();
 
  // Start I2C transmission
  Wire.beginTransmission(Addr_Accl);
  // Select control register 4
  Wire.write(0x23);
  // Full scale 2000 dps, continuous update
  Wire.write(0x00);
  // Stop I2C transmission
  Wire.endTransmission();
  
  for(int i = 0; i < 6; i++)
  { 
    // Start I2C Transmission
    Wire.beginTransmission(Addr_Accl);
    // Select data register
    Wire.write((40+i));
    // Stop I2C Transmission
    Wire.endTransmission();
    
    // Request 6 byte of data from the device
    // xAccl lsb, xAccl msb, yAccl lsb, yAccl msb, zAccl lsb, zAccl msb
    Wire.requestFrom(Addr_Accl, 1);
    
    if(Wire.available() == 1)
    {
      data[i] = Wire.read();
    }
  }
  
  // Convert the data
  int xAccl =  (data[1] * 256) + data[0];
  int yAccl =  (data[3] * 256) + data[2];
  int zAccl =  (data[5] * 256) + data[6];
  
  // Output data to serial monitor
  Serial.print("Acceleration in X-Axis : ");
  Serial.print(xAccl); 
  Serial.println(" g"); 
  Serial.print("Acceleration in Y-Axis : ");
  Serial.print(yAccl);
  Serial.println(" g"); 
  Serial.print("Acceleration in Z-Axis : ");
  Serial.print(zAccl);
  Serial.println(" g");
  delay(1000);
  
  // Start I2C Transmission
  Wire.beginTransmission(Addr_Mag);
  // Select Mode select register
  Wire.write(0x02);
  // Continuous conversion mode
  Wire.write(0x00);
  // Stop I2C Transmission
  Wire.endTransmission();
  
  // Start I2C Transmission
  Wire.beginTransmission(Addr_Mag);
  // Select gain configuration register
  Wire.write(0x01);
  // Gain +/-1.3
  Wire.write(0x20);
  // Stop I2C Transmission
  Wire.endTransmission();

  for(int i = 0; i < 6; i++)
  { 
    // Start I2C Transmission
    Wire.beginTransmission(Addr_Mag);
    // Select data register
    Wire.write((3+i));
    // Stop I2C Transmission
    Wire.endTransmission();
    
    // Request 6 byte of data
    // xAccl lsb, xAccl msb, yAccl lsb, yAccl msb, zAccl lsb, zAccl msb
    Wire.requestFrom(Addr_Mag, 1);
    
    if(Wire.available() == 1)
    {
      data[i] = Wire.read();
    }
  }
  
  // Convert the data
  int xMag =  (data[1] * 256) + data[0];
  int yMag =  (data[5] * 256) + data[4];
  int zMag =  (data[3] * 256) + data[2];
  
  // Output data to serial monitor
  Serial.print("Magnetic Field in X-Axis: ");
  Serial.println(xMag);  
  Serial.print("Magnetic Field in Y-Axis: ");
  Serial.println(yMag); 
  Serial.print("Magnetic Field in Z-Axis: ");
  Serial.println(zMag);
  delay(1000);
}
