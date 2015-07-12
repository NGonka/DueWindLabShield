/*                     ___________        ____________________
_________________      | Due     |        |Measurement Board |
|Load controller| -->> |A5     A0| <<---- |ADC0 (= V_G)      |     
-----------------      | |     A1| <<---- |ADC1 (= I_G)      |
|--> PC com Port       | |-> DAC0| ---->> |V_I_Set           |
                       -----------        --------------------
                                 |--> PC com Port            |--> not connected to PC!
 
#####  Version History #####
Due 0.0.1beta:
- converted the Encoder code part into a library
- as first test, the due is used in between the Load controller and the measurement board
- Due replaces the measurement board to use the hardware encoder of the due for the speed
- Due measure the Set Current Voltage from the Load controller Board and feeds it through the measurement board (just for testing purposes and error checking reasons)
- Due now measures the Generator Voltage and current and sends the data to the PC

*/

/*
######## Programming Hint: ########
For Arduino Due, datatype int is the same size as long: -2.147.483.648 to 2.147.483.647 
######## Programming Hint  ########
*/


#include "speed_enc.h"


//defines
#define F_CPU 84000000L

// header definitions
boolean DEBUG=false;
String version ="Due 0.0.1beta";
static long buad = 115200;

//variables for time readings
volatile unsigned long currTime = 0;

//Variables for ADC readings
int readA0;
int readA1;
int RPM;
int ADCbits = 12;
long ADCresolution= pow(2,ADCbits);

//Variables for Serial Communication
String msg = "HH";
int Power;


//External classes binding
speed_enc speed(1024*4, 10);  //Encoder Pulses per Revolution (QuadEncoder), Samples per Seconds


//Functions
int readVcc()
{
	/* 
	* preparation for later!
	* read Voltage Reference at Analog input (3.000V)
	* to compensate possible Input Voltage fluctuations	
	*
	* at the moment it just returns the input Voltage
	*/
	/*
	int V_ref = analogRead(A3);
	result =
	*/
	int result = 3299; //Vcc in mV
	return result;
}

int readVoltage(int Vcc)
{
	//int Vcc = readVcc();
	int V_G = Vcc*analogRead(A0)*5/ADCresolution;
	
	return V_G; //in mV
	
}

int readCurrent(int Vcc)
{
	//int Vcc = readVcc();
	int I_G = Vcc*analogRead(A1)/ADCresolution;
	
	return I_G; //in mA
		
}




String constTimeStringLengthOf10(unsigned long timestamp)
{
	char charBuffer[11+1];
	
	sprintf(charBuffer,"%10lu",timestamp);
	return String(charBuffer);
}


String constRPMStringLengthOf8(long value)
{
	char charBuffer[8+1];
	
	sprintf(charBuffer,"%8ld",value);
	return String(charBuffer);
}

String constVoltStringLengthOf5(long value)
{
	char charBuffer[6+1];
	
	sprintf(charBuffer,"%5ld",value);
	return String(charBuffer);
}

String constCurrStringLengthOf4(long value)
{
	char charBuffer[5+1];
	
	sprintf(charBuffer,"%4ld",value);
	return String(charBuffer);
}






void setup()
{
	Serial.begin(buad);		
	analogReadResolution(ADCbits);
	analogWriteResolution(ADCbits);
	
	delay(10);
	Serial.println("Ready");
}


void loop()
{
	//feed trough of  V_I_Set voltage for the load controller
	
	analogWrite(DAC0,analogRead(A5));
	
	
	//! Read the Serial Data in for the commands
	if(Serial.available()>0)
	{
		msg="";
		while(Serial.available()>0)
		{
			msg+=char(Serial.read());
			delay(10);
		}
		msg = msg.substring(0,2);
		msg.toUpperCase();
		//serial.print("msg: ");
		//serial.println(msg);
	}
	

	//! Definition of the Serial Commands and the send back Data
	if (msg.equals("DD"))
	{
		
		DEBUG=!DEBUG;
		msg= "AA";
		if (DEBUG)
		{
			Serial.println("Debug: On");
			
		}
		else
		{
			Serial.println("Debug: Off");
			
		}
		
	}
	else if (msg.equals("AA"))
	{
		
		int VCC = readVcc();
		
		currTime = micros();		
		readA0	 = readVoltage(VCC);	//in mV
		readA1	 = readCurrent(VCC);	//in mA
		RPM		 = speed.speed();		//in rpm
		Power	 = readA0*readA1/1000;	//in milliWatt
		
		
		if(!DEBUG)
		{
			
			//JSON Output
			Serial.println("{");
				Serial.print("\t\"voltage\": ");
				Serial.print(constVoltStringLengthOf5(readA0));
				Serial.println(",");
				Serial.print("\t\"current\": ");
				Serial.print(constCurrStringLengthOf4(readA1));
				Serial.println(",");
				Serial.print("\t\"rpm\": ");
				Serial.print(constRPMStringLengthOf8(RPM));
				Serial.println(",");
				Serial.print("\t\"power\": ");
				Serial.print(constCurrStringLengthOf4(Power));
				Serial.println(",");
				Serial.print("\t\"timestamp\": ");
				Serial.println(constTimeStringLengthOf10(currTime));
			Serial.println("}");
			Serial.println("EOL");
		}
		else
		{
			//Raw data output
			Serial.print("V: ");
			Serial.print(readA0);
			Serial.print("; ");
			Serial.print("I: ");
			Serial.print(readA1);
			Serial.print("; ");
			Serial.print("N: ");
			Serial.print(RPM);
			Serial.print("; ");
			Serial.print("t: ");
			Serial.println(currTime);

		}

		
	}
	else if (msg.equals("HH"))
	{
		//Help Massage
		Serial.println("Measurement Board");
		Serial.print("Firmware Version: ");
		Serial.println(version);
		Serial.println("Commands:");
		Serial.println("HH : prints this massage");
		Serial.println("AA : print Volt, Current, RPM, Power and a Timestamp (since last restart) in actual Units ([mV],[mA],[1/min],[mW],[us])");
		Serial.println("     Output is in JSON Style!!!");
		//Serial.println("Ax : prints only Analog Pin x (bit value (x: 0-5))");
		Serial.println("DD : toggle debug mode on and off");
		msg="";
		
		
		
	}
	else
	{
		msg="";
		
	}
	
	
}


