/*
 * ultrasonico.cpp
 *
 *  Created on: 16 ago. 2025
 *      Author: ojuan
 */

#include <TPO/ultrasonico.h>

Timer T_echo(Timer::MICROS, callback_Timer_Error);

ultrasonico::ultrasonico() {
	// TODO Auto-generated constructor stub
}

ultrasonico::ultrasonico(uint8_t port_echo, uint8_t bit_echo, uint8_t port_trig,
		uint8_t bit_trig) {
	echo(Intext::PINT0, port_echo, bit_echo, callBack_echo);
	echo.set_flanco(Intext::AMBOS_FLANCOS);
	echo.set_nivel(Intext::ALTO);
	trig(port_trig, bit_trig, Gpio::PUSHPULL, Gpio::HIGH, Gpio::OUTPUT);

}

uint32_t getDistancia(void) {
	return distancia;
}

void medir(void) {

}

void callBack_echo(void) {
	static bool flag_flanco = Intext::ASCENDENTE;
	if(flag_flanco == Intext::ASCENDENTE){
		T_echo.TimerStart(100);
	}
}

void setDistancia(uint32_t d) {
	distancia = d;
}
ultrasonico::~ultrasonico() {
	// TODO Auto-generated destructor stub
}

