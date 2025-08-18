/*
 * ultrasonico.cpp
 *
 *  Created on: 16 ago. 2025
 *      Author: ojuan
 */

#include <TPO/ultrasonico.h>



static Ultrasonico* g_instance = nullptr;

static void EchoISR()      { if (g_instance) g_instance->callBack_echo(); }
static void TimerTrigISR() { if (g_instance) g_instance->callback_TimerTrig(); }
static void TimerErrISR()  { if (g_instance) g_instance->callback_Timer_Error(); }

Ultrasonico::Ultrasonico(uint8_t port_echo, uint8_t bit_echo, uint8_t port_trig,
		uint8_t bit_trig):
	echo(Intext::PINT0, port_echo, bit_echo, EchoISR),
	trig(port_trig, bit_trig, Gpio::INACTIVE, Gpio::HIGH, Gpio::OUTPUT),
	T_trig(Timer::MICROSEG, TimerTrigISR),
	T_echo(Timer::MICROSEG, TimerErrISR)
	{
	g_instance = this;
	distancia = 0;
	echo.set_flanco(Intext::AMBOS_FLANCOS);
	echo.set_nivel(Intext::ALTO);
	busy = false;
	flag_flanco = Intext::ASCENDENTE;
}

uint32_t Ultrasonico::getDistancia(void) {
	return distancia;
}

void Ultrasonico::medir(void) {
	if(!busy){
	trig.SetPin();
	T_trig.TimerStart(T_TRIG);
	busy = true;
	}
}

void Ultrasonico::callback_TimerTrig(void){
	trig.ClrPin();
}

void Ultrasonico::callBack_echo(void) {
	uint32_t tiempo = 0;
	if(flag_flanco == Intext::ASCENDENTE){
		T_echo.TimerStart(T_MAX);
		flag_flanco = Intext::DESCENDENTE;
	}else if(flag_flanco == Intext::DESCENDENTE){
		T_echo.GetTimer(tiempo);
		T_echo.TimerStop();
		tiempo = T_MAX - tiempo;
		uint32_t d = tiempo/58;
		setDistancia(d);
		flag_flanco = Intext::ASCENDENTE;
	}
	busy = false;
}

 void Ultrasonico::setDistancia(uint32_t d) {
	distancia = d;
}

void Ultrasonico::callback_Timer_Error(void){
	distancia = ERROR_US;
	flag_flanco = Intext::ASCENDENTE;
	busy = false;
}

void Ultrasonico::timerCallBacks(void){
	T_trig.TmrEvent();
	T_echo.TmrEvent();
}

Ultrasonico::~Ultrasonico() {
	g_instance = nullptr;
}

