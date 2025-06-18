#ifndef HCSR04HANDLER_H
#define HCSR04HANDLER_H

#include <Arduino.h>

void initHCSR04(int trigPin, int echoPin);
float getDistanceCM();

#endif