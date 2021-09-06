#include "switch8266.h"
#include "Arduino.h"
void pin_init()
{
  pinMode(PWM, OUTPUT); 	

  pinMode(ZCD, INPUT_PULLUP);
}

