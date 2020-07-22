/*
 * Treat & Train Custom Driver
 * Author: Walker Arce, March 2020
 * 
 * This program is used to drive the Treat & Train device for pellet dispensation.
 * It operates using a simple USB command protocol and can also be driven by the use of the
 * accompanying RF remote.
 * 
 * The current operation of the system is as follows:
 *    1. A command is sent over USB to dispense a pellet,
 *        a. Drives wheel until pellet sensor is broken or wheel goes a full rotation.
 *    2. The button 'Dispense' is clicked on the RF remote
 *        a. Drives wheel until pellet sensor is broken or wheel goes a full rotation.
 *        
 * The motor can be driven at a variety of levels, for the second state of operation, it will be driven at half speed (6V DC).
 * 
 * This initial version is meant to simply replicate functionality.  The Treat & Train can be restored to original functionality
 * by reconnecting the internal board and sensors.
 * 
 * v0.1
 * 16 March 2020
 */

#include "pins.h"

float Vout = 0.00;
float Vin = 0.00;
float R1 = 1000000.00; // resistance of R1 (1M) 
float R2 = 1000000.00; // resistance of R2 (1M) 

float wheelVoltage = 0;
float treatVoltage = 0;
int clickSignal = 0;

int clickCycles = 1;
int globalMotorSpeed = 127;

void setup()
{
   Serial.begin(9600);
   initPins();
   stopMotor();
   //testWheel();
   //moveMotor(50, true); 
}

void loop()
{
  //Listen for the RF remote
  if (digitalRead(CLICK) == HIGH)
  {
    if(dispenseCommand(clickCycles, 127, false, false))
    {
      Serial.println("Successfully dispensed");
    }
    else
    {
      Serial.println("Error occurred");
    }
    while(digitalRead(CLICK) == HIGH) {}
  }
  //Wait for USB input
  while(Serial.available() > 0)
  {
    char command[4];
    size_t bytesRead = Serial.readBytes(command, 4);
    //Check for 'C' being sent
    if (command[0] == 67 && command[3] == 69)
    {
      switch(command[1])
      {
        //Check for 'D' being sent
        //This dispenses the specified number of treats, give or take.
        case 68:
          if (dispenseCommand(command[2], globalMotorSpeed, false, true))
          {
            Serial.write(0x00);
          }
          break;
        //Check for 'P' being sent
        //This prints out the current state of the internal sensors.
        case 80:
          debugSensors();
          Serial.write(0x00);
          break;
        //Check for 'B' being sent
        //This updates the number of treats dispensed when RF remote is clicked
        case 66:
          clickCycles = command[2] - 0x30;
          Serial.write(0x00);
          break;
        //Command not recognized
        default:
          Serial.write(0x01);
      }
    }
    else
    {
      Serial.write(0x01);
    }
  }
}

void testWheel()
{
  int motorSpeed = 0;
  while(motorSpeed != 255/2){
  moveMotor(motorSpeed++, true);
  wheelVoltage = getVoltage(WHEEL_SENSOR, 10);
  wheelVoltage = roundFloat(wheelVoltage, 1); 

  treatVoltage = getVoltage(TREAT_SENSOR, 10);
  treatVoltage = roundFloat(treatVoltage, 1);

  Serial.print("Wheel Voltage: ");
  Serial.print(wheelVoltage);
  Serial.print("V | Treat Voltage: ");
  Serial.print(treatVoltage);
  Serial.print("V");
  Serial.print(" | Speed: ");
  Serial.print(motorSpeed);
  Serial.println();
  delay(500);
  }
  stopMotor();
}

void debugSensors()
{
  wheelVoltage = getVoltage(WHEEL_SENSOR, 10);
  wheelVoltage = roundFloat(wheelVoltage, 1); 

  treatVoltage = getVoltage(TREAT_SENSOR, 10);
  treatVoltage = roundFloat(treatVoltage, 1);

  Serial.print("Wheel Voltage: ");
  Serial.print(wheelVoltage);
  Serial.print("V | Treat Voltage: ");
  Serial.print(treatVoltage);
  Serial.print("V");
  Serial.println();
}
bool dispenseCommand(int cycles, int motorSpeed, bool motorDir, bool isASCII)
{
  int treatSamples = cycles;
  if (isASCII) { treatSamples = cycles - 0x30; }
  int wheelSamples = treatSamples * 3;
  moveMotor(motorSpeed, motorDir);
  
  while (treatSamples != 0)
  {
    treatVoltage = getVoltage(TREAT_SENSOR, 10);
    treatVoltage = roundFloat(treatVoltage, 1);

    if (treatVoltage < 1.00)
    {
      treatSamples--;
      while (treatVoltage < 1.00)
      {
        treatVoltage = getVoltage(TREAT_SENSOR, 10);
        treatVoltage = roundFloat(treatVoltage, 1);
      }
    }
    /*
    wheelVoltage = getVoltage(WHEEL_SENSOR, 10);
    wheelVoltage = roundFloat(wheelVoltage, 1); 
    
    if (wheelVoltage < 4.90)
    {
      wheelSamples--;
      delay(500);
      while (wheelVoltage < 4.90)
      {
        wheelVoltage = getVoltage(WHEEL_SENSOR, 10);
        wheelVoltage = roundFloat(wheelVoltage, 1); 
      }
    }
    */
  }
  stopMotor();
  return true;
}

//Source: https://stackoverflow.com/questions/1343890/how-do-i-restrict-a-float-value-to-only-two-places-after-the-decimal-point-in-c
float roundFloat(float raw, int precision)
{
  return floorf(raw * (10 * precision)) / 10;
}
//Source: https://create.arduino.cc/projecthub/next-tech-lab/voltmeter-using-arduino-00e7d1
float getVoltage(int pin, int samples)
{
  int val = getAverageReading(pin, samples);
  Vout = (val * 5.00) / 1024.00; // formula for calculating voltage out i.e. V+, here 5.00
  Vin = Vout / (R2/(R1+R2)); // formula for calculating voltage in i.e. GND
  if (Vin<0.09)//condition 
  {
   Vin=0.00;
  } 
  return Vin;
}

int getAverageReading(int pin, int numReadings)
{
  int adcReading = 0;
  for (int i = 0; i < numReadings; i++)
  {
    adcReading += analogRead(pin);
  }
  adcReading /= numReadings;
  return adcReading;
}

void moveMotor(int motorSpeed, bool motorDir)
{
  //Set motor speed before driving motor
  analogWrite(MSPEED, motorSpeed);
  //If true, then drive it clockwise
  if (motorDir)
  {
    digitalWrite(MDRIVERA, HIGH);
    digitalWrite(MDRIVERB, LOW);
  }
  //If false, then drive it counter-clockwise
  else
  {
    digitalWrite(MDRIVERA, LOW);
    digitalWrite(MDRIVERB, HIGH);
  }
}

void stopMotor()
{
  analogWrite(MSPEED, 0);
  digitalWrite(MDRIVERA, LOW);
  digitalWrite(MDRIVERB, LOW);
}

void initPins()
{
  //Initialize inputs
  pinMode(WHEEL_SENSOR, INPUT);
  pinMode(TREAT_SENSOR, INPUT);
  pinMode(CLICK, INPUT);

  //Initialize outputs
  pinMode(MDRIVERA, OUTPUT);
  pinMode(MDRIVERB, OUTPUT);
  pinMode(MSPEED, OUTPUT);
}
