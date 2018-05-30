#include <Arduino.h>
#include <Wire.h>
#include <avr/io.h>

//******************************************************************************/
// Created by: Robert Keller
// Date: 20 MAY 2018
// File Description: This file is responsible for reading and calibrating the
// values from the five flex sensors of the glove and sending them to the arm
// receiver via Zigbee protocol
//*******************************************************************************/

const int flexSensor[] = {A0, A1, A2, A3, A4}; //create array for input pin of each flex sensor: thumb --> pinky
int maxVal[] = {400, 400, 400, 400, 400}; //initialize maximum values detected by each sensor
int minVal[] = {250, 250, 250, 250, 250}; //initialize minimum values detected by each sensor
int sensorVal[5]; //empty vector to store the 5 voltage values read by the sensors

void setup()
{
  Serial.begin(9600); //set baud rate for serial data
  while(millis() < 10000) //set 10-second sensor calibration period
  {
    //sensor calibration loop; store new minimum and maxmimum values detected by sensors
    for(int i = 0; i <= 4; i++)
    {
      sensorVal[i] = analogRead(flexSensor[i]);
      if(sensorVal[i] < minVal[i])
      {
        minVal[i] = sensorVal[i];
      }
      if(sensorVal[i] > maxVal[i])
      {
        maxVal[i] = sensorVal[i];
      }
      delay(20); //cmiliseconompare new values every 20 ds
    }
  }
}

void loop()
{
  //loop to set servo positions according to
  int servoPos[5]; //empty vector to store servo positons
  for(int i = 0; i <= 4; i++)
  {
    sensorVal[i] = analogRead(flexSensor[i]); //read each sensor value from respective flex sensor pin
    servoPos[i] = map(sensorVal[i], maxVal[i]-10, minVal[i], -50, 180); //linearly map minimum and maximum sensor values to 0 and 180 degrees respectively; adjustments made to min and max values to reduce the interval
    servoPos[i] = constrain(servoPos[i], 0, 180); //prevent new values from falling outside of 0 to 180 degree range
    if(servoPos[i] > 175){
      servoPos[i] = 180;
    }
    if(servoPos[i] < 30){
      servoPos[i] = 0;
    }
    if(servoPos[2] < 45){ //adjustment for middle finger
      servoPos[2] = 0;
    }

    /*************HARDCODED COMMON HAND GESTURES*****************************************************/
    if(sensorVal[0] < 270 && sensorVal[1] < 245 && sensorVal[2] > 325 && sensorVal[3] < 295 && sensorVal[4] < 295){ //middle finger
        servoPos[0] = 180; servoPos[1] = 180; servoPos[2] = 0; servoPos[3] = 180; servoPos[4] = 180;
    }
      else if(sensorVal[0] > 430 && sensorVal[1] > 360 && sensorVal[2] < 260 && sensorVal[3] < 300 && sensorVal[4] < 265){ //L
        servoPos[0] = 0; servoPos[1] = 0; servoPos[2] = 180; servoPos[3] = 180; servoPos[4] = 180;
      }
      else if(sensorVal[0] < 255 && sensorVal[1] > 360 && sensorVal[2] < 260 && sensorVal[3] < 290 && sensorVal[4] > 385){ //horns
        servoPos[0] = 180; servoPos[1] = 0; servoPos[2] = 180; servoPos[3] = 180; servoPos[4] = 0;
      }
      else if(sensorVal[0] > 420 && sensorVal[1] < 245 && sensorVal[2] < 260 && sensorVal[3] < 290 && sensorVal[4] > 390){ //hang loose
        servoPos[0] = 0; servoPos[1] = 180; servoPos[2] = 180; servoPos[3] = 180; servoPos[4] = 0;
      }
      else if(sensorVal[0] > 415 && sensorVal[1] > 380 && sensorVal[2] < 265 && sensorVal[3] < 280 && sensorVal[4] > 420){ //spiderman
        servoPos[0] = 0; servoPos[1] = 0; servoPos[2] = 180; servoPos[3] = 180; servoPos[4] = 0;
      }
      else if(sensorVal[0] < 280 && sensorVal[1] > 360 && sensorVal[2] > 330 && sensorVal[3] < 310 && sensorVal[4] < 325){ //peace sign
        servoPos[0] = 180; servoPos[1] = 0; servoPos[2] = 0; servoPos[3] = 180; servoPos[4] = 180;
      }
      /***********************************************************************************************/

    Serial.write(byte(servoPos[i])); //send command to XBee.

//    Serial.print(sensorVal[0]); //for serial debugging, comment out previous line
//    Serial.print("\t");
//    Serial.print(sensorVal[1]);
//    Serial.print("\t");
//    Serial.print(sensorVal[2]);
//    Serial.print("\t");
//    Serial.print(sensorVal[3]);
//    Serial.print("\t");
//    Serial.print(sensorVal[4]);
//    Serial.println();
//    delay(300);

  }
  delay(40); //delay 40ms before repeating loop (response time)
}
