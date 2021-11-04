// Arduino synth library MIDI example
//
// The arduino receives MIDI over Serial at 31250 BAUD
//
// NOTE_ON with velocity over zero triggers voice. 
// Potentiometer on Arduino Pin 1 controls waveform: SINE[0-21] TRIANGLE[22-42] SQUARE[43-63] SAW[64-84] RAMP[85-105] NOISE[106-127]
// Potentiometer on Arduino Pin 2 controls length/decay: [0-127]
// Potentiometer on Arduino Pin 3 controls envelope: ENVELOPE0[0-32] ENVELOPE1[33-64] ENVELOPE2[65-96] ENVELOPE3[97-127]
// Potentiometer on Arduino Pin 4 controls modulation: negative[0-63] neutral[64] positive[65-127]
//
//

#include <synth.h>
#include "MIDI_parser.h"

synth edgar;        //-Make a synth
midiParser parser;  //-Make a MIDI parser

const int shapePin = A1;
const int lenPin = A2;
const int envPin = A3;
const int modPin = A4;

const int sin_LED = 5;
const int tri_LED = 6;
const int sqr_LED = 7;
const int saw_LED = 8;
const int rmp_LED = 9;
const int noise_LED = 10;

int lenVal = 0;
int shapeVal = 0;
int envVal = 0;
int modVal = 0;

int numNotes = 0;
boolean byteReady; 
unsigned char midiByte;

void setup() 
{
  Serial.begin(31250);    //MIDI BAUD rate
  //Serial.begin(9600);    //MIDI BAUD rate
  byteReady = false;
  midiByte = 0; 
  edgar.begin();          //Init synth
  edgar.setupVoice(0,SINE,60,ENVELOPE0,90,64);
  edgar.setupVoice(1,SINE,62,ENVELOPE1,90,64);
  edgar.setupVoice(2,SINE,64,ENVELOPE2,90,64);
  edgar.setupVoice(3,SINE,67,ENVELOPE0,90,64);
  pinMode(13,OUTPUT);
  pinMode(sin_LED,OUTPUT);
  pinMode(tri_LED, OUTPUT);
  pinMode(sqr_LED, OUTPUT);
  pinMode(saw_LED, OUTPUT);
  pinMode(rmp_LED, OUTPUT);
  pinMode(noise_LED, OUTPUT);
  pinMode(shapePin, INPUT);
  pinMode(lenPin, INPUT);
  pinMode(envPin, INPUT);
  pinMode(modPin, INPUT);
}

void loop()
{
  unsigned char voice;
  lenVal = (analogRead(lenPin) / 8);
  shapeVal = (analogRead(shapePin) / 8);
  envVal = (analogRead(envPin) / 8);
  modVal = (analogRead(modPin) / 8);

  if (byteReady) {
      byteReady = false;
      Serial.write(midiByte);
   }

  if(shapeVal >=0 && shapeVal < 22) {
    edgar.setWave(0,NOISE);
    edgar.setWave(1,NOISE);
    edgar.setWave(2,NOISE);
    edgar.setWave(3,NOISE);
    digitalWrite(sin_LED, LOW);
    digitalWrite(tri_LED, LOW);
    digitalWrite(sqr_LED, LOW);
    digitalWrite(saw_LED, LOW);
    digitalWrite(rmp_LED, LOW);
    digitalWrite(noise_LED, HIGH);
  } else if(shapeVal > 23 && shapeVal < 42) {
    edgar.setWave(0,RAMP);
    edgar.setWave(1,RAMP);
    edgar.setWave(2,RAMP);
    edgar.setWave(3,RAMP);
    digitalWrite(sin_LED, LOW);
    digitalWrite(tri_LED, LOW);
    digitalWrite(sqr_LED, LOW);
    digitalWrite(saw_LED, LOW);
    digitalWrite(rmp_LED, HIGH);
    digitalWrite(noise_LED, LOW);
  } else if(shapeVal > 43 && shapeVal < 63) {
    edgar.setWave(0,SAW);
    edgar.setWave(1,SAW);
    edgar.setWave(2,SAW);
    edgar.setWave(3,SAW);
    digitalWrite(sin_LED, LOW);
    digitalWrite(tri_LED, LOW);
    digitalWrite(sqr_LED, LOW);
    digitalWrite(saw_LED, HIGH);
    digitalWrite(rmp_LED, LOW);
    digitalWrite(noise_LED, LOW);
  } else if(shapeVal > 63 && shapeVal < 84) {
    edgar.setWave(0,SQUARE);
    edgar.setWave(1,SQUARE);
    edgar.setWave(2,SQUARE);
    edgar.setWave(3,SQUARE);
    digitalWrite(sin_LED, LOW);
    digitalWrite(tri_LED, LOW);
    digitalWrite(sqr_LED, HIGH);
    digitalWrite(saw_LED, LOW);
    digitalWrite(rmp_LED, LOW);
    digitalWrite(noise_LED, LOW);
  } else if(shapeVal > 83 && shapeVal < 106) {
    edgar.setWave(0,TRIANGLE);
    edgar.setWave(1,TRIANGLE);
    edgar.setWave(2,TRIANGLE);
    edgar.setWave(3,TRIANGLE);
    digitalWrite(sin_LED, LOW);
    digitalWrite(tri_LED, HIGH);
    digitalWrite(sqr_LED, LOW);
    digitalWrite(saw_LED, LOW);
    digitalWrite(rmp_LED, LOW);
    digitalWrite(noise_LED, LOW);
  } else if(shapeVal > 105 && shapeVal < 128) {
    edgar.setWave(0, SINE);
    edgar.setWave(1, SINE);
    edgar.setWave(2, SINE);
    edgar.setWave(3, SINE);
    digitalWrite(sin_LED, HIGH);
    digitalWrite(tri_LED, LOW);
    digitalWrite(sqr_LED, LOW);
    digitalWrite(saw_LED, LOW);
    digitalWrite(rmp_LED, LOW);
    digitalWrite(noise_LED, LOW);
  }

  edgar.setLength(0,lenVal);
  edgar.setLength(1,lenVal);
  edgar.setLength(2,lenVal);
  edgar.setLength(3,lenVal);

  if(envVal >=0 && envVal < 33) {
    edgar.setEnvelope(0,ENVELOPE3);
    edgar.setEnvelope(1,ENVELOPE3);
    edgar.setEnvelope(2,ENVELOPE3);
    edgar.setEnvelope(3,ENVELOPE3);
  } else if(envVal > 33 && envVal < 65) {
    edgar.setEnvelope(0,ENVELOPE2);
    edgar.setEnvelope(1,ENVELOPE2);
    edgar.setEnvelope(2,ENVELOPE2);
    edgar.setEnvelope(3,ENVELOPE2);
  } else if(envVal > 64 && envVal < 97) {
    edgar.setEnvelope(0,ENVELOPE1);
    edgar.setEnvelope(1,ENVELOPE1);
    edgar.setEnvelope(2,ENVELOPE1);
    edgar.setEnvelope(3,ENVELOPE1);
  } else if(envVal > 96 && envVal < 128) {
    edgar.setEnvelope(0,ENVELOPE0);
    edgar.setEnvelope(1,ENVELOPE0);
    edgar.setEnvelope(2,ENVELOPE0);
    edgar.setEnvelope(3,ENVELOPE0);
  }

   edgar.setMod(0,modVal);
   edgar.setMod(1,modVal);
   edgar.setMod(2,modVal);
   edgar.setMod(3,modVal);
   
  while(Serial.available())
  {
    
    if(parser.update(Serial.read()))  //-Feed MIDI stream to parser and execute commands
    {
      switch(parser.midi_cmd)
      {
        //*********************************************
        // Handle MIDI notes
        //*********************************************
      case 0x90: //-Channel 1 (voice 0)
      case 0x91: //-Channel 2 (voice 1)
      case 0x92: //-Channel 3 (voice 2)
      case 0x93: //-Channel 4 (voice 3)

        voice = parser.midi_cmd-0x90;
        if(parser.midi_2nd)  { //-Velocity not zero (could implement NOTE_OFF here);
          if(numNotes>3) { numNotes = 0; }
          switch(numNotes) {
             case 0 :
                edgar.mTrigger(0,parser.midi_1st);
                numNotes++;
             break;

             case 1:
                edgar.mTrigger(1,parser.midi_1st);
                numNotes++;
             break;
             
             case 2:
                edgar.mTrigger(2,parser.midi_1st);
                numNotes++;
             break;

             case 3:
                edgar.mTrigger(3,parser.midi_1st);
                numNotes++;
             break;
          }
        } 
        break;
      }
    }
  }
}

void serialEvent() {
  if (Serial.available()) {
    // get the new byte:
    midiByte = (unsigned char)Serial.read();
    byteReady = true;
  }
}
