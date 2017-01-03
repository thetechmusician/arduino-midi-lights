/* 
 *  Christmas New year light show with Arduino
 *  by Akshay James
 *  techmusician.wordpress.com
 * 
 * This project was inspired from a YouTube video by John Storms
 * from his channel ListenToOurLights (http://www.youtube.com/user/listentoourlights).
 * Thank you.

 * GND (pin 8) to ground,
 * Vcc (pin 16) to 5V
 * OE (pin 13) to ground
 * MR (pin 10) to 5V
 * 
 * DS (pin 14) to Ardunio DigitalPin 9 (blue wire)
 * SH_CP (pin 11) to to Ardunio DigitalPin 8 (yellow wire)
 * ST_CP (pin 12) to Ardunio DigitalPin 10 (green wire)
 */


#include"notes_bits.h"

//Pin connected to latch pin (ST_CP) of 74HC595
const int latchPin = 9;
//Pin connected to clock pin (SH_CP) of 74HC595
const int clockPin = 8;
//Pin connected to Data in (DS) of 74HC595
const int dataPin = 10;

int pitch, cmd, velocity;
int light;
boolean state;

byte lightData1 = 0, lightData2 = 0, lightData3 = 0; //

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  Serial.begin(38400);
  
}

void loop() {

  //read the midi signal
  if(Serial.available()>2) {
    cmd = Serial.read();
    pitch = Serial.read();
    velocity = Serial.read();
  
  //the lights are set according to the data on MIDI ch.1
  if(cmd==144) state=0;               //144 ch1 noteON ; 0=relayON
  else if(cmd==128) state=1;        //128 ch1 noteOFF  ; 1=relayOFF
    
  // so that lights don't change while the new data is being shifted
  digitalWrite(latchPin, LOW);

  light = getBit(pitch);
  // set the corresponding bit in lightData
  if(light<8)
    bitWrite(lightData1, light, state);
  else if(light<16){
    light-=8;
    bitWrite(lightData2, light, state);
  }
  else {
    light-=16;
    bitWrite(lightData3, light, state);
  }
    
  
  // shift out the data
  shiftOut(dataPin, clockPin, MSBFIRST, lightData3);
  shiftOut(dataPin, clockPin, MSBFIRST, lightData2);
  shiftOut(dataPin, clockPin, MSBFIRST, lightData1);

  // turn on the lights according to the new data
  digitalWrite(latchPin, HIGH);

  }
}
