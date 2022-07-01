#include <Wire.h> // Library for I2C communication
#include <SPI.h>  // not used here, but needed to prevent a RTClib compile error
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); 
RTC_DS1307 RTC;     // Setup an instance of DS1307 naming it RTC

#define IR1 2
#define IR2 3

#define REL1 A0
#define REL2 A1
#define REL3 A2
#define REL4 A3

unsigned int visin = 0, visout = 0;
unsigned int flh1,flh2;
unsigned long valY, valX;

char inchar=0,t;
char cmd_arr1[100];
int cmd_count1;
long Time_set = 700;
long Time_set_off = 730;
long settime = 0;
int on_flag =0;
int hours,mint; 
int temp_thress = 20;
long timestam;

// First we include the libraries
#include <OneWire.h> 
#include <DallasTemperature.h>
/********************************************************************/
// Data wire is plugged into pin 2 on the Arduino 
#define ONE_WIRE_BUS 10 
/********************************************************************/
// Setup a oneWire instance to communicate with any OneWire devices  
// (not just Maxim/Dallas temperature ICs) 
OneWire oneWire(ONE_WIRE_BUS); 
/********************************************************************/
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
float temp;
void serialFlush()
{
  while(Serial.available() > 0) {
    t = Serial.read();
  }
  t=0;
} 
void serial_get_command()
{
  inchar=0;
  if(Serial.available() > 0)
  {
    inchar = Serial.read();    
    if(inchar == '<')
    {
      cmd_count1=0;
      while(inchar != '>' && cmd_count1<15)
      {
        if(Serial.available() > 0)
        {
          inchar = Serial.read();
          cmd_arr1[cmd_count1++] = inchar;
        }        
      }
      if(inchar == '>')
      { 
        cmd_count1--;
        cmd_arr1[cmd_count1] = '\0';
 
        if(cmd_arr1[0]=='A')
        {
          cmd_arr1[0]='0';
          Time_set = atol(cmd_arr1);
          on_flag= 0;
          //Time_set_off = Time_set + 1;
          //lcd.setCursor(0, 1);lcd.print(" L4 ON Time Set ");delay(1500);lcd.setCursor(0, 1);lcd.print("                ");
        }
        else if(cmd_arr1[0]=='B')
        {
          cmd_arr1[0]='0';
          lcd.setCursor(0, 1);lcd.print("  Time Set: HOT  ");delay(1500);lcd.setCursor(0, 1);lcd.print("                ");
          temp_thress = 45;
        }
        else if(cmd_arr1[0]=='C')
        {
          cmd_arr1[0]='0';
          lcd.setCursor(0, 1);lcd.print("  Time Set: MID  ");delay(1500);lcd.setCursor(0, 1);lcd.print("                ");
          temp_thress = 40;
        }
        else if(cmd_arr1[0]=='D')
        {
          cmd_arr1[0]='0';
          lcd.setCursor(0, 1);lcd.print("  Time Set: LOW  ");delay(1500);lcd.setCursor(0, 1);lcd.print("                ");
          temp_thress = 35;
        }
        else if(cmd_arr1[0]=='T' && cmd_arr1[1]=='1' && cmd_arr1[2]=='1')
        {cmd_arr1[0]='0';cmd_arr1[1]='0';cmd_arr1[2]='0'; digitalWrite(REL2,LOW);lcd.setCursor(0, 1);lcd.print("   Light 2 ON   "); delay(1500);lcd.setCursor(0, 1);lcd.print("                ");}
        else if(cmd_arr1[0]=='T' && cmd_arr1[1]=='1' && cmd_arr1[2]=='0')
        {cmd_arr1[0]='0';cmd_arr1[1]='0';cmd_arr1[2]='0'; digitalWrite(REL2,HIGH);lcd.setCursor(0, 1);lcd.print("   Light 2 OFF  ");delay(1500);lcd.setCursor(0, 1);lcd.print("                ");}
        
        else if(cmd_arr1[0]=='T' && cmd_arr1[1]=='2' && cmd_arr1[2]=='1')
        {cmd_arr1[0]='0';cmd_arr1[1]='0';cmd_arr1[2]='0'; digitalWrite(REL3,LOW);lcd.setCursor(0, 1);lcd.print("   Light 3 ON   ");delay(1500);lcd.setCursor(0, 1);lcd.print("                ");}   
        else if(cmd_arr1[0]=='T' && cmd_arr1[1]=='2' && cmd_arr1[2]=='0')
        {cmd_arr1[0]='0';cmd_arr1[1]='0';cmd_arr1[2]='0'; digitalWrite(REL3,HIGH);lcd.setCursor(0, 1);lcd.print("   Light 3 OFF  ");delay(1500);lcd.setCursor(0, 1);lcd.print("                ");}   
        
      }  
      else
      {
          
      }
    }
}
}

void counter_algorithm()
{
  if(flh1==0)
  {
    if(digitalRead(IR1) == LOW)    // IR value, adjust/change as per your sensor's logic
    {
      flh1=1;
      flh2=0;
      
    }
    else if(digitalRead(IR2) == LOW)   // IR value, adjust/change as per your sensor's logic
    {
      flh1=1;
      flh2=1;
    }
  }                                                            
  
  if(flh1==1)
  {
    if(flh2==1)
    {
      if(digitalRead(IR1) == LOW)    // IR value, adjust/change as per your sensor's logic
      {
        flh1=0;
        if(visin<999)
        {     
          visin++;
        }
        while(digitalRead(IR1) == LOW){ }
        
        lcd.setCursor(12, 0);
        lcd.print("C:");
        lcd.setCursor(15, 0);
        lcd.print(" ");
        lcd.setCursor(15, 0);
        lcd.print(visin);
       }
    }
    
    if(flh2==0)
    {
      if(digitalRead(IR2) == LOW)  // IR value, adjust/change as per your sensor's logic
      {
        flh1=0;
        if(visin>0)
        {
          visin--; 
        }  
      while(digitalRead(IR2) == LOW){}
      
      lcd.setCursor(12, 0);
      lcd.print("C:");
      lcd.setCursor(15, 0);
      lcd.print(" ");
      lcd.setCursor(15, 0);
      lcd.print(visin);
      }                 
    }
  } 

  lcd.setCursor(12, 0);
  lcd.print("C:");
  lcd.setCursor(15, 0);
  lcd.print(" ");
  lcd.setCursor(15, 0);
  lcd.print(visin);
  
  if(visin >= 1)
  {
    digitalWrite(REL1,LOW);
  }
  if(visin < 1)
  {
    digitalWrite(REL1,HIGH);
  }

}

void temperature_logic_control()
{
  sensors.requestTemperatures();
  temp = sensors.getTempCByIndex(0);
  lcd.setCursor(0,0);
  lcd.print("READING TEMP.   ");
  lcd.setCursor(0,1);
  lcd.print("Temp :          ");
  lcd.setCursor(7,1);
  lcd.print(temp,1);lcd.print(" *C");
  
  if(settime >= Time_set)
  {
     if(on_flag == 0)
     {
      digitalWrite(REL4,LOW);
      on_flag= 1;
      lcd.setCursor(0, 1);lcd.print("   Light 4 ON   ");delay(1500);lcd.setCursor(0, 1);lcd.print("                ");
     }
  }
  if(temp >= temp_thress)
  {
    if(on_flag == 1)
     {
      digitalWrite(REL4,HIGH);
       on_flag= 2;
       lcd.setCursor(0, 1);lcd.print("   Light 4 OFF  ");delay(1500);lcd.setCursor(0, 1);lcd.print("                ");
     }
  }


}

void setup () 
{
  Serial.begin(9600); // Set serial port speed
  sensors.begin();
  pinMode(IR1, INPUT);  pinMode(IR2, INPUT);
  digitalWrite(IR1,HIGH);digitalWrite(IR2,HIGH);
  pinMode(REL1, OUTPUT);
  pinMode(REL2, OUTPUT);
  pinMode(REL3, OUTPUT);
  pinMode(REL4, OUTPUT);
  digitalWrite(REL1,HIGH);digitalWrite(REL2,HIGH);
  digitalWrite(REL3,HIGH);digitalWrite(REL4,HIGH);
  lcd.begin(16,2);
  lcd.init();
  lcd.backlight();
  Wire.begin(); // Start the I2C
  RTC.begin();  // Init RTC
  //RTC.adjust(DateTime(__DATE__, __TIME__));  // Time and date is expanded to date and time on your computer at compiletime
  Serial.print("Time and date set");
  lcd.setCursor(0, 0);
  lcd.print("   Smart Home   ");
  delay(3000);
  lcd.clear();
  DateTime now = RTC.now();
  hours = now.hour(), DEC;
  mint  = now.minute(), DEC;
  settime = (hours*100 + mint*1);
  Time_set = settime *1000;
  timestam = millis();
}
void time_print()
{
  DateTime now = RTC.now();
  hours = now.hour(), DEC;
  mint  = now.minute(), DEC;
  settime = (hours*100 + mint*1);
  lcd.setCursor(0, 0);
  lcd.print("T:          ");
  lcd.setCursor(3, 0);
  lcd.print(now.hour(), DEC);
  lcd.print(":");
  lcd.print(now.minute(), DEC);
   
}
 
void loop () 
{
  time_print(); 
  serial_get_command(); 
  counter_algorithm();
  if(millis()-timestam>=30000)
  {
    timestam=millis();
    temperature_logic_control();
    delay(500);
  }
  delay(1);
  
}
