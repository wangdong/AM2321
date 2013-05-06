AM2321
======

AM2321 sensor support for Arduino.

Example:

'''
#include <AM2321.h>

AM2321 am2321;

void AM2321_Read()
{
    am2321.read();

    Serial.print("(");
    Serial.print(am2321.temperature/10.0);
    Serial.print(", ");
    Serial.print(am2321.humidity/10.0);
    Serial.println(')');

	
}
'''
