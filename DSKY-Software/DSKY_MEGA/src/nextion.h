#ifndef NEXTION_H
#define NEXTION_H

//extern int current_key;
//extern int old_key;
extern void printRegister(int dregister, long number = 0, bool leadzero = true, bool blink = false, bool alarm = false);
extern void printProg(int prog, bool blink = false);
extern void printVerb(int verb, bool blink = false);
extern void printNoun(int noun, bool blink = false);
extern void printChar(int dregister, int digit, char character,  bool blink = false, bool alarm = false, bool dp = false);
extern void clearRegister(int dregister);
extern void nextion_debug_page(void);
extern void nextion_main_page(void);

#endif