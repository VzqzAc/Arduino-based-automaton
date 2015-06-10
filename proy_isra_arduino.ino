#include <LiquidCrystal.h>
  
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
boolean flags[8], space;
//char str[8], other[8];
char* final = "----------------";
int entry = 0;
char* cap_letters = {"abcdefghijklmnopqrstuvwxyz"};
char* lowercase_letters = {"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
char* binaries[26] = {"00001","00010","00011","00100","00101","00110","00111","01000","01001","01010","01011","01100","01101",
                     "01110","01111","10000","10001","10010","10011","10100","10101","10110","10111","11000","11001","11010"};
//65-90 mayus
int where = 3, index = 0, flag_count;
boolean cap, space_flag = false, last_validation = true, cap_flag = false, to_keep_previous_apart = false;
String message, message2, message3;
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

char check_word(String str, boolean cap_flag)
{
  for(int i=0; i<26; i++){
      if(str.equals(binaries[i]) && cap_flag) return cap_letters[i];
      else if(str.equals(binaries[i])) return lowercase_letters[i]; 
  }
}


void setup()
{
  for(int i=0; i<8; i++)  flags[i] = false;
  cap = false;
  message = final;
  message2 = String();
  message3 = String();
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
              message2 = "No aceptado";
              message3.remove(0);
                break;
                //for(int i=0; i<8; i++) flags[i] = 0;
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
              message2 = "Letras";
              space_flag = false;
              flags[1] = true;
                break;
            }
            else if(entry == 1)
            {
              message3.concat("0");
              message2 = message3;
              space_flag = true;
              flags[1] = true;
                break;
              //message2 = "Espacio";
            }
            break;
          case 2:
            if(entry == 2)
            {
                if(!space_flag)
                {
                    message3.concat("1");
                    delay(20);
                    message2 = message3 + " Minusculas";
                    flags[2] = true;
                    break;
                }
                else
                {
                  message2 = "Espacio(00000)";
                  to_keep_previous_apart = true;
                  space = true;
                  message3.concat("1");
                  flags[2] = true;
                  break;
                }
                break;
            }
            else if(entry == 1)
            {
              cap_flag = true;
              if(cap && !space_flag)
                {
                  message2 = "No se pueden MAY";
                  for(int i=0; i<8; i++) flags[i] = 0;
                  message3.remove(0);
                  break;
                }
              else if(!cap && !space_flag)
                {
                  message3.concat("0");
                  message2 = message3 + "Mayusculas";
                  flags[2] = true;
                  break;
                }
              else
              {
                message2 = "No aceptado";
                message3.remove(0);
                for(int i = 0; i < 8; i++) flags[i] = false;
                break;
              }
                break;
            }
            break;
          default:
            if(flag_count == 6)
            {
              if(entry == 1)
              {
                message3.remove(0);
                message2.remove(0);
                delay(100);
                message[index] = check_word(message3.substring(3), cap_flag);
                index++;
                for(int i = 0; i < 8; i++) flags[i] = false;
                break;
              }
              else if(entry == 2)
              {
                message2 = "No aceptado";
                message3.remove(0);
                for(int i = 0; i < 8; i++) flags[i] = false;
                break;
              }
              break;
            }
            else if(flag_count < 7)
            {
              if(entry == 1)
              {
                  message3.concat("0");
                  message2 = message3;
                  flags[flag_count + 1] = true;
                break;
              }
              else if(entry == 2)
              {
                message3.concat("1");
                message2 = message3;
                flags[flag_count + 1] = true;
                break;
              }
                break;
            }
            else
            {
              delay(100);
              message[index] = check_word(message3.substring(3), cap_flag); //Checar si es 4 o 3
              index++;
              message2.remove(0);
              message3.remove(0);
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
        message2 = "No aceptado";
        message3.remove(0);
        for(int i=0; i<8; i++)  flags[i] = false;
        space = false;
        where = 3;
      }
    }
    else to_keep_previous_apart = false;
}