#ifndef Customer_h
#define Customer_h

#include "Arduino.h"
#include "debounce.h"

const int SMOOTHING_TOTAL = 20;
const long BUTTON_DEBOUNCE = 10;
const long SENSOR_DEBOUNCE = 20;
const long SENSOR_THRESHOLD = 500;

/*
struct AvgValue
{
  int total;
  int readings[SMOOTHING_TOTAL];
  int average;
  int readIndex;
};
*/

class Customer
{
  public:
    Customer();
    void setup(int, int, int, int, int);
    boolean readSensorValue(int _index);
    boolean readButtonValue(int _index);
    
    boolean initRfid, init;
    String rfid;
    
    int serialPin[2];
    int btnPin[3];

    Debounce glassStatus[2];
    Debounce btnStatus[3];
    
    //AvgValue glassTotals[2];
};

Customer::Customer()
{}

void Customer::setup(int _serialPin0, int _serialPin1, int _btnPin0, int _btnPin1, int _btnPin2)
{
  //SET PINS
  serialPin[0] = _serialPin0;
  serialPin[1] = _serialPin1;

  btnPin[0] = _btnPin0;
  btnPin[1] = _btnPin1;
  btnPin[2] = _btnPin2;
  
  //For each button
  for (int i=0; i<3; i++) {

    pinMode(btnPin[i],INPUT);
    btnStatus[i].setup(BUTTON_DEBOUNCE,0);
  }

  //For each Glass
  for (int i=0; i<2; i++) {
    glassStatus[i].setup(SENSOR_DEBOUNCE,1);
  }
  
  initRfid = false;
  init = false;
  rfid = "";
}

boolean Customer::readButtonValue(int _btnIndex)
{
  
  return btnStatus[_btnIndex].read(digitalRead(btnPin[_btnIndex]));
}

boolean Customer::readSensorValue(int _glassIndex)
{
  int _val = 0;
  if (analogRead(serialPin[_glassIndex]) < SENSOR_THRESHOLD) {
    _val = 1; 
  }
  return glassStatus[_glassIndex].read(_val);
}


#endif
