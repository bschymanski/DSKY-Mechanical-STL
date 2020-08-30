#include <Arduino.h>
#include <main.h>
#include <button_read_int.h>
#include <neopixel.h>
#include <nextion.h>


void clearVerbfunction()
{
  verb_valid = false;
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

void inputVerb()
{
  int temporaryKey = current_key;
  if ((verb_0 >= 0) && (verb_1 >= 0) && (verb_valid != true) && (verb_error !=true)) // we might possibly have a valid verb! Lets check it out
  {
    lightVerblamp(blue);
    switch(temporaryKey)
    {
      case keyRelease:  // we changed our mind and don't want to input a verb, back to idle mode
        if ((verb_0 <= 0) && (verb_1 <= 0) && (verb_valid != true) && (verb_error !=true))
        {
          mode = modeIdle;
          verb_0 = -1;
          verb_1 = -1;
          printVerb(verb, false);
          lightVerblamp(green);
        }
        else if ((verb_valid != true) && (verb_error !=true))
        {
          mode = modeIdle;
          verb_0 = -1;
          verb_1 = -1;
          printVerb(verb, false);
          lightVerblamp(green);
        }
        break;
    }
  }
  else if ((verb_valid != true) && (verb_error !=true)) // we do not have a valid verb yet
  {
    lightVerblamp(yellow);
    switch(temporaryKey)
    {
      case keyVerb:
        clearVerbfunction();
        break;
      case keyNoun: // if we are in verbinput mode, why press the noun button, makes sense only if we already have a valid verb.
        if (verb_valid == true)
        {
            mode = modeInputNoun;
        }
        break;
      case keyRelease:  // we changed our mind and don't want to input a verb, back to idle mode
        if ((verb_0 <= 0) && (verb_1 <= 0) && (verb_valid != true) && (verb_error !=true))
        {
          mode = modeIdle;
          lightVerblamp(green);
        }
        else if ((verb_valid != true) && (verb_error !=true))
        {
          mode = modeIdle;
          verb_0 = -1;
          verb_1 = -1;
          printVerb(verb, false);
          lightVerblamp(green);
        }
        break;
      // Now we are entering the numbers for the verb!
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
        if (verb_valid == false)
        { // we don't have a valid verb yet, but we have entered a number, lets assing it to the proper place (first or second Digit)
          if (verb_0 < 0)
          { // First Verb Digit has been entered
            verb_0 = current_key_int;
            printVerb0(verb_0, false);
          }
          else if ((verb_0 >= 0) && (verb_1 < 0))
          { // Second verb Digit has been entered
            verb_1 = current_key_int;
            printVerb1(verb_1, false);
          }
        }
        break;
    }
  }
  else if ((verb_valid != true) && (verb_error ==true)) // we do not have a valid verb yet, a wrong verb has been entered
  {
    lightVerblamp(red);
  }
  else if ((verb_valid == true) && (noun_valid != true)) // we have a valid verb and now lets see what we comes next: Lamptest, Program Input, Verb input?
  {
    lightVerblamp(green);
  }
  else if ((verb_valid == true) && (noun_valid == true)) // we have a valid verb and a valid noun, but we want to enter a new verb, so the old noun is invalid now
  {
    lightVerblamp(yellow);
  }
}

