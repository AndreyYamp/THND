#ifndef TEMPERATURE_H_
#define TEMPERATURE_H_

#include <math.h>
#include <Arduino.h>

#define UPPER_TEMP_LIMIT_C 40
#define LOWER_TEMP_LIMIT_C 1

bool checkTemp(double temp, String *message);
double calcTemp(int rawVal);

#endif
