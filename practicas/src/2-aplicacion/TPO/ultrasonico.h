/*
 * ultrasonico.h
 *
 *  Created on: 16 ago. 2025
 *      Author: ojuan
 */

#ifndef SRC_2_APLICACION_TPO_ULTRASONICO_H_
#define SRC_2_APLICACION_TPO_ULTRASONICO_H_

#include "LPC845.h"
#include "Gpio.h"
#include "intext.h"
#include "Timer.h"

#define T_MAX 30000 // 30000 uS
#define T_TRIG 10 // 10uS
class Ultrasonico {

private:

#define ERROR_US 1000

	volatile uint32_t distancia;
	volatile bool flag_flanco;
	volatile bool busy;
	Intext echo;
	Gpio trig;
	Timer T_trig;
	Timer T_echo;

public:
	Ultrasonico(uint8_t port_echo, uint8_t bit_echo, uint8_t port_trig,
			uint8_t bit_trig);
	uint32_t getDistancia(void);
	void medir(void);
	void callback_Timer_Error(void);
	void callback_TimerTrig(void);
	void callBack_echo(void);
	void timerCallBacks(void);
	virtual ~Ultrasonico();
private:

	void setDistancia(uint32_t d);

};

#endif /* SRC_2_APLICACION_TPO_ULTRASONICO_H_ */
