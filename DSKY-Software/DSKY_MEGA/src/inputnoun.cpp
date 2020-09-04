#include <Arduino.h>
#include <main.h>
#include <button_read_int.h>
#include <neopixel.h>
#include <nextion.h>
#include <inputverb.h>

void clearNounfunction()
{
  noun_valid = false;
  verb = 0;
  noun = 0;
  verb_0 = -1;
  verb_1 = -1;
  noun_0 = -1;
  noun_1 = -1;
  action = action_none;
  executeAction = false;
  lookupAction = false;
  verb_error = false;
  noun_error = false;
  //clipnum = 1;
  printVerb(verb, false);
  printNoun(noun, false);
  printProg(prog, false);
}

void inputNoun()
{
  if ((noun_0 >= 0) && (noun_1 >= 0) && (noun_valid != true) && (noun_error !=true) && (current_key == keyRelease)) // we entered both noun_0 and noun_1, but want to cancel anyway, pressed keyRelease
  {
    clearNounfunction();
    clearVerbfunction();
    lightNounlamp(green);
    lightVerblamp(green);
    mode = modeIdle;
  }
  else if ((noun_0 >= 0) && (noun_1 >= 0) && (noun_valid != true) && (noun_error !=true) ) // we might possibly have a valid noun! Lets check it out
  {
    noun_temp = (noun_0*10)+noun_1;
    short idx = 0;
    bool found = false;
    while ((idx < NUM_PROG_TABLE_ENTRIES) && (found == false))
        { // lets have a look at the Programtable for a valid verb
            if (ProgramTable[idx].NounNumber == noun_temp)
            {
                old2_noun = old1_noun;
                old1_noun = noun;
                noun = noun_temp;
                noun_valid = true;
                found = true;
                if (noun != old1_noun)
                { // a new noun has been entered, but we had a valid noun before, now the old noun is invalid
                }
                //mode = modeIdle;
                verb_0 = -1;
                verb_1 = -1;
                noun_0 = -1;
                noun_1 = -1;
                lightNounlamp(green);
                printNoun(noun,false);
                switch(current_key)
                {
                  case keyEnter:
                    //setLamp(yellow, lampProgCond);
                    mode = modeIdle;
                    break;
                }
                //setLamp(blue, lampVerb);
            }
            idx++;
        }
        if (noun_valid == false)
        { // we don't have a valid verb, sorry. Turn on the error lamp and clear verb_0 - noun_1 variables
          setLamp(white, lampOprErr);
          noun_error = true;
        }
  }
  else if ((noun_valid != true) && (noun_error !=true)) // we don't have a valid noun yet
  {
    lightNounlamp(yellow);
    switch(current_key)
    {
      case keyVerb:
        clearNounfunction();
        break;
      case keyNoun: // if we are in Noun input mode, why press the noun button?
        break;
      case keyRelease:  // we changed our mind and don't want to input a noun, back to idle mode
        if ((noun_0 <= 0) && (noun_1 <= 0) && (noun_valid != true) && (noun_error !=true))
        {
          mode = modeIdle;
          clearNounfunction();
          lightNounlamp(green);
        }
        else if ((noun_valid != true) && (noun_error !=true))
        {
          mode = modeIdle;
          clearNounfunction();
          printNoun(noun, false);
          lightNounlamp(green);
        }
        break;
      case keyClear:
        if ((noun_0 >= 0) && (noun_1 <= 0) && (noun_valid != true) && (noun_error !=true))
        {
          noun_0 = -1;
          printNoun0(noun_0, false);
        }
        else if ((noun_0 >= 0) && (noun_1 >= 0) && (noun_valid != true) && (noun_error !=true))
        {
          noun_1 = -1;
          printNoun1(noun_1, false);
        }
        break;
      // Now we are entering the numbers for the noun!
      case keyNumber0:  
      case keyNumber1:
      case keyNumber2:
      case keyNumber3:
      case keyNumber4:
      case keyNumber5:
      case keyNumber6:
      case keyNumber7:
      case keyNumber8:
      case keyNumber9:
        if (noun_valid == false)
        { // we don't have a valid noun yet, but we have entered a number, lets assess it to the proper place (first or second Digit)
          if (noun_0 < 0)
          { // First Noun Digit has been entered
            noun_0 = current_key_int;
            printNoun0(noun_0, false);
          }
          else if ((noun_0 >= 0) && (noun_1 < 0))
          { // Second Noun Digit has been entered
            noun_1 = current_key_int;
            printNoun1(noun_1, false);
          }
        }
        break;
    }
  }
  else if ((noun_valid != true) && (noun_error ==true)) // we do not have a valid noun yet, a wrong noun has been entered
  {
    lightNounlamp(red);
    switch(current_key)
    {
      case keyClear:
        noun_0 = -1;
        noun_1 = -1;
        printNoun0(noun_0, false);
        printNoun1(noun_1, false);
        setLamp(off, lampOprErr);
        noun_error = false;
        lightNounlamp(yellow);  // we cleared the wrong noun_0 and noun_1, but are still in verbinputmode
        break;
    }
  }
}

