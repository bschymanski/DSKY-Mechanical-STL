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

extern const short     keyNone;
extern const short     keyVerb;
extern const short     keyNoun;
extern const short     keyPlus;
extern const short     keyMinus;
extern const short     keyNumber0;
extern const short     keyNumber1;
extern const short     keyNumber2;
extern const short     keyNumber3;
extern const short     keyNumber4;
extern const short     keyNumber5;
extern const short     keyNumber6;
extern const short     keyNumber7;
extern const short     keyNumber8;
extern const short     keyNumber9;
extern const short     keyClear;
extern const short     keyProceed;
extern const short     keyRelease;
extern const short     keyEnter;
extern const short     keyReset;
#endif