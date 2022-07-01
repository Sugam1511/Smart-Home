
//#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>


char auth[] = "TxbD1AREeUNKHPwlStHuwFgtv30rETcp"; 
char ssid[] = "AndroidAP";  
char pass[] = "87654321";  

char cmd_arr1[100];
int cmd_count1;
char inchar=0,t;
int level;
float temp;

bool ON_OFF1=0;
bool ON_OFF2=0;


BLYNK_WRITE(V0)  // Light 1 
{
  ON_OFF1 = param.asInt();
  if (ON_OFF1 ==1)
  {
    Serial.print("<T11>");
  }
  else 
  {
    Serial.print("<T10>");
  }
}

BLYNK_WRITE(V1) // Light 2
{
  ON_OFF2 = param.asInt();
  if (ON_OFF2 ==1)
  {
    Serial.print("<T21>");
  }
  else 
  {
    Serial.print("<T20>");
  }
}

BLYNK_WRITE(V2) // Set Time 
{
  String i = param.asStr(); //String Value
  Serial.print("<A");Serial.print(i);Serial.print(">");
}

BLYNK_WRITE(V3) // Set Time 
{
  String i = param.asStr(); //String Value
  if(i == "HOT")
  {
   Serial.print("<B>");  
  }
  else if(i == "MID")
  {
   Serial.print("<C>");  
  }
  if(i == "LOW")
  {
   Serial.print("<D>");  
  }
  
}


void setup()
{
  
  Serial.begin(9600); // See the connection status in Serial Monitor
  pinMode(5,OUTPUT);
  digitalWrite(5,LOW);
  Blynk.begin(auth, ssid, pass);
  // Setup a function to be called every second
}

BLYNK_CONNECTED()
{
 digitalWrite(5,HIGH);
}

void loop()
{  
  Blynk.run(); // Initiates Blynk
  if(!Blynk.connected())
  {
    digitalWrite(5,LOW);
    //Serial.println("Reconnecting ... ");
    Blynk.connect();
  }
}
