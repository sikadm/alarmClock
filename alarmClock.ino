
/* Alarm Clock: requires Arduino Uno, Real Time Clock, piezo buzzer,
 *  and 3 push buttons--for hour, minutes, and set buttons. 
*/

#include "RTClib.h"
#include <Wire.h>

RTC_DS3231 rtc;

const int hrbuttonPin = 2;
const int minbuttonPin = 3;
const int SetbuttonPin = 4;
const int buzzerPin = 5;

int hrCounter = 0;   // counter for the number of button presses for hours
int hrState = 0;         // current state of the button
int lastHrState = 0;     // previous state of the button

int minCounter = 0;   // counter for the number of button presses for minutes
int minState = 0;         // current state of the button
int lastMinState = 0;     // previous state of the button

int SetState = 0;     // current state of alarm set button
int setCounter = 0;   // counts the number of state switches
int lastSetState = 0; // previous state of button

  
void setup() 
{
  pinMode(hrbuttonPin, INPUT);
  pinMode(minbuttonPin, INPUT);  
  pinMode(SetbuttonPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);
  Wire.begin();
  Serial.println("Set the time. Click the set button when time is correct");
}

void loop() {
  hrState = digitalRead(hrbuttonPin);
  minState = digitalRead(minbuttonPin);
  SetState = digitalRead(SetbuttonPin);
  if (SetState == LOW && setCounter == 0) {
    if (hrState != lastHrState) {
        hrCounter++;
        if (hrCounter = 25) {   //return to 1 if reach midnight -- military time
          hrCounter = 1;
        }
      }
      else if (minState != lastMinState) {
        minCounter++;
        if (minCounter = 60) {
          minCounter = 0;
        }
      }
      lastHrState = hrState;
      lastMinState = minState;
      lastSetState = SetState;
      Serial.print(hrCounter);
      Serial.print(":");
      if (minCounter < 10) {
        Serial.print('0');
        Serial.print(minCounter);
      }
      else {
        Serial.print(minCounter);
      }
      Serial.println();
      Serial.println("Click the Set button if this time is correct.");
  }
  else if (SetState == HIGH) {
      // After the set button has been pressed, you can set the time
      // press button again to save the alarm time
    if (SetState != lastSetState) {
      rtc.adjust(DateTime(2014, 1, 21, hrCounter, minCounter, 0));
      setCounter++;
      hrCounter = 0;
      minCounter = 0;
    }
    else {
      if (hrState != lastHrState) {
        // if the state has changed, increment the counter
        hrCounter++;
        if (hrCounter = 25) {
          hrCounter = 1;
        }
      }
      else if (minState != lastMinState) {
        minCounter++;
        if (minCounter = 60) {
          minCounter = 0;
        }
      }
      lastHrState = hrState;
      lastMinState = minState;
      lastSetState = SetState;
      Serial.print(hrCounter);
      Serial.print(":");
      if (minCounter < 10) {
        Serial.print('0');
        Serial.print(minCounter);
      }
      else {
        Serial.print(minCounter);
      }
      Serial.println();
      Serial.println("Click the Set button if this alarm time is correct.");
    }
  }
  else if (SetState == LOW && setCounter != 0) {
     DateTime now = rtc.now();
     if (now.hour() == byte(hrCounter)) {
        if (now.minute() == byte(minCounter)) {
          while (SetState == LOW) {
            tone(buzzerPin, 1000, 500);
            delay (10000);
            hrCounter = 0;
            minCounter = 0;
          }
        }
        else {
          Serial.print(now.hour(), DEC);
          Serial.print(':');
          Serial.print(now.minute(), DEC);
        }
      }
      else {
          Serial.print(now.hour(), DEC);
          Serial.print(':');
          Serial.print(now.minute(), DEC);        
      }
    }
  }
