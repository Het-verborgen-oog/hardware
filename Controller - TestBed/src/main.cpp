#include <Arduino.h>
#include <string.h>

// TODO: Make sure pins are equal to the ones currently in use, so the client wont have to switch stuff around

// PIN DEFINES
#define MotorPin A3
#define HorizontalPin A2
#define VerticalPin A1

// DATA DEFINES
#define StartMarker '@'
#define PayloadMarker ':'
#define EndMarker '#'

#define SpeedProtocol "SPD"
#define HorizontalProtocol "HRZ"
#define VerticalProtocol "VER"

// SERIAL DEFINES
#define BaudRate 9600
#define MessageDelay 50

// CYCLING DEFINES
#define ResetDelay 500
int lastPing = 0;
int CycleState = LOW;
int Rotations = 0;

// SPEED DEFINES
#define MinSpeed 0
#define MaxSpeed 5


bool sensorState;
int HorizontalTilt, VerticalTilt, Speed;

void setup()
{
  pinMode(MotorPin,INPUT);
  pinMode(HorizontalPin,INPUT);
  pinMode(VerticalPin,INPUT);
  Serial.begin(BaudRate);
}

void ReadSpeed() // Will return the speed that the thing is being driven by.
{
  int check = digitalRead(MotorPin);
  if(CycleState != check)
  {
    if(check == LOW)
    {
      Rotations++;
    }
    CycleState = check;
  }
  return;
}

void ResetRotations()
{
  Rotations = 0;
  lastPing = millis();
}

void SendMessage(char protocol[], int value)
{
  Serial.print(StartMarker);
  Serial.print(protocol);
  Serial.print(PayloadMarker);
  Serial.print(value);
  Serial.println(EndMarker);
  delay(20);
}

void loop()
{
  HorizontalTilt = analogRead(HorizontalPin);
  VerticalTilt = analogRead(VerticalPin);
  Speed = map(analogRead(MotorPin),0,1023,MinSpeed,MaxSpeed);

  //if(millis() > ResetDelay + lastPing) ResetRotations();

  SendMessage(SpeedProtocol,Speed);
  SendMessage(HorizontalProtocol,HorizontalTilt);
  SendMessage(VerticalProtocol,VerticalTilt);
}