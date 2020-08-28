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

extern int     keyNone;
extern int     keyVerb;
extern int     keyNoun;
extern int     keyPlus;
extern int     keyMinus;
extern int     keyNumber0;
extern int     keyNumber1;
extern int     keyNumber2;
extern int     keyNumber3;
extern int     keyNumber4;
extern int     keyNumber5;
extern int     keyNumber6;
extern int     keyNumber7;
extern int     keyNumber8;
extern int     keyNumber9;
extern int     keyClear;
extern int     keyProceed;
extern int     keyRelease;
extern int     keyEnter;
extern int     keyReset;
#endif