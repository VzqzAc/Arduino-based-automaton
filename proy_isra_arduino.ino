#include <LiquidCrystal.h>
  
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
boolean flags[8], space;
//char str[8], other[8];
char* final = "----------------";
int entry = 0;
char* lowercase_letters = "abcdefghijklmnopqrstuvwxyz";
char* cap_letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
char* binaries[26] = {"00001","00010","00011","00100","00101","00110","00111","01000","01001","01010","01011","01100","01101",
                     "01110","01111","10000","10001","10010","10011","10100","10101","10110","10111","11000","11001","11010"};
//65-90 mayus

int special_case;

int where = 3, index = 0, flag_count;
boolean cap, space_flag = false, last_validation = true, cap_flag, to_keep_previous_apart = false;
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
  message2 = "No aceptado";
  return '-';
}


void setup()
{
  Serial.begin(9600);
  for(int i=0; i<8; i++)  flags[i] = false;
  for(int i=0; i<26; i++){
      Serial.println(lowercase_letters[i]);
  }
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
              message2 = "No aceptado";
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
              message2 = "Letras";
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
                    flags[flag_count] = true;
                    break;
                }
                else
                {
                  message2 = "Espacio(00000)";
                  to_keep_previous_apart = true;
                  space = true;
                  message3.concat("1");
                  flags[flag_count] = true;
                  break;
                }
                break;
            }
            else if(entry == 1)
            {
              if(cap && !space_flag)
                {
                  message2 = "No se pueden MAY";
                  for(int i=0; i<8; i++) flags[i] = 0;
                  message3.remove(0);
                  break;
                }
              else if(!cap && !space_flag)
                { 
                  Serial.println("Entered to cap_flag assignation");   
                  cap_flag = true;
                  message3.concat("0");
                  message2 = message3 + "Mayusculas";
                  flags[flag_count] = true;
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
                  message2 = "No aceptado";
                  message3.remove(0);
                  special_case = false;
                  for(int i = 0; i < 8; i++) flags[i] = false;
                  break;
                }
                message3.concat("1");
                message2 = message3;
                flags[5] = true;
                break;
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
            if(flag_count == 7 && special_case == 3)
            {
              Serial.println("Entered to special case");
              if(entry == 1)
              {
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
                Serial.print("This is the substring: ");  Serial.println(message3.substring(3));
                Serial.print("And the string: "); Serial.println(message3);
                message2 = "No aceptado";
                message3.remove(0);
                for(int i = 0; i < 8; i++) flags[i] = false;
                break;
              }
              break;
            }
            else if(flag_count < 8)
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
            if(flag_count == 8)
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
        message2 = "No aceptado";
        message3.remove(0);
        for(int i=0; i<8; i++)  flags[i] = false;
        space = false;
        special_case = 0;
        where = 3;
      }
    }
    else to_keep_previous_apart = false;
}