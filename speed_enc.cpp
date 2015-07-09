/* 
* speed_enc.cpp
*
* Created: 09.07.2015 14:30:45
* Author: Maik Wagner
* Source: http://forum.arduino.cc/index.php?topic=140205.55
* -converted the source into a library
*
** The inputs are 2 and 13 (and optionally A6 for the index).
** ch a -> 13
** ch b ->  2 
*/


#include "speed_enc.h"

// default constructor
speed_enc::speed_enc(const int enc_edges_per_rev, const int enc_samples_per_sec)
{
	_EncEdg_Rev	= enc_edges_per_rev;
	_EncS_Sec	= enc_samples_per_sec;
	
	REG_PMC_PCER0 = PMC_PCER0_PID27
	| PMC_PCER0_PID28
	| PMC_PCER0_PID29;

	// Setup a channel in waveform mode (eg an input into the encoder to trigger the time based sampling)
	// Note some of the choices here impact calculations below and if you change them, you need to change
	// the next section to suit.
	// Section 37.7.11 p906 (also refer to Section 37.6.14.5 p891 for detail of what to set)
	REG_TC0_CMR2 = TC_CMR_TCCLKS_TIMER_CLOCK4
	| TC_CMR_WAVE
	| TC_CMR_ACPC_TOGGLE
	| TC_CMR_WAVSEL_UP_RC;

	// Now define the sample period, using the clock chosen above as the basis
	// Note that REG_TC0_CMR2 above is using CLOCK4; this is an 128 divisor. You need to change the
	// divisor below if you change the clock above. You could change the input clock and the RC mode to
	// suit your app (eg how many pulses are you  expecting - depends on encoder type
	// and slowest/normal/fastest rotation speed and what you want to do with the result).
	// Section 37.6.14.5 p891 notes you need to set this up, otherwise 0 comes out all the time :-)
	REG_TC0_RC2 = F_CPU / 128 / _EncS_Sec;
	
	// Setup a channel in capture mode
	// Section 37.7.10 p904 (also refer to Section 37.6.14.5 p891 for detail of what to set)
	REG_TC0_CMR0 = TC_CMR_ABETRG
	| TC_CMR_LDRA_EDGE
	| TC_CMR_LDRB_EDGE
	| TC_CMR_ETRGEDG_EDGE
	| TC_CMR_CPCTRG;

	// Enable features, noting Speed not Position is chosen
	// Section 37.7.2 p895 (also refer to Section 37.6.14.5 p891 for detail of what to set)
	REG_TC0_BMR = TC_BMR_QDEN
	| TC_BMR_SPEEDEN
	| TC_BMR_EDGPHA;
	
	// Set everything going
	REG_TC0_CCR0 = TC_CCR_CLKEN | TC_CCR_SWTRG;
	REG_TC0_CCR1 = TC_CCR_CLKEN | TC_CCR_SWTRG;
	REG_TC0_CCR2 = TC_CCR_CLKEN | TC_CCR_SWTRG;
	
} //speed_enc

long speed_enc::speed()
{
	_SpeedPPP = REG_TC0_RA0;
	_SpeedRPS = (( _SpeedPPP / (_EncEdg_Rev*1.0)) * _EncS_Sec);
	Speed = long(_SpeedRPS *60*1000);
	return Speed;
}

int speed_enc::direction()
{
		_IndexCount = REG_TC0_CV1;
		Dir = _IndexCount;
		return Dir;
}

//// default destructor
//speed_enc::~speed_enc()
//{
//} //~speed_enc
