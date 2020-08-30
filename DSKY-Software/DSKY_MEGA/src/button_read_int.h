#ifndef BUTTON_READ_INT_H
#define BUTTON_READ_INT_H

extern int commonPin;
extern int buttonPins[];

extern unsigned long lastFire;

extern void press(int button);
extern void configureCommon();
extern void pressInterrupt();
extern void readkeys();
extern void configureDistinct();

const short   keyNone                 = 20;
const short   keyVerb                 = 19;
const short   keyNoun                 = 1;
const short   keyPlus                 = 7;
const short   keyMinus                = 8;
const short   keyNumber0              = 9;
const short   keyNumber1              = 10;
const short   keyNumber2              = 11;
const short   keyNumber3              = 12;
const short   keyNumber4              = 13;
const short   keyNumber5              = 14;
const short   keyNumber6              = 15;
const short   keyNumber7              = 16;
const short   keyNumber8              = 17;
const short   keyNumber9              = 18;
const short   keyClear                = 6;
const short   keyProceed              = 2;
const short   keyRelease              = 5;
const short   keyEnter                = 3;
const short   keyReset                = 4;
#endif