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

class ultrasonico {
private:
	volatile uint32_t distancia;
	Intext echo;
	Gpio trig;

public:
	ultrasonico();
	ultrasonico(uint8_t port_echo, uint8_t bit_echo, uint8_t port_trig, uint8_t bit_trig);
	uint32_t getDistancia(void);
	void medir(void);

	virtual ~ultrasonico();
private:
	void callBack_echo(void);
	void setDistancia(uint32_t d);
	void callback_Timer_Error();


};

#endif /* SRC_2_APLICACION_TPO_ULTRASONICO_H_ */
