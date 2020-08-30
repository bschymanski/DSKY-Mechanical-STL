#include <nextion.h>
#include <Arduino.h>
#include <main.h>
#include <neopixel.h>

String fieldname = " ";
void nextion_enter(int serialport)
{
  if (serialport == 1)
  {
    Serial1.write(0xff);
    Serial1.write(0xff);
    Serial1.write(0xff);
  }
  else if (serialport ==2)
  {
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
  }
}

void nextion_print_REG(int dregister, int place, int number)  // not in use, use printRegister()
{
    Serial1.print("R");
    Serial1.print(dregister);
    Serial1.print("N");
    Serial1.print(place);
    Serial1.print(".txt=\"");
    Serial1.print(number);
    Serial1.print("\"");
    Serial1.write(0xff);  //Send this three lines after each command sent to the nextion display.
    Serial1.write(0xff);
    Serial1.write(0xff);
    
}

void nextion_print_REGOFF(int dregister, int place)
{
    Serial1.print("R");
    Serial1.print(dregister);
    Serial1.print("N");
    Serial1.print(place);
    Serial1.print(".txt=\" \"");
    Serial1.write(0xff);  //Send this three lines after each command sent to the nextion display.
    Serial1.write(0xff);
    Serial1.write(0xff);
    
}


void nextion_print_REGPLUS(int dregister)
{
    Serial1.print("R");
    Serial1.print(dregister);
    Serial1.print("sign.txt=\"+\"");
    Serial1.write(0xff);  //Send this three lines after each command sent to the nextion display.
    Serial1.write(0xff);
    Serial1.write(0xff);
    
}
void nextion_print_REGMINUS(int dregister)
{
    Serial1.print("R");
    Serial1.print(dregister);
    Serial1.print("sign.txt=\"-\"");
    Serial1.write(0xff);  //Send this three lines after each command sent to the nextion display.
    Serial1.write(0xff);
    Serial1.write(0xff);
    
}

void printRegister(int dregister, long number, bool leadzero = true, bool blink = false, bool alarm)
{
  if (leadzero == true)
    bool number_positive = true; 
    int one = 0;
    int ten = 0;
    int hundred = 0;
    long thousand = 0;
    long tenthousand = 0;
    bool number_positive = true;
    // first, check if the number is positive or negative and set the plus or minus sign
    if (number < 0)
    {
        number = -number;
        number_positive = false;
    }
    else 
    {
        // Set the plus sign
        //SevenSeg.setRow(dregister, 0, B01110100);
        number_positive = true;
    }
    // now seperate the number
    if (number == 0)
    {
        one = int(number);
    }
    else if ((number > 0) && (number < 10))
    {
        one = int(number);
    }
    else if ((number >= 10) && (number < 100))
    {   
        one = number % 10;
        ten = (number - one) / 10;
    }
    else if ((number >= 100) && (number < 1000))
    {
        one = number % 10;
        ten = (number / 10) % 10;
        hundred = (number / 100) % 10;

    }
    else if ((number >= 1000) && (number < 10000))
    {
        one = number % 10;
        ten = (number / 10) % 10;
        hundred = (number / 100) % 10;
        thousand = (number / 1000) % 10;
    }
    else if ((number >= 10000) && (number < 100000))
    {
        one = number % 10;
        ten = (number / 10) % 10;
        hundred = (number / 100) % 10;
        thousand = (number / 1000) % 10;
        tenthousand = (number / 10000) % 10;
    }
    // show the number
    if (blink == false)
    {
        if (number >= 100000)
        {
            nextion_print_REG(dregister, 5, " ");
            nextion_print_REG(dregister, 4, 1);
            nextion_print_REG(dregister, 3, 3);
            nextion_print_REG(dregister, 2, 0);
            nextion_print_REG(dregister, 1, 3);
        }
        else 
        {
            if (number_positive == true)
            {
              nextion_print_REGPLUS(dregister);
            }
            else
            {
              nextion_print_REGMINUS(dregister);
            }
            
            nextion_print_REG(dregister,1, one);
            nextion_print_REG(dregister,2, ten);
            nextion_print_REG(dregister,3, hundred);
            nextion_print_REG(dregister,4, thousand);
            nextion_print_REG(dregister,5, tenthousand);
        }
    }
    if (blink == true)
    {
        nextion_print_REGOFF(dregister,1);
        nextion_print_REGOFF(dregister,2);
        nextion_print_REGOFF(dregister,3);
        nextion_print_REGOFF(dregister,4);
        nextion_print_REGOFF(dregister,5);
    }
  
}

void printProg(int prog, bool blink = false)
{
  int one = 0;
  int ten = 0;
  if (prog == programNone)
  {
    Serial1.print("P1");
    Serial1.print(".txt=\" \"");
    Serial1.write(0xff);  //Send this three lines after each command sent to the nextion display.
    Serial1.write(0xff);
    Serial1.write(0xff);
    Serial1.print("P2");
    Serial1.print(".txt=\" \"");
    Serial1.write(0xff);  //Send this three lines after each command sent to the nextion display.
    Serial1.write(0xff);
    Serial1.write(0xff);
  }
}

void printVerb(int verb, bool blink = false)
{
  if (verb == verbNone)
  {
    Serial1.print("V1");
    Serial1.print(".txt=\" \"");
    Serial1.write(0xff);  //Send this three lines after each command sent to the nextion display.
    Serial1.write(0xff);
    Serial1.write(0xff);
    Serial1.print("V2");
    Serial1.print(".txt=\" \"");
    Serial1.write(0xff);  //Send this three lines after each command sent to the nextion display.
    Serial1.write(0xff);
    Serial1.write(0xff);
  }
}
void printVerb0(int verb_0, bool blink = false)
{
  if (verb_0 >= 0)
  {
    Serial1.print("V1");
    Serial1.print(".txt=\"");
    Serial1.print(verb_0);
    Serial1.print("\"");
    Serial1.write(0xff);  //Send this three lines after each command sent to the nextion display.
    Serial1.write(0xff);
    Serial1.write(0xff);
  }
  else if (verb_0 < 0)
  {
    Serial1.print("V1");
    Serial1.print(".txt=\" \"");
    Serial1.write(0xff);  //Send this three lines after each command sent to the nextion display.
    Serial1.write(0xff);
    Serial1.write(0xff);
  }
}
void printVerb1(int verb_1, bool blink = false)
{
  if (verb_1 >= 0)
  {
    Serial1.print("V2");
    Serial1.print(".txt=\"");
    Serial1.print(verb_1);
    Serial1.print("\"");
    Serial1.write(0xff);  //Send this three lines after each command sent to the nextion display.
    Serial1.write(0xff);
    Serial1.write(0xff);
  }
  else if (verb_1 < 0)
  {
    Serial1.print("V2");
    Serial1.print(".txt=\" \"");
    Serial1.write(0xff);  //Send this three lines after each command sent to the nextion display.
    Serial1.write(0xff);
    Serial1.write(0xff);
  }
}


void printNoun(int noun, bool blink = false)
{
  int one = 0;
  int ten = 0;
  if (noun == nounNone)
  {
    Serial1.print("N1");
    Serial1.print(".txt=\" \"");
    Serial1.write(0xff);  //Send this three lines after each command sent to the nextion display.
    Serial1.write(0xff);
    Serial1.write(0xff);
    Serial1.print("N2");
    Serial1.print(".txt=\" \"");
    Serial1.write(0xff);  //Send this three lines after each command sent to the nextion display.
    Serial1.write(0xff);
    Serial1.write(0xff);
  }
}

void lightVerblamp(int color)
{
  switch (color)
  {
    case blue:
      Serial1.print("verblight.pco=BLUE");
      Serial1.write(0xff);  //Send this three lines after each command sent to the nextion display.
      Serial1.write(0xff);
      Serial1.write(0xff);
      break;
    case green:
      Serial1.print("verblight.pco=GREEN");
      Serial1.write(0xff);  //Send this three lines after each command sent to the nextion display.
      Serial1.write(0xff);
      Serial1.write(0xff);
      break;
    case yellow:
      Serial1.print("verblight.pco=YELLOW");
      Serial1.write(0xff);  //Send this three lines after each command sent to the nextion display.
      Serial1.write(0xff);
      Serial1.write(0xff);
      break;
    case red:
      Serial1.print("verblight.pco=RED");
      Serial1.write(0xff);  //Send this three lines after each command sent to the nextion display.
      Serial1.write(0xff);
      Serial1.write(0xff);
      break;
    case off:
      Serial1.print("verblight.pco=BLACK");
      Serial1.write(0xff);  //Send this three lines after each command sent to the nextion display.
      Serial1.write(0xff);
      Serial1.write(0xff);
      break;
    default:
      break;
  }
  
  
}

void lightCompActy(int color)
{
  switch (color)
  {
    case blue:
      Serial1.print("compactylight.pco=BLUE");
      Serial1.write(0xff);  //Send this three lines after each command sent to the nextion display.
      Serial1.write(0xff);
      Serial1.write(0xff);
      break;
    case green:
      Serial1.print("compactylight.pco=GREEN");
      Serial1.write(0xff);  //Send this three lines after each command sent to the nextion display.
      Serial1.write(0xff);
      Serial1.write(0xff);
      break;
    case yellow:
      Serial1.print("compactylight.pco=YELLOW");
      Serial1.write(0xff);  //Send this three lines after each command sent to the nextion display.
      Serial1.write(0xff);
      Serial1.write(0xff);
      break;
    case off:
      Serial1.print("compactylight.pco=BLACK");
      Serial1.write(0xff);  //Send this three lines after each command sent to the nextion display.
      Serial1.write(0xff);
      Serial1.write(0xff);
      break;
    default:
      break;
  }
  
  
}

void printREG(int dregister, int digit, char character,  bool blink = false, bool alarm = false)
{
  
}

void clearRegister(int dregister)
{

}

void nextion_debug_page()
{
    Serial1.print("page page1");
    Serial1.write(0xff);  //Send this three lines after each command sent to the nextion display.
    Serial1.write(0xff);
    Serial1.write(0xff);
    
}
void nextion_main_page()
{
    Serial1.print("page page2");
    Serial1.write(0xff);  //Send this three lines after each command sent to the nextion display.
    Serial1.write(0xff);
    Serial1.write(0xff);
    
}

void nextion_debug()
{
  //V0.val=1
  //V0.pco=BLUE

  // print Verb1 verb0 and verb
  Serial2.print("V0.val=");
  Serial2.print(verb_0);
  nextion_enter(2);
  if (verb_0 >= 0)
  {
    Serial2.print("V0.pco=BLUE");
    nextion_enter(2);
  }
  else
  {
    Serial2.print("V0.pco=RED");
    nextion_enter(2);
  }
  
  Serial2.print("V1.val=");
  Serial2.print(verb_1);
  nextion_enter(2);
  if (verb_1 >= 0)
  {
    Serial2.print("V1.pco=BLUE");
    nextion_enter(2);
  }
  else
  {
    Serial2.print("V1.pco=RED");
    nextion_enter(2);
  }

  Serial2.print("verb.val=");
  Serial2.print(verb);
  nextion_enter(2);
  if (verb > 0)
  {
    Serial2.print("verb.pco=BLUE");
    nextion_enter(2);
  }
  else
  {
    Serial2.print("verb.pco=RED");
    nextion_enter(2);
  }

  // Verb Valid?
  // vvalid.bco=RED  / GREEN
  if (verb_valid == true)
  {
    Serial2.print("vvalid.bco=GREEN");
    nextion_enter(2);
  }
  else
  {
    Serial2.print("vvalid.bco=RED");
    nextion_enter(2);
  }
  
    // print noun1 noun0 and noun
  Serial2.print("N0.val=");
  Serial2.print(noun_0);
  nextion_enter(2);
  if (noun_0 >= 0)
  {
    Serial2.print("N0.pco=BLUE");
    nextion_enter(2);
  }
  else
  {
    Serial2.print("N0.pco=RED");
    nextion_enter(2);
  }
  
  Serial2.print("N1.val=");
  Serial2.print(noun_1);
  nextion_enter(2);
  if (noun_1 >= 0)
  {
    Serial2.print("N1.pco=BLUE");
    nextion_enter(2);
  }
  else
  {
    Serial2.print("N1.pco=RED");
    nextion_enter(2);
  }

  Serial2.print("noun.val=");
  Serial2.print(noun);
  nextion_enter(2);
  if (noun > 0)
  {
    Serial2.print("noun.pco=BLUE");
    nextion_enter(2);
  }
  else
  {
    Serial2.print("noun.pco=RED");
    nextion_enter(2);
  }

  // Noun Valid?
  // vvalid.bco=RED  / GREEN
  if (noun_valid == true)
  {
    Serial2.print("nvalid.bco=GREEN");
    nextion_enter(2);
  }
  else
  {
    Serial2.print("nvalid.bco=RED");
    nextion_enter(2);
  }



  // Verb Valid?
  // vvalid.bco=RED  / GREEN
  if (verb_valid == true)
  {
    Serial2.print("vvalid.bco=GREEN");
    nextion_enter(2);
  }
  else
  {
    Serial2.print("vvalid.bco=RED");
    nextion_enter(2);
  }
  
  // print prog1 prog0 and prog
  Serial2.print("P0.val=");
  Serial2.print(prog_0);
  nextion_enter(2);
  if (prog_0 >= 0)
  {
    Serial2.print("P0.pco=BLUE");
    nextion_enter(2);
  }
  else
  {
    Serial2.print("P0.pco=RED");
    nextion_enter(2);
  }
  
  Serial2.print("P1.val=");
  Serial2.print(prog_1);
  nextion_enter(2);
  if (prog_1 >= 0)
  {
    Serial2.print("P1.pco=BLUE");
    nextion_enter(2);
  }
  else
  {
    Serial2.print("P1.pco=RED");
    nextion_enter(2);
  }

  Serial2.print("prog.val=");
  Serial2.print(prog);
  nextion_enter(2);
  if (prog > 0)
  {
    Serial2.print("prog.pco=BLUE");
    nextion_enter(2);
  }
  else
  {
    Serial2.print("prog.pco=RED");
    nextion_enter(2);
  }

  // Noun Valid?
  // vvalid.bco=RED  / GREEN
  if (prog_valid == true)
  {
    Serial2.print("pvalid.bco=GREEN");
    nextion_enter(2);
  }
  else
  {
    Serial2.print("pvalid.bco=RED");
    nextion_enter(2);
  }







  // Print mode
  // Loop Modes
  /*
  const short     modeIdle                    = 0;
  const short     modeInputVerb               = 1;
  const short     modeInputNoun               = 2;
  const short     modeInputProgram            = 3;
  const short     modeInputNumber             = 4;
  const short     modeLampTest                = 5;
  const short     modeExcuteAction            = 6;
  const short     modeExcuteProgram           = 7;
  const short     modeIncDecNumber            = 8;
  */
  Serial2.print("mode.txt=\"");
  switch (mode)
  {
    case 0:
    Serial2.print("modeidle");
    break;
    case 1:
    Serial2.print("modeInputVerb");
    break;
    case 2:
    Serial2.print("modeInputNoun");
    break;
    case 3:
    Serial2.print("modeInputProgram");
    break;
    case 4:
    Serial2.print("modeInputNumber");
    break;
    case 5:
    Serial2.print("modeLampTest");
    break;
    case 6:
    Serial2.print("modeExcuteAction");
    break;
    case 7:
    Serial2.print("modeExcuteProgram");
    break;
    case 8:
    Serial2.print("modeIncDecNumber");
    break;
  }
  Serial2.print("\"");
  nextion_enter(2);

  // old_key + current key
  /*
  int     keyNone                 = 20;
  int     keyVerb                 = 19;
  int     keyNoun                 = 1;
  int     keyPlus                 = 7;
  int     keyMinus                = 8;
  int     keyNumber0              = 9;
  int     keyNumber1              = 10;
  int     keyNumber2              = 11;
  int     keyNumber3              = 12;
  int     keyNumber4              = 13;
  int     keyNumber5              = 14;
  int     keyNumber6              = 15;
  int     keyNumber7              = 16;
  int     keyNumber8              = 17;
  int     keyNumber9              = 18;
  int     keyClear                = 6;
  int     keyProceed              = 2;
  int     keyRelease              = 5;
  int     keyEnter                = 3;
  int     keyReset                = 4;
  */
  Serial2.print("currentkey.val=");
  Serial2.print(current_key);
  nextion_enter(2);
  if (verb_0 >= 0)
  {
    Serial2.print("currentkey.pco=BLUE");
    nextion_enter(2);
  }
  else
   {
    Serial2.print("currentkey.pco=RED");
    nextion_enter(2);
  }
  Serial2.print("currentkeytxt.txt=\"");
  switch (current_key)
  {
    case 0:
    Serial2.print("keyNone");
    break;
    case 1:
    Serial2.print("keyNoun");
    break;
    case 2:
    Serial2.print("keyProceed");
    break;
    case 3:
    Serial2.print("keyEnter");
    break;
    case 4:
    Serial2.print("keyReset");
    break;
    case 5:
    Serial2.print("keyRelease");
    break;
    case 6:
    Serial2.print("keyClear");
    break;
    case 7:
    Serial2.print("keyPlus");
    break;
    case 8:
    Serial2.print("keyMinus");
    break;
    case 9:
    Serial2.print("keyNumber0");
    break;
    case 10:
    Serial2.print("keyNumber1");
    break;
    case 11:
    Serial2.print("keyNumber2");
    break;
    case 12:
    Serial2.print("keyNumber3");
    break;
    case 13:
    Serial2.print("keyNumber4");
    break;
    case 14:
    Serial2.print("keyNumber5");
    break;
    case 15:
    Serial2.print("keyNumber6");
    break;
    case 16:
    Serial2.print("keyNumber7");
    break;
    case 17:
    Serial2.print("keyNumber8");
    break;
    case 18:
    Serial2.print("keyNumber9");
    break;
    case 19:
    Serial2.print("keyVerb");
    break;
    case 20:
    Serial2.print("keyNone");
    break;
  }
  Serial2.print("\"");
  nextion_enter(2);

  Serial2.print("oldkey.val=");
  Serial2.print(old_key);
  nextion_enter(2);
  if (verb_0 >= 0)
  {
    Serial2.print("oldkey.pco=BLUE");
    nextion_enter(2);
  }
  else
   {
    Serial2.print("oldkey.pco=RED");
    nextion_enter(2);
  }
  Serial2.print("oldkeytxt.txt=\"");
  switch (old_key)
  {
    case 0:
    Serial2.print("keyNone");
    break;
    case 1:
    Serial2.print("keyNoun");
    break;
    case 2:
    Serial2.print("keyProceed");
    break;
    case 3:
    Serial2.print("keyEnter");
    break;
    case 4:
    Serial2.print("keyReset");
    break;
    case 5:
    Serial2.print("keyRelease");
    break;
    case 6:
    Serial2.print("keyClear");
    break;
    case 7:
    Serial2.print("keyPlus");
    break;
    case 8:
    Serial2.print("keyMinus");
    break;
    case 9:
    Serial2.print("keyNumber0");
    break;
    case 10:
    Serial2.print("keyNumber1");
    break;
    case 11:
    Serial2.print("keyNumber2");
    break;
    case 12:
    Serial2.print("keyNumber3");
    break;
    case 13:
    Serial2.print("keyNumber4");
    break;
    case 14:
    Serial2.print("keyNumber5");
    break;
    case 15:
    Serial2.print("keyNumber6");
    break;
    case 16:
    Serial2.print("keyNumber7");
    break;
    case 17:
    Serial2.print("keyNumber8");
    break;
    case 18:
    Serial2.print("keyNumber9");
    break;
    case 19:
    Serial2.print("keyVerb");
    break;
    case 20:
    Serial2.print("keyNone");
    break;
  }
  Serial2.print("\"");
  nextion_enter(2);
}
