/*
###############################################################################
# This file is responsible for temperature handling.
###############################################################################
*/

#include "Temperature.h"

/*
###############################################################################
# checkTemp function:
# Compare input temperature against upper and lower limits.
# If one of these limits is exceeded, then global message is initialized with 
# warning text.
#
# INPUT: temp: double - temperature in Celcius degrees
# OUTPUT: result: bool - result of operation:
#			true - temperature has exceeded the limits
#			false - temperature is inside limits
###############################################################################
*/
bool checkTemp(double temp, String *message)
{
	bool result = false;
	if (temp >= UPPER_TEMP_LIMIT_C || temp <= LOWER_TEMP_LIMIT_C){
		*message = "Hello! This is notification from Arduino THND!\nTemperature limit exceeded: " + String(temp);
		result = true;
	}
	else{
		*message = "";
		result = false;
	}
	return result;
}

/*
###############################################################################
# calcTemp function:
# Calculates the temperature from analog signal which is in range of 0..1023
# Steinhart-Hart Thermistor equation is used to calculate temperature.
#
# INPUT: rawVal: int - analog signal from thermistor
# OUTPUT: tempAn: double - temperature in Celcius degrees
###############################################################################
*/
double calcTemp(int rawVal)
{
	double tempAn;
	tempAn = log(10000.0*((1024.0 / rawVal - 1)));
	tempAn = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempAn * tempAn))* tempAn);
	tempAn = tempAn - 273.15;

	return tempAn;
}
