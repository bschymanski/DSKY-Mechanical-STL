#include <nextion.h>
#include <Arduino.h>
#include <main.h>

String fieldname = " ";

void nextion_print_REG(int dregister, int place, int number)
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

}

void printVerb(int verb, bool blink = false)
{

}

void printNoun(int noun, bool blink = false)
{

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