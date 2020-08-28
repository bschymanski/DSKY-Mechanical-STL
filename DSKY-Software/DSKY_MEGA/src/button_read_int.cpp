#include <Arduino.h>
#include <button_read_int.h>
#include <main.h>
#include <neopixel.h>

void(* resetFunc) (void) = 0;//declare reset function at address 0

int commonPin = 3;
int buttonPins[] = {23,24,25,26,27,44,45,30,31,32,33,34,35,36,37,42,39,40,41};
int numpins = 19;

const short     keyNone                 = 20;
const short     keyVerb                 = 19;
const short     keyNoun                 = 1;
const short     keyPlus                 = 7;
const short     keyMinus                = 8;
const short     keyNumber0              = 9;
const short     keyNumber1              = 10;
const short     keyNumber2              = 11;
const short     keyNumber3              = 12;
const short     keyNumber4              = 13;
const short     keyNumber5              = 14;
const short     keyNumber6              = 15;
const short     keyNumber7              = 16;
const short     keyNumber8              = 17;
const short     keyNumber9              = 18;
const short     keyClear                = 6;
const short     keyProceed              = 2;
const short     keyRelease              = 5;
const short     keyEnter                = 3;
const short     keyReset                = 4;

unsigned long int lastFire = 0;

void press(int button) { // Our handler
  //Serial.println(button + 1);
  button = button+1;
  current_key = button;
  setLamp(yellow, lampNoAtt);
  //Serial.print("current_key ");
  //Serial.println(current_key);
  gotInterrupt = true;
  setLamp(yellow, lampSTBY);
  //Serial.print("gotInterrupt ");
  //Serial.println(gotInterrupt);
  //configureCommon();
  if (current_key == 4)
  {
    
  }
  //detachInterrupt(digitalPinToInterrupt(commonPin));
  switch (button) {
  case 1:
    current_key = keyNoun;
    break;
  case 2:
    current_key = keyProceed;
    break;
  case 3:
    current_key = keyEnter;
    break;
  case 4:
    current_key = keyReset;
    break;
  case 5:
    current_key = keyRelease;
    break;
  case 6:
    current_key = keyClear;
    break;
  case 7:
    current_key = keyPlus;
    break;
  case 8:
    current_key = keyMinus;
    break;
  case 9:
    current_key = keyNumber0;
    break;
  case 10:
    current_key = keyNumber1;
    break;
  case 11:
    current_key = keyNumber2;
    break;
  case 12:
    current_key = keyNumber3;
    break;    
  case 13:
    current_key = keyNumber4;
    break;
  case 14:
    current_key = keyNumber5;
    break;
  case 15:
    current_key = keyNumber6;
    break;
  case 16:
    current_key = keyNumber7;
    break;
  case 17:
    current_key = keyNumber8;
    break;
  case 18:
    current_key = keyNumber9;
    break;
  case 19:
    current_key = keyVerb;
    break;
  default:
    current_key = keyNone;
    break; // Wird nicht benötigt, wenn Statement(s) vorhanden sind
}
  //current_key = button +1;
}

void configureDistinct() {
  //if (millis() - lastFire < 300) { // Debounce
  //  delayMicroseconds(100);
  //}
  //lastFire = millis();
  //Serial.println("configureDistinct entered");
  //detachInterrupt(digitalPinToInterrupt(commonPin));
  pinMode(commonPin, OUTPUT);
  digitalWrite(commonPin, LOW);

  for (int i = 0; i < numpins; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
}

void configureCommon() {
  pinMode(commonPin, INPUT_PULLUP);

  for (int i = 0; i < numpins; i++) {
    pinMode(buttonPins[i], OUTPUT);
    digitalWrite(buttonPins[i], LOW);
  }
}

void pressInterrupt() { // ISR
  setLamp(yellow, lampKeyRelease);
  //Serial.print("ISR attached ");
  //configureCommon();
  //gotInterrupt = true;
  
  //readkeys();
  if (micros() - lastFire < 20000) { // Debounce
    return;
  }
  lastFire = micros();
  interrupts ();
  configureDistinct(); // Setup pins for testing individual buttons
    for (int i = 0; i < numpins; i++) { // Test each button for press
    if (!digitalRead(buttonPins[i])) {
      press(i);
      
    }
  }
  configureCommon(); // Return to original state */
}
void readkeys()
{
  //Serial.println("readkeys entered");
  if (millis() - lastFire < 1) { // Debounce
    return;
  }
  lastFire = millis();
  //configureDistinct();
  //Serial.println("readkeys loop");
  for (int i = 0; i < numpins; i++) { // Test each button for press
    if (!digitalRead(buttonPins[i])) {
      //Serial.println("readkeys loop press(i) should be printed");
      press(i);
    }
  }
  configureCommon(); // Return to original state
  //gotInterrupt = false;
}

