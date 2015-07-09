/* 
* speed_enc.h
*
* Created: 09.07.2015 14:30:45
* Author: Maik Wagner
* Source: http://forum.arduino.cc/index.php?topic=140205.55
* -converted the source inro a library
** The inputs are 2 and 13 (and optionally A6 for the index).
** ch a -> 13
** ch b ->  2
*/


#include<Arduino.h>

#ifndef __SPEED_ENC_H__
#define __SPEED_ENC_H__


class speed_enc
{
//variables
public:
	long Speed;
	int Dir;
protected:
private:
	double _SpeedRPS; //Revolution per sec
	double _SpeedRPM; //Revolution per min
	int _EncEdg_Rev; //Encoder Edges per Revolution
	int _EncS_Sec; //Encoder Samples per second
	int _SpeedPPP; //speed pulses per sampling period
	int _IndexCount; 
//functions
public:
	speed_enc(const int enc_edges_per_rev, const int enc_samples_per_sec);
	long speed();
	int direction();
	//~speed_enc();
protected:
private:
	
	//speed_enc( const speed_enc &c );
	//speed_enc& operator=( const speed_enc &c );

}; //speed_enc

#endif //__SPEED_ENC_H__
