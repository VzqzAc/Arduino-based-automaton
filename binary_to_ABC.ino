/*
    Arduino-based automaton
    This project was originally made for the 'Automatons I' subject
    on the Autonomous University of Aguascalientes.

    The purpose of this is to convert an 8-bit digital (binary) input into ABC letters from the ascii code
    and the ascii space by following the rules shown below:

    1. Only capital letters, lowercase letters, and the space are accepted by the automaton
    2. There will be accepted only one capital letter by run

    The first rule represent 3 special cases, the 'a', 'A', 'z', and 'Z' in the ascii table, they're meant
    to represent the top and bottom of the binaries accepted by the automaton, independently of the first
    three digits of the binary number, the next five digits must be between '00001' and '11010'. Also, the
    space is represented by the binary number '00100000', which is also a special case.

    The second rule will be verified by a flag, you can notice there are many flags in this code, but remember
    that was made for academic purposes only, anyway there might be a next version more clean-coded
 */


#include <LiquidCrystal.h>
  
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//This boolean arrays are to get te quantity of bits read by the automaton
boolean flags[8];
//The final message that will appear ont the first row of the LCD, here the input letters/spaces will be displayed if accepted
char* final = "----------------";
// 'entry' var is a helper to know which button was pressed, the button that represents '1' or '0'
int entry = 0;
//These arrays contain the letters supported by the automaton
char* lowercase_letters = "abcdefghijklmnopqrstuvwxyz", cap_letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
// 'binaries' array contains the last five digits of the binary string read by the automaton, they will be compared
// below on the 'check_word' function among a substring from the input
char* binaries[26] = {"00001","00010","00011","00100","00101","00110","00111","01000","01001","01010","01011","01100","01101",
                     "01110","01111","10000","10001","10010","10011","10100","10101","10110","10111","11000","11001","11010"};
// A variable to check if the input is going to the special case that can be either 'z' or 'Z' (check rules on the top)
int special_case;

// 'where' is a helper variable for the space case, 'index' is the position where the input will be displayed on the 'final' array
// and 'flag_count' is a var where the result of 'check_flags' function will be allocated
int where, index, flag_count;
boolean cap, space_flag = false, last_validation = true, cap_flag, to_keep_previous_apart = false,  space;

// 'message' is the main message      TODO: maybe can be deleted and leave only 'final' displayed
// 'message2' is the message that will appear on the 2nd row of the LCD, it will display both the input and an accept/error message 
String message, message2, message3;

// The next #define's are meant to keep the arduino input/output pins as constants
#define zero_button 8
#define one_button 9
#define led_pin 7

int check_flags()
{
  int count = 0;
  for(int i=0; i<8; i++){
      if(flags[i]) count++;
  }
  return count;
}

char check_word(String str, boolean capflag)
{
  Serial.print("The cap flag is "); Serial.println(capflag);
  for(int i=0; i<26; i++){
      if(str.equals(binaries[i]))
      {
        if(capflag) return cap_letters[i];
        else return lowercase_letters[i];
      }
  }
  message2 = "ERR-Wrong Input";
  return '-';
}


void setup()
{
  Serial.begin(9600);
  for(int i=0; i<8; i++)  flags[i] = false;
  for(int i=0; i<26; i++){
      Serial.println(lowercase_letters[i]);
  }
  index = 0;
  where = 3;
  cap = false;
  cap_flag = false;
  message = final;
  message2 = String();
  message3 = String();
  special_case = 0;
  pinMode(one_button, INPUT);
  pinMode(zero_button, INPUT);
  pinMode(led_pin, OUTPUT);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Cargando...");
  delay(5000);
}

void loop()
{
  entry = 0;
  digitalWrite(led_pin, HIGH);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(message);
  lcd.setCursor(0, 1);
  lcd.print(message2);
  delay(250);
  while(!digitalRead(zero_button) && !digitalRead(one_button))
  {
    digitalWrite(led_pin, LOW);
    if(digitalRead(zero_button))
    {
         entry = 1;
         delay(500);
         break;
    }
    if(digitalRead(one_button))
    {
        entry = 2;
         delay(500);
         break;
    }
  }
  if(!space)
  {
    flag_count = check_flags();
    switch (flag_count) 
    {
          case 0:
            if(entry == 2)
            {
              message2 = "ERR-Wrong Input";
              message3.remove(0);
              break;
            }
            else if(entry == 1)
            {
              flags[0] = true;
              lcd.clear();
              lcd.setCursor(0, 2);
              lcd.print("0");
              message3.concat("0");
              message2 = message3;
                break;
            }
            break;
          case 1:
            if(entry == 2)
            {
              message3.concat("1");
              message2 = message3 + " Letters";
              space_flag = false;
              flags[flag_count] = true;
                break;
            }
            else if(entry == 1)
            {
              message3.concat("0");
              message2 = message3;
              space_flag = true;
              flags[flag_count] = true;
              break;
            }
            break;
          case 2:
            if(entry == 2)
            {
                if(!space_flag)
                {
                    message3.concat("1");
                    delay(20);
                    message2 = message3 + " Lowercase";
                    flags[flag_count] = true;
                    break;
                }
                else
                {
                  to_keep_previous_apart = true;
                  space = true;
                  message3.concat("1");
                  message2 = message3 + " Space(00000)";
                  flags[flag_count] = true;
                  break;
                }
                break;
            }
            else if(entry == 1)
            {
              if(cap && !space_flag)
                {
                  message2 = "CAP already in";
                  for(int i=0; i<8; i++) flags[i] = 0;
                  message3.remove(0);
                  break;
                }
              else if(!cap && !space_flag)
                { 
                  Serial.println("Entered to cap_flag assignation");   
                  cap_flag = true;
                  message3.concat("0");
                  message2 = message3 + " CAP letters";
                  flags[flag_count] = true;
                  break;
                }
              else
              {
                message2 = "ERR-Wrong Input";
                message3.remove(0);
                for(int i = 0; i < 8; i++) flags[i] = false;
                break;
              }
                break;
            }
            break;
            case 3: case 4:
/*zero*/      if(entry == 1)
              {
                message3.concat("0");
                message2 = message3;
                flags[flag_count] = true;
                break;
              }
              else if(entry == 2)
              {
                special_case++;
                message3.concat("1");
                message2 = message3;
                flags[flag_count] = true;
                break;
              }
              break;

            case 5:
              if(entry == 1)
              {
                special_case++;
                message3.concat("0");
                message2 = message3;
                flags[5] = true;
                break;
              }
              else if(entry == 2)
              {
                if(special_case == 2)
                {
                  message2 = "ERR-Wrong Input";
                  message3.remove(0);
                  special_case = 0;
                  for(int i = 0; i < 8; i++) flags[i] = false;
                  break;
                }
                else
                {
                  message3.concat("1");
                  message2 = message3;
                  flags[5] = true;
                  break;
                }
              }
              break;
            case 6:
              if(entry == 1)
              {
                message3.concat("0");
                message2 = message3;
                flags[flag_count] = true;
                break;
              }
              else if(entry == 2)
              {
                special_case++;
                message3.concat("1");
                message2 = message3;
                flags[flag_count] = true;
                break;
              }
              break;
              // CHECK FOR THE SPECIAL CASE, THE LAST CASE OF THE .txt FILE
          default:
            if(flag_count < 7)
            {
              if(entry == 1)
              {
                  message3.concat("0");
                  message2 = message3;
                  flags[flag_count] = true;
                break;
              }
              else if(entry == 2)
              {
                message3.concat("1");
                message2 = message3;
                flags[flag_count] = true;
                break;
              }
            }
            else if(flag_count == 7)
            {
                while(!digitalRead(zero_button) && !digitalRead(one_button))
                {
                  digitalWrite(led_pin, LOW);
                  if(digitalRead(zero_button))
                  {
                       entry = 1;
                       delay(500);
                       break;
                  }
                  if(digitalRead(one_button))
                  {
                      entry = 2;
                      delay(500);
                      special_case++;
                      break;
                  }
                }
              if(special_case != 4)
              {
                if(entry == 1)
                {
                  message3.concat("0");
                  flags[flag_count] = true;
                }
                else if(entry == 2)
                {
                  message3.concat("1");
                  flags[flag_count] = true;
                }
              }
              else
              {
                Serial.println("Entered to special_case");
                if(entry == 1)
                {
                  message3.concat("0");
                  message2.remove(0);
                  delay(100);
                  Serial.print("This is the substring: ");  Serial.println(message3.substring(3));
                  Serial.print("And the string: "); Serial.println(message3);
                  message[index] = check_word(message3.substring(3), cap_flag);
                  if(cap_flag){ cap_flag = !cap_flag;   cap = true; }
                  index++;
                  message3.remove(0);
                  special_case = 0;
                  for(int i = 0; i < 8; i++) flags[i] = false;
                  break;
                }
                else if(entry == 2)
                {
                  message3.concat("1");
                  Serial.print("This is the substring: ");  Serial.println(message3.substring(3));
                  Serial.print("And the string: "); Serial.println(message3);
                  message2 = "ERR-Wrong Input";
                  message3.remove(0);
                  for(int i = 0; i < 8; i++) flags[i] = false;
                  break;
                }
              }
          }
          if(check_flags() == 8)
          {
            delay(100);
            message2.remove(0);
            Serial.print("This is the substring: ");  Serial.println(message3.substring(3));
            Serial.print("And the string: "); Serial.println(message3);
            special_case = 0;
            message[index] = check_word(message3.substring(3), cap_flag); //Checar si es 4 o 3
            index++;
            
            if(cap_flag){ cap_flag = !cap_flag;   cap = true; }
            
            message3.remove(0);
            special_case = 0;
            for(int i = 0; i < 8; i++) flags[i] = false;
            break;
          }
      }
    }
    if(space && !to_keep_previous_apart)
    {
      if(where == 7)
      {
          for(int i=0; i<8; i++)  flags[i] = false;
          space = false;
          message3.remove(0);
          message.setCharAt(index, ' ');
          message2 = message3;
          where = 0;
          special_case = 0;
          index++;
      }
      else if(entry == 1)
      {
        message3.concat("0");
        message2 = message3;
        flags[where] = true;
        where++;
      }
      else if(entry == 2)
      {
        message2 = "ERR-Wrong Input";
        message3.remove(0);
        for(int i=0; i<8; i++)  flags[i] = false;
        space = false;
        special_case = 0;
        where = 3;
      }
    }
    else to_keep_previous_apart = false;
}
