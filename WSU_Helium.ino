/*
  WSU Helium Valve Control and Scuttle Balloon Function
 
 The circuit:
 * One Wire Bus - pin 3
 * Helium Valve Input - pin 4
 * Scuttle Balloon Input - pin 5
 * Helium Valve Output - pin 6
 * Scuttle Balloon Output - pin 7
  
created  14 March 2013
 
by Bruce Rahn, K0BAR
 
 */

#include <Arduino.h>

//#define DEBUG

#define CYCLE_HELIUM_VALVE

const int Helium_Input = 4;
const int Helium_Output = 6;
const int Scuttle_Input = 5;
const int Scuttle_Output = 7;

unsigned long timer=0;   //general purpose timer
unsigned long Scuttle_Timer = 0;
unsigned long heliumTimer = 0;
unsigned long Helium_Valve_Cycle_Timer = 0;
unsigned long Scuttle_Master_Timer = 0;
boolean DTMFscuttle;
boolean DTMFscuttlestate = 0;
boolean DTMFhelium;
boolean DTMFheliumstate = 0;
boolean heliumvalvestate = 0;
boolean Helium_Valve_Cycle_State = 0;

#define AutoScuttleTime 14400000 //(in miliiseconds) 4 hours
#define HeliumOpenTime 30000 //(in milliseconds) 30 seconds
#define Helium_Valve_Cycle_Time 10000 //(in milliseconds) 10 seconds
#define Helium_Valve_Cycle_Open_Time 10000 //(in milliseconds 10 seconds
#define Scuttle_Heartbeat_Time 10000 //(in milliseconds) 10 seconds

void setup()
{
#ifdef DEBUG
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  Serial.print("Initializing...");
#endif

  digitalWrite(Scuttle_Output,LOW);
  pinMode(Scuttle_Output,OUTPUT);
  digitalWrite(Helium_Output,LOW);
  pinMode(Helium_Output,OUTPUT);
  pinMode(Scuttle_Input,INPUT);
  pinMode(Helium_Input,INPUT);
  digitalWrite(Scuttle_Input,HIGH);
  digitalWrite(Helium_Input,HIGH);

  timer=millis();
  Helium_Valve_Cycle_Timer=timer;
  Scuttle_Master_Timer= timer;
}

void loop()
{
  DTMFhelium = digitalRead(Helium_Input);
  DTMFscuttle = digitalRead(Scuttle_Input);

  if((!DTMFhelium) && (!DTMFheliumstate))
  {
    DTMFheliumstate = 1;
    heliumTimer = millis();
    digitalWrite(Helium_Output, HIGH);
    heliumvalvestate = 1;
  }

  if((DTMFhelium) && (DTMFheliumstate))
  {
    DTMFheliumstate = 0;
  } 

  if(((millis()-heliumTimer)>=HeliumOpenTime) && (heliumvalvestate))
  {
    digitalWrite(Helium_Output,LOW);
    heliumvalvestate = 0;
    Helium_Valve_Cycle_Timer = millis();
  }

#ifdef CYCLE_HELIUM_VALVE
  if(((millis()-Helium_Valve_Cycle_Timer)>=Helium_Valve_Cycle_Time) && (!Helium_Valve_Cycle_State) && (!heliumvalvestate))
  {
    digitalWrite(Helium_Output,HIGH);
    Helium_Valve_Cycle_State = 1;
    Helium_Valve_Cycle_Timer = millis();
  }
 
 if(((millis()-Helium_Valve_Cycle_Timer)>=Helium_Valve_Cycle_Open_Time) && (Helium_Valve_Cycle_State)&& (!heliumvalvestate))
  {
    digitalWrite(Helium_Output,LOW);
    Helium_Valve_Cycle_State = 0;
    Helium_Valve_Cycle_Timer = millis();
  } 
#endif

 if(!DTMFscuttle)
  {
    digitalWrite(Scuttle_Output, HIGH);
  }


  if((millis()-Scuttle_Master_Timer)>=AutoScuttleTime)
  {
    digitalWrite(Scuttle_Output,HIGH);
  }

}

