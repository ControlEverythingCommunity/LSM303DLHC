// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// LSM303DLHC
// This code is designed to work with the LSM303DLHC_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/products

#include <application.h>
#include <spark_wiring_i2c.h>

// LSM303DLHC acceerometer I2C address is 0x19(25)
#define Addr_Accl 0x19
// LSM303DLHC magnetometer I2C address is 0x1E(30)
#define Addr_Mag 0x1E

int xAccl = 0, yAccl =  0, zAccl = 0,  xMag = 0, yMag = 0, zMag = 0;
void setup()
{
    // Set variable
    Particle.variable("i2cdevice", "LSM303DLHC");
    Particle.variable("xAccl", xAccl);
    Particle.variable("yAccl", yAccl);
    Particle.variable("zAccl", zAccl);
    Particle.variable("xMag", xMag);
    Particle.variable("yMag", yMag);
    Particle.variable("zMag", zMag);
    
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
        Wire.write((40 + i));
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
    delay(300);
    
    // Convert the data
    xAccl = ((data[1] * 256) + data[0]);
    if(xAccl > 32767)
    {
      xAccl -= 65536;
    }
  
    yAccl = ((data[3] * 256) + data[2]);
    if(yAccl > 32767)
    {
      yAccl -= 65536;
    }
    
    zAccl = ((data[5] * 256) + data[4]);
    if(zAccl > 32767)
    {
      zAccl -= 65536;
    }

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
        Wire.write((3 + i));
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
    delay(300);
    
    // Convert the data
    xMag = ((data[1] * 256) + data[0]);
    if(xMag > 32767)
    {
      xMag -= 65536;
    }
    yMag = ((data[3] * 256) + data[2]);
    if(yMag > 32767)
    {
      yMag -= 65536;
    }
    zMag = ((data[5] * 256) + data[4]);
    if(zMag > 32767)
    {
      zMag -= 65536;
    }
    
    // Output data to dashboard
    Particle.publish("Acceleration in X-Axis : ", String(xAccl));
    Particle.publish("Acceleration in Y-Axis : ", String(yAccl));
    Particle.publish("Acceleration in Z-Axis : ", String(zAccl));
    delay(1000);
    Particle.publish("Magnetic Field in X-Axis: ", String(xMag));
    Particle.publish("Magnetic Field in Y-Axis: ", String(yMag));
    Particle.publish("Magnetic Field in Z-Axis: ", String(zMag));
    delay(1000);
}
