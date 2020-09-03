#include <Arduino.h>
#include <main.h>
#include <button_read_int.h>
#include <wire.h>
#include <neopixel.h>
#include <nextion.h>
#include <inputverb.h>
#include <inputnoun.h>


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
volatile int current_key_int = 0;
volatile bool gotInterrupt = false;
volatile bool keypressed = false;
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

void countregister()  // simple Idlefunction that counts on the registers
{
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
}

#define DEBUG 1

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
  #ifdef DEBUG
    Serial2.begin(9600);
    Serial2.print("baud=115200");
    Serial2.write(0xff);  //Send this three lines after each command sent to the nextion display.
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.end();
    Serial2.begin(115200);
  #endif
  nextion_main_page();
  printRegister(1, 0, true, false, false);
  printRegister(2, 23111, true, false, false);
  printRegister(3, -222, true, false, false);
  lightVerblamp(green);
  lightNounlamp(green);
  lightCompActy(off);
  printVerb(verb, false);
  printNoun(noun, false);
  printProg(prog, false);
}

void loop() {
  // Empty!
  if (gotInterrupt == true)
  {
    //setLamp(green, lampTemp);
    gotInterrupt = false;
  }
  else if (gotInterrupt == false) // Main Program starts here
  {
    //if (millis() - mainLoopDelay < 50)  // Little Mainloop Delay
    //{
    //  return;
    //}
    //mainLoopDelay = millis();
    // End Little Mainloop Delay

    setLamp(off, lampTemp);
    setLamp(off, lampUplinkActy);
    setLamp(off, lampNoAtt);
    setLamp(off, lampSTBY);
    setLamp(off, lampKeyRelease);
    setLamp(off, lampGimbalLock);
    setLamp(off, lampVel);
    if (keypressed == true) // a new key has been pressed, save
    {
      setLamp(off, lampPosition);
      setLamp(off, lampPosition);
      setLamp(off, lampPosition);
      switch (mode)  // Switch Modes
      {
        case modeIdle:  // IdleMode, DSKY waits for input
          // only new valid key in idle mode would be the VERB key
          if (current_key == keyVerb)
          {
            mode = modeInputVerb;
            inputVerb();
            break;
          }
          // switch current_key
          break;
        case modeInputVerb:
          inputVerb();
          break;
        case modeInputNoun:
          inputNoun();
          break;

        default:
          break;
      }
      // end switch mode()
      old_key = current_key;
      keypressed = false;
    }
    // end a new key has been pressed, save
    else if (keypressed == false)
    {
      switch (mode)
      {
        case modeIdle:  // IdleMode, DSKY waits for input
          countregister();
          break;
        case modeInputVerb:
          //lightVerblamp(blue);
          //printVerb(verb,false);
          countregister();
          break;
        case modeInputNoun:
          countregister();
          break;
        default:
          break;
      }
    }

    

  }  // END Main Program starts here
  //delay(00);
  #ifdef DEBUG
  nextion_debug();
  #endif
}

