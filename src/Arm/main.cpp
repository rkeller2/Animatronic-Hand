#include <Arduino.h>
#include <Wire.h>
#include <avr/io.h>
#include <Servo.h>

//******************************************************************************/
// Created by: Robert Keller
// Date: 20 MAY 2018
// File Description: This file is responsible for receiving the commands from
// the glove via Zigbee protocol and sending them to the servo motors on the arm
//*******************************************************************************/

Servo servo[5]; //empty vector to define 5 servo motors

void setup()
{
  Serial.begin(9600); //set baud rate for serial communication
  //declare servo pins
  for(int i = 0; i<5; i++)
  {
    servo[i].attach(2+2*i); //pins 2, 4, 6, 8, 10: thunmb --> pinky
  }
}
void loop()
{
  byte buffer[5]; //empty vector to store values received by XBee
  if(Serial.available() > 0) //reads serial port and returns the number of bytes available
  {
    Serial.readBytes(buffer,5);
    for(int i = 0; i <5; i++)
    {
      servo[i].write(buffer[i]); //send position (0 to 180 degrees) to each servo
    }
  }
}
