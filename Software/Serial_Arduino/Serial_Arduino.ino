/*
  Software serial multple serial test

 Receives from the hardware serial, sends to software serial.
 Receives from software serial, sends to hardware serial.

 The circuit:
 * RX is digital pin 10 (connect to TX of other device)
 * TX is digital pin 11 (connect to RX of other device)

 Note:
 Not all pins on the Mega and Mega 2560 support change interrupts,
 so only the following can be used for RX:
 10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69

 Not all pins on the Leonardo support change interrupts,
 so only the following can be used for RX:
 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).

 created back in the mists of time
 modified 25 May 2012
 by Tom Igoe
 based on Mikal Hart's example

 This example code is in the public domain.

 */
#include <stdint.h> 
#include <SparkFunTSL2561.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Arduino.h>
#define LENG 31   //0x42 + 31 bytes equal to 32 bytes
unsigned char buf[LENG];
 
#define SEALEVELPRESSURE_HPA (1013.25)
SFE_TSL2561 light;
Adafruit_BME280 bme; // I2C

SoftwareSerial mySerial(8, 9); // RX, TX
int UVOUT = A2; //Output from the sensor
int REF_3V3 = A3; //3.3V power on the Arduino board

int PM01Value=0;          //define PM1.0 value of the air detector module
int PM2_5Value=0;         //define PM2.5 value of the air detector module
int PM10Value=0;         //define PM10 value of the air detector module

#define PRE_PIN          7
#define VNOX_PIN         A0
#define VRED_PIN         A1

#define PRE_HEAT_SECONDS 10

int vnox_value = 0;
int vred_value = 0;

boolean gain;     // Gain setting, 0 = X1, 1 = X16;
unsigned int ms;  // Integration ("shutter") time in milliseconds

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  mySerial.begin(115200);
  Serial.setTimeout(1500);
  pinMode(UVOUT, INPUT);
  pinMode(REF_3V3, INPUT);

pinMode(PRE_PIN, OUTPUT);
  pinMode(A6 , INPUT);
  digitalWrite(PRE_PIN, 1);
 delay(PRE_HEAT_SECONDS * 1000);
  digitalWrite(PRE_PIN, 0);

    light.begin();
     gain = 0;
     
  unsigned char time = 1;
  light.setTiming(gain,time,ms);
  light.setPowerUp();
  if (!bme.begin()) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
   
}

void loop() { // run over and over
if (mySerial.read() == 0x16) {
    while(Serial.read() != 'F'){
   mySerial.println("D");
   mySerial.print(uvsensor());
  mySerial.print(",");
     mySerial.print(float(lightsensor()));
      mySerial.print(",");
 mySerial.print(bme.readTemperature());
  mySerial.print(",");
      mySerial.print(bme.readPressure() / 100.0F);
        mySerial.print(",");
   mySerial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    mySerial.print(","); 
   mySerial.print(bme.readHumidity());
    mySerial.print(",");
   mySerial.print(PM01Value);
    mySerial.print(",");
   mySerial.print(PM2_5Value);
    mySerial.print(",");
   mySerial.print(PM10Value);
   mySerial.print(",");
   mySerial.print(cosensor());
    mySerial.print(",");
  mySerial.print(no2sensor());
   mySerial.print(",");
  mySerial.print(micsensor());
  delay(1000);
    }
 }
 
 if(Serial.find(0x42)){    //start to read when detect 0x42
    Serial.readBytes(buf,LENG);
 
    if(buf[0] == 0x4d){
      if(checkValue(buf,LENG)){
        PM01Value=transmitPM01(buf); //count PM1.0 value of the air detector module
        PM2_5Value=transmitPM2_5(buf);//count PM2.5 value of the air detector module
        PM10Value=transmitPM10(buf); //count PM10 value of the air detector module 
      }           
    } 
  }
   
}
//Takes an average of readings on a given pin
//Returns the average
int averageAnalogRead(int pinToRead)
{
  byte numberOfReadings = 8;
  unsigned int runningValue = 0; 

  for(int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
  runningValue /= numberOfReadings;

  return(runningValue);  
}

//The Arduino Map function but for floats
//From: http://forum.arduino.cc/index.php?topic=3922.0
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float uvsensor(){
  int uvLevel = averageAnalogRead(UVOUT);
  int refLevel = averageAnalogRead(REF_3V3);
  
  //Use the 3.3V power pin as a reference to get a very accurate output value from sensor
  float outputVoltage = 3.3 / refLevel * uvLevel;
  
  float uvIntensity = mapfloat(outputVoltage, 0.99, 2.9, 0.0, 15.0);

  return uvIntensity;  
  }

double lightsensor(){
  delay(ms);
  // light.manualStop();
  
  // Once integration is complete, we'll retrieve the data.
  
  // There are two light sensors on the device, one for visible light
  // and one for infrared. Both sensors are needed for lux calculations.
  
  // Retrieve the data from the device:

  unsigned int data0, data1;
  
  if (light.getData(data0,data1))
  {

    double lux;    // Resulting lux value
    boolean good;  // True if neither sensor is saturated
    
    // Perform lux calculation:

    good = light.getLux(gain,ms,data0,data1,lux);
    
    // Print out the results:
  
   return lux ;
  
  }
  }
  
 char checkValue(unsigned char *thebuf, char leng)
{  
  char receiveflag=0;
  int receiveSum=0;
 
  for(int i=0; i<(leng-2); i++){
  receiveSum=receiveSum+thebuf[i];
  }
  receiveSum=receiveSum + 0x42;
  
  if(receiveSum == ((thebuf[leng-2]<<8)+thebuf[leng-1]))  //check the serial data 
  {
    receiveSum = 0;
    receiveflag = 1;
  }
  return receiveflag;
}
 
int transmitPM01(unsigned char *thebuf)
{
  int PM01Val;
  PM01Val=((thebuf[3]<<8) + thebuf[4]); //count PM1.0 value of the air detector module
  return PM01Val;
}
 
//transmit PM Value to PC
int transmitPM2_5(unsigned char *thebuf)
{
  int PM2_5Val;
  PM2_5Val=((thebuf[5]<<8) + thebuf[6]);//count PM2.5 value of the air detector module
  return PM2_5Val;
  }
 
//transmit PM Value to PC
int transmitPM10(unsigned char *thebuf)
{
  int PM10Val;
  PM10Val=((thebuf[7]<<8) + thebuf[8]); //count PM10 value of the air detector module  
  return PM10Val;
}

int cosensor(){
  vred_value = analogRead(VRED_PIN);
  vred_value = map(vred_value,0,1023,0,100);
  return vred_value;
  }
  
int no2sensor(){
  vnox_value = analogRead(VNOX_PIN);
  vnox_value  = map(vnox_value ,0,1023,0,100);
  return vnox_value;
  }  

 int micsensor(){
    int dB = 20 * log10(analogRead(A6)/0.005010);
int x = (-44) + dB;
int y = x + 92;
int rez = y-40;
return rez;    
    }
