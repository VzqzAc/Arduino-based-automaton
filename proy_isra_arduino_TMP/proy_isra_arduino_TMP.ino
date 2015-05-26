#include <LiquidCrystal.h>
  
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
boolean flags[8], space;
//char str[8], other[8];
String final = "----------------";
char cap_letters[26];
char lowercase_letters[26];
char* binaries[] = {"00001","00010","00011","00100","00101","00110","00111","01000","01001","01010","01011","01100","01101",
                     "01110","01111","10000","10001","10010","10011","10100","10101","10110","10111","11000","11001","11010"};
//65-90 mayus
int where = 3, index = 0, flag_count;
boolean cap, space_flag = false, last_validation = true, cap_flag = false;
String message, message2, message3;
#define zero_button 8
#define one_button 9

int check_flags()
{
  int count = 0;
  for(int i=0; i<8; i++){
      if(flags[i]) count++;
  }
  return count;
}

char check_word(String str, boolean cap_flag)
{
  for(int i=0; i<26; i++){
      if(str.equals(binaries[i]) && cap_flag) return cap_letters[i];
      else if(str.equals(binaries[i])) return lowercase_letters[i]; 
  }
}


void setup()
{
  int justforfun;
  for(int i=0; i<8; i++){
      flags[i] = false;
  }
  for(int i=65; i<91; i++){
      cap_letters[justforfun] = i;
      justforfun++;
    }
  justforfun = 0;
  for(int i=97; i<123; i++){
      lowercase_letters[justforfun] = i;
      justforfun++;
    }
  cap = false;
  message = String();
  message2 = String();
  message3 = String();
  pinMode(one_button, INPUT);
  pinMode(zero_button, INPUT);
  lcd.begin(16, 2);
  lcd.setCursor(0, 1);
}

void loop()
{
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print(message);
  lcd.setCursor(0, 2);
  lcd.print(message2);
  if(!space)
  {
    flag_count = check_flags();
    switch (flag_count) 
    {
          case 0:
            if(digitalRead(one_button))
            {
              message2 = "No aceptado";
                //for(int i=0; i<8; i++) flags[i] = 0;
            }
            else if(digitalRead(zero_button))
            {
              flags[0] = true;
              lcd.clear();
              lcd.setCursor(0, 2);
              lcd.print("0");
              message3.concat("0");
              message2 = message3;
            }
            break;
          case 1:
            if(digitalRead(one_button))
            {
              message3.concat("1");
              message2 = "Letras";
            }
            else if(digitalRead(zero_button))
            {
              message3.concat("0");
              space_flag = true;
              //message2 = "Espacio";
            }
            flags[1] = true;
            break;
          case 2:
            if(digitalRead(one_button))
            {
                if(!space_flag)
                {
                    message3.concat("1");
                    message2 = message3 + "Minusculas";
                    flags[2] = true;
                }
                //else if(flags[1] && cap)
                //{
                //  message2 = "No se pueden min";
                //  for(int i=0; i<8; i++) flags[i] = 0;
                //  message3.remove(0);
                //}
                else
                {
                  message2 = "Espacio(00000)";
                  space = true;
                  message3.concat("1");
                  flags[2] = true;
                }
            }
            if(digitalRead(zero_button))
            {
              cap_flag = true;
              if(cap)
                {
                  message2 = "No se pueden MAY";
                  for(int i=0; i<8; i++) flags[i] = 0;
                  message3.remove(0);
                }
              else
                {
                  message3.concat("0");
                  message2 = message3 + "Mayusculas";
                  flags[2] = true;
                }
            }
            break;
          default:
            if(flags[3] && flags[4] && flags[6])
            {
              if(digitalRead(zero_button))
              {
                message3.concat("0");
                final[index] = check_word(message3.substring(3), cap_flag);
                index++;
                for(int i = 0; i < 8; i++) flags[i] = false;
                break;
              }
              else if(digitalRead(one_button))
              {
                message2 = "No aceptado";
                message3.remove(0);
                for(int i = 0; i < 8; i++) flags[i] = false;
                break;
              }
            }
            if(!flags[7])
            {
              if(digitalRead(zero_button))
              {
                  message3.concat("0");
                  message2 = message3;
              }
              else if(digitalRead(one_button))
              {
                message3.concat("1");
                message2 = message3;
              }
            }
            else
            {
              final[index] = check_word(message3.substring(3), cap_flag);
              index++;
              for(int i = 0; i < 8; i++) flags[i] = false;
            }
            Serial.println();
            // do something
      }
    }
    if(space)
    {
      if(where == 8)
      {
          for(int i=0; i<8; i++)  flags[i] = false;
          space = false;
          message3.remove(0);
          message.setCharAt(index, ' ');
          index++;
      }
      else if(digitalRead(zero_button))
      {
        message3.concat("0");
        flags[where] = 1;
        where++;
      }
      else if(digitalRead(one_button))
      {
        message2 = "No aceptado";
        message3.remove(0);
        for(int i=0; i<8; i++)  flags[i] = false;
        space = false;
      }
    }  
}