#ifndef Debounce_h
#define Debounce_h

#include "Arduino.h"

class Debounce
{
  public:
    Debounce();
    void setup(long, int);
    boolean read(long);
    
    int lastValue;
    int currentValue;
    long lastDebounceTime;
    long debounceDelay;
};

Debounce::Debounce()
{}

void Debounce::setup(long _debounceDelay, int _currentValue)
{
  debounceDelay = _debounceDelay;
  currentValue = _currentValue;
  lastDebounceTime = millis();
}

boolean Debounce::read(long _read)
{
  
  // If the switch changed, due to noise or pressing:
  if (_read != lastValue) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  } 
  lastValue = _read;
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (_read != currentValue) {
      currentValue = _read;

      return true;
    }
  }
  return false;
  //lastValue = _read;
}

#endif
