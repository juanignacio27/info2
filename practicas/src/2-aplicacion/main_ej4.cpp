/*
 * main_ej4.cpp
 *
 *  Created on: 9 ago. 2025
 *      Author: ojuan
 */

#include "Gpio.h"
#include "LPC845.h"

#define PIN_CALENTADOR 0
#define PIN_TREF 16
#define PIN_TEMP1 17
#define CalentarOn Calentador.SetPin()
#define CalentarOff Calentador.ClrPin()

enum Estados{CALENTAR_OFF, CALENTAR_ON};

Gpio Calentador(Gpio::PORT0, PIN_CALENTADOR , Gpio::PUSHPULL , Gpio::HIGH , Gpio::OUTPUT );
Gpio Tref(Gpio::PORT0, PIN_TREF , Gpio::PULLDOWN , Gpio::HIGH , Gpio::INPUT );
Gpio Temp1(Gpio::PORT0, PIN_TEMP1 , Gpio::PULLDOWN , Gpio::HIGH , Gpio::INPUT );

Estados estado = CALENTAR_ON;

int main(void){
	CalentarOn;
	while(1){
		if(estado == CALENTAR_ON && Tref.GetPin() == Gpio::ON){
			CalentarOff;
			estado = CALENTAR_OFF;
		}else if(estado == CALENTAR_OFF && Temp1.GetPin() == Gpio::OFF){
			CalentarOn;
			estado = CALENTAR_ON;
		}
	}
	return 0;
}



