#include "speed_enc.h"

#define F_CPU 84000000L

volatile unsigned long currTime2 = 0;

speed_enc speed(1024*4, 100);

String constTimeStringLengthOf10(unsigned long timestamp)
{
	char charBuffer[11+1];
	
	sprintf(charBuffer,"%10lu",timestamp);
	return String(charBuffer);
}

void setup()
{
	Serial.begin(250000);
	Serial.println("Speed_Enc Starting");
	delay(10);
}


void loop()
{
	currTime2 = micros();
	//Serial.print("Speed (RPM): ");
	//Serial.print(speed.speed());
	//Serial.print(" , Direction: ");
	//Serial.println(speed.direction());
	Serial.println("{");
	Serial.print("\t\"voltage\": ");
	Serial.print("12345");
	Serial.println(",");
	Serial.print("\t\"current\": ");
	Serial.print("1234");
	Serial.println(",");
	Serial.print("\t\"rpm\": ");
	Serial.print(speed.speed());
	Serial.println(",");
	Serial.print("\t\"power\": ");
	Serial.print("1234");
	Serial.println(",");
	Serial.print("\t\"timestamp\": ");
	Serial.println(constTimeStringLengthOf10(currTime2));
	Serial.println("}");
	Serial.println("EOL");
/*
{
	"voltage":  2375,
	"current":    9,
	"rpm":    3239792,
	"power":   21,
	"timestamp": 1290463408
}
EOL
{
	"voltage": 00000,
	"current": 0000,
	"rpm": 3272460,
	"power": 0000,
	"timestamp":    4850896
}
EOL
*/

}


