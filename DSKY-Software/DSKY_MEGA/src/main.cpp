#include <Arduino.h>
#include <main.h>
#include <button_read_int.h>
#include <wire.h>
#include <neopixel.h>
#include <nextion.h>


ProgramStruct ProgramTable[] =
{
	/*VerbNumber            NounNumber            action                        ProgramNumber                     Description */
	{ verbLampTest,         nounNotUsed,          action_LampTest,              programNotUsed                },  /* V35E N--     - Bulb test */
	{ verbDisplayDecimal,   nounIMUAttitude,      action_displayIMUAttitude,    programNotUsed                },  /* V16  N17 E  - Action: IMUAttitude */
  { verbDisplayDecimal,   nounClockTime,        action_displayRealTimeClock,  programNotUsed                },  /* V16  N36 E  - Action: Display Time : actionReadTime()*/
  { verbDisplayDecimal,   nounLatLongAltitude,  action_displayGPS,            programNotUsed                },  /* V16  N43 E  - Action: Display Lattitude / Longitude / Altidue : actionReadGPS() */
  { verbDisplayDecimal,   nounGPSTime,          action_displayGPSTime,        programNotUsed                },  /* V16  N38 E  - Action: Display GPS Time : actionReadGPSTime() */
  { verbDisplayDecimal,   nounIMUgyro,          action_displayIMUGyro,        programNotUsed                },  /* V16  N18 E  - Action: IMUGyro */
  { verbDisplayDecimal,   nounSelectAudioclip,  action_PlayAudioclip,         programNotUsed                },  /* V16  98E   -  Action: Play Selected AudioClip : actionPlaySelectedAudioclip(int clipnum)*/
  { verbInputProg,        nounNotUsed,          action_none,                  programDispTimeDate           },  /* V37  20E    - Program: Display Date / Month / Time : progDispTimeDate()*/ 
  { verbInputProg,        nounNotUsed,          action_none,                  programSetDateMan             },  /* V37  21E*/
  { verbInputProg,        nounNotUsed,          action_none,                  programSetTimeGPS             },  /* V37  22E*/
  { verbInputProg,        nounNotUsed,          action_none,                  programSetDateGPS             },  /* V37  23E*/
  { verbInputProg,        nounNotUsed,          action_none,                  programSetDebugEEPROM         },  /* V37  24E*/
  { verbInputProg,        nounNotUsed,          action_none,                  programSetColormodeEEPROM     },  /* V37  24E*/
  { verbInputProg,        nounNotUsed,          action_none,                  programJFKAudio               },  /* V37  62E*/
  { verbInputProg,        nounNotUsed,          action_none,                  programApollo11Audio          },  /* V37  69E*/
  { verbInputProg,        nounNotUsed,          action_none,                  programApollo13Audio          },  /* V37  70E*/
  { verbInputNumber,      nounClockTime,        action_setTime,               programNotUsed                },  /* V21  36E   - Action: Set Time : actionSetTime() */
  { verbInputNumber,      nounGPSTime,          action_setGPSTime,            programNotUsed                },  /* V21  38E   - Action: Set GPS Time : actionSetGPSTime() */
  { verbInputNumber,      nounSelectAudioclip,  action_SelectAudioclip,       programNotUsed                },  /* V21  98E   - Action: Select AudioClip : aactionSelectAudioclip() actionPlaySelectedAudioclip(int clipnum)*/
  { verbDisplayDecimal,   nounIMUgyro,          action_displayIMUGyro,        programNotUsed                }   /* V16E  N18 E - Display IMUGyro */
};

volatile int current_key = 0;
volatile bool gotInterrupt = false;
int old_key = 0;
unsigned long int mainLoopDelay = 0;
unsigned long int mainLoopISRDelay = 10000;

// initialisation of Mode-, Verb, Noun, Program and Action Modes
// current Mode-, Verb, Noun, Program and Action Values
short mode    = modeIdle;     // eq 0
short verb    = verbNone;     // eq 0
short noun    = verbNone;     // eq 0
short prog    = programNone;  // eq 0
short action  = action_none;  // eq 0

bool actionrunning = false;
bool programrunning = false;

// old Mode-, Verb, Noun, Program and Action Values
short old1_mode    = modeIdle;     // eq 0
short old1_verb    = verbNone;     // eq 0
short old1_noun    = verbNone;     // eq 0
short old1_prog    = programNone;  // eq 0
short old1_action  = action_none;  // eq 0

// before old Mode-, Verb, Noun, Program and Action values
short old2_mode    = modeIdle;     // eq 0
short old2_verb    = verbNone;     // eq 0
short old2_noun    = verbNone;     // eq 0
short old2_prog    = programNone;  // eq 0
short old2_action  = action_none;  // eq 0

bool verb_valid = false;
bool noun_valid = false;
bool prog_valid = false;

short verb_0 = -1;
short verb_1 = -1;

short noun_0 = -1;
short noun_1 = -1;

short prog_0 = -1;
short prog_1 = -1;

short verb_temp = -1;
short noun_temp = -1;
short prog_temp = -1;

bool verb_error = false;
bool noun_error = false;
bool prog_error = false;

short NUM_PROG_TABLE_ENTRIES = (sizeof(ProgramTable)/sizeof(ProgramStruct));

bool blink = false;
bool toggledprogblink = false;
bool blinkprog = false;
bool toggled = false;
bool toggled500 = false;
bool toggled250 = false;
bool toggledverbblink = false;
bool gpsreadtoggle = true;
bool blinkgpstime = false;
bool blinkgpstimetoggled = false;

short gps_hour = 0;
short gps_minute = 0;
short gps_second = 0;

bool blinkverb = true;
bool blinknoun = true;
bool imutoggle = true;
bool printregtoggle = true;
bool uplink_compact_toggle = true;

bool global_state_1sec = true;
bool global_state_500 = true;
bool global_state_250 = true;
bool toggle500 = true;
bool toggle250 = true;
bool toggle = true;
bool printed = true;

bool executeAction = false;
bool lookupAction = false;
bool executeProgram = false;
bool lookupProgram = false;

long IncDecNumber = 0;
bool IncDecNumber_done =false;

long loopincrement_r1 = 0;
long loopincrement_r2 = 0;
long loopincrement_r3 = 0;

void setup() {
  configureCommon(); // Setup pins for interrupt
  attachInterrupt(digitalPinToInterrupt(commonPin), pressInterrupt, FALLING);
  //configureDistinct();
  NeoPixelSetup();
  setLamp(yellow, lampVel);
  setLamp(yellow, lampAlt);
  setLamp(yellow, lampTracker);
  setLamp(yellow, lampRestart);
  setLamp(yellow, lampProgCond);
  setLamp(yellow, lampGimbalLock);
  setLamp(yellow, lampTemp);
  setLamp(white, lampClk);
  setLamp(white, lampPosition);
  setLamp(white, lampOprErr);
  setLamp(white, lampKeyRelease);
  setLamp(white, lampSTBY);
  setLamp(white, lampNoAtt);
  setLamp(white, lampUplinkActy);
  delay(100);
  setLamp(off, lampVel);
  setLamp(off, lampAlt);
  setLamp(off, lampTracker);
  setLamp(off, lampRestart);
  setLamp(off, lampProgCond);
  setLamp(off, lampGimbalLock);
  setLamp(off, lampTemp);
  setLamp(off, lampClk);
  setLamp(off, lampPosition);
  setLamp(off, lampOprErr);
  setLamp(off, lampKeyRelease);
  setLamp(off, lampSTBY);
  setLamp(off, lampNoAtt);
  setLamp(off, lampUplinkActy);
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial1.print("baud=115200");
  Serial1.write(0xff);  //Send this three lines after each command sent to the nextion display.
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.end();
  Serial1.begin(115200);
  nextion_main_page();
  printRegister(1, 0, true, false, false);
  printRegister(2, 23111, true, false, false);
  printRegister(3, -222, true, false, false);
}

void loop() {
  // Empty!
  if (gotInterrupt == true)
  {
    //readkeys();
    setLamp(green, lampTemp);
    //Serial.print("gotInterrupt ");
    //Serial.println(gotInterrupt);
    //Serial.println("key pressed");
    //delay(100);
    //configureCommon();
    //sei ();
    //attachInterrupt(digitalPinToInterrupt(commonPin), pressInterrupt, FALLING);
    //delay(100);
    //detachInterrupt(digitalPinToInterrupt(commonPin));
    //configureCommon();
    //sei ();
    //attachInterrupt(digitalPinToInterrupt(commonPin), pressInterrupt, FALLING);
    //sei ();
    gotInterrupt = false;
    //Serial.print("gotInterrupt ");
    //Serial.println(gotInterrupt);
  }
  else if (gotInterrupt == false) // Main Program starts here
  {
    if (millis() - mainLoopDelay < 100) { // Debounce
      return;
      
    }
    //configureCommon();
    //sei ();
    mainLoopDelay = millis();
    setLamp(off, lampTemp);
    setLamp(off, lampUplinkActy);
    setLamp(off, lampNoAtt);
    setLamp(off, lampSTBY);
    setLamp(off, lampKeyRelease);
    //Serial.print("gotInterrupt ");
    //Serial.println(gotInterrupt);
    //Serial.println("Loop");
    //Serial.print("old_key     : ");
    //Serial.println(old_key);
    //Serial.print("current_key : ");
    //Serial.println(current_key);
    if (current_key != 0)
    {
      setLamp(blue, lampAlt);
    }
    if (current_key != old_key) // a new key has been pressed, save
    {
      old_key = current_key;
      //Serial.print("A new Key has been pressed! : ");
      setLamp(yellow, lampUplinkActy);
      setLamp(red, lampGimbalLock);

      //Serial.println(current_key);
      setLamp(yellow, lampVel);
      switch (current_key) {
        case 1:
          current_key = keyNoun;
          setLamp(red, lampVel);
          //Serial.print("NOUN ");
          //Serial.println(current_key);
          break;
        case 2:
          current_key = keyProceed;
          setLamp(red, lampVel);
          //Serial.print("PRO ");
          //Serial.println(current_key);
          break;
        case 3:
          current_key = keyEnter;
          setLamp(red, lampVel);
          //Serial.print("ENTR ");
          //Serial.println(current_key);
          break;
        case 4:
          current_key = keyReset;
          setLamp(red, lampVel);
          //Serial.print("RSET ");
          //Serial.println(current_key);
          break;
        case 5:
          current_key = keyRelease;
          setLamp(red, lampVel);
          //Serial.print("KEY REL ");
          //Serial.println(current_key);
          break;
        case 6:
          current_key = keyClear;
          setLamp(red, lampVel);
          //Serial.print("CLR ");
          //Serial.println(current_key);
          break;
        case 7:
          current_key = keyPlus;
          setLamp(red, lampVel);
          //Serial.print("+ ");
          //Serial.println(current_key);
          break;
        case 8:
          current_key = keyMinus;
          setLamp(red, lampVel);
          //Serial.print("- ");
          //Serial.println(current_key);
          break;
        case 9:
          current_key = keyNumber0;
          setLamp(red, lampVel);
          //Serial.print("0 ");
          //Serial.println(current_key);
          break;
        case 10:
          current_key = keyNumber1;
          setLamp(red, lampVel);
          //Serial.print("1 ");
          //Serial.println(current_key);
          break;
        case 11:
          current_key = keyNumber2;
          setLamp(red, lampVel);
          //Serial.print("2 ");
          //Serial.println(current_key);
          break;
        case 12:
          current_key = keyNumber3;
          setLamp(red, lampVel);
          //Serial.print("3 ");
          //Serial.println(current_key);
          break;    
        case 13:
          current_key = keyNumber4;
          setLamp(red, lampVel);
          //Serial.print("4 ");
          //Serial.println(current_key);
          break;
        case 14:
          current_key = keyNumber5;
          setLamp(red, lampVel);
          //Serial.print("5 ");
          //Serial.println(current_key);
          break;
        case 15:
          current_key = keyNumber6;
          setLamp(red, lampVel);
          //Serial.print("6 ");
          //Serial.println(current_key);
          break;
        case 16:
          current_key = keyNumber7;
          setLamp(red, lampVel);
          //Serial.print("7 ");
          //Serial.println(current_key);
          break;
        case 17:
          current_key = keyNumber8;
          setLamp(red, lampVel);
          //Serial.print("8 ");
          //Serial.println(current_key);
          break;
        case 18:
          current_key = keyNumber9;
          setLamp(red, lampVel);
          //Serial.print("9 ");
          //Serial.println(current_key);
          break;
        case 19:
          current_key = keyVerb;
          setLamp(red, lampVel);
          //Serial.print("VERB ");
          //Serial.println(current_key);
          break;
        default:
          current_key = keyNone;
          setLamp(red, lampVel);
          //Serial.print("NONE ");
          //Serial.println(current_key);
          break; // Wird nicht benötigt, wenn Statement(s) vorhanden sind
      }
      if (current_key == old_key)
      {
        setLamp(off, lampVel);
        setLamp(green, lampGimbalLock);
        //detachInterrupt(digitalPinToInterrupt(commonPin));
        //configureCommon();
        //sei ();
        //attachInterrupt(digitalPinToInterrupt(commonPin), pressInterrupt, FALLING);
        //configureCommon();
        //sei ();
        //attachInterrupt(digitalPinToInterrupt(commonPin), pressInterrupt, FALLING);
        //detachInterrupt(digitalPinToInterrupt(commonPin));
        //configureCommon();
        //sei ();
        //attachInterrupt(digitalPinToInterrupt(commonPin), pressInterrupt, FALLING);
        //sei ();
      }
      if (mainLoopISRDelay > 10000) { // Debounce
        setLamp(yellow, lampVel);
        //Serial.print("Interrupt neu attachet nach 10000 Sekunden");
        //configureCommon();
        //sei ();
        //attachInterrupt(digitalPinToInterrupt(commonPin), pressInterrupt, FALLING);

        return;
        }
      
    }
    if (loopincrement_r1 < 100000 && loopincrement_r2 < 100000 && loopincrement_r3 < 100000)
    {
      loopincrement_r1++;
      printRegister(1, loopincrement_r1, true, false, false);
      printRegister(2, loopincrement_r2, true, false, false);
      printRegister(3, loopincrement_r3, true, false, false);
    }
    else if (loopincrement_r1 >= 100000 && loopincrement_r2 < 100000 && loopincrement_r3 < 100000)
    {
      loopincrement_r1 = 0;
      loopincrement_r2++;
      printRegister(1, loopincrement_r1, true, false, false);
      printRegister(2, loopincrement_r2, true, false, false);
      printRegister(3, loopincrement_r3, true, false, false);
    }
    else if (loopincrement_r1 < 100000 && loopincrement_r2 >= 100000 && loopincrement_r3 < 100000)
    {
      loopincrement_r2 = 0;
      loopincrement_r3++;
      printRegister(1, loopincrement_r1, true, false, false);
      printRegister(2, loopincrement_r2, true, false, false);
      printRegister(3, loopincrement_r3, true, false, false);
    }
    else if (loopincrement_r1 < 100000 && loopincrement_r2 < 100000 && loopincrement_r3 >= 100000)
    {
      loopincrement_r3 = 0;
      printRegister(1, loopincrement_r1, true, false, false);
      printRegister(2, loopincrement_r2, true, false, false);
      printRegister(3, loopincrement_r3, true, false, false);
    }
    
    //nextion_debug_page();

  }  // END Main Program starts here
  //delay(00);
}

