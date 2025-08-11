/*
 * seteo.cpp
 *
 *  Created on: 10 ago. 2025
 *      Author: ojuan
 */

#include "seteo.h"

Timer temp(Timer::SEG);
volatile uint32_t* tiempo_aux = nullptr;

void tempEncRapido(void) {
	temp.TimerStart(TIEMPO_RAPIDO, nullptr, Timer::SEG);
}

bool estadoTempEncRapido(void) {
	return (bool) temp;
}

void setPotencia(Potencia p, Potencia &potencia) {
	potencia = p;
}

void incrementarTiempo(uint32_t t, uint32_t &tiempo) {
	tiempo += t;
}

void reset(Potencia &potencia, uint32_t &tiempo, Gpio &calentador1,
		Gpio &calentador2, Gpio &calentador3, Gpio& sirena) {
	calentador1.ClrPin();
	calentador2.ClrPin();
	calentador3.ClrPin();
	sirena.ClrPin();

	potencia = MAXIMA;
	tiempo = 0;
}

void resetTiempo(uint32_t &tiempo) {
	tiempo = 0;
}

void iniciarCalentamiento(Potencia p, uint32_t *tiempo, Gpio &calentador1,
		Gpio &calentador2, Gpio &calentador3) {
	switch (p) {
	case MINIMA:
		calentador1.SetPin();
		break;
	case MEDIA:
		calentador1.SetPin();
		calentador2.SetPin();
		break;
	case MAXIMA:
		calentador1.SetPin();
		calentador2.SetPin();
		calentador3.SetPin();
		break;
	}
	tiempo_aux = tiempo;
	temp.TimerStart(1, temp_CallBack, Timer::SEG);
}

void temp_CallBack(void) {
	(*tiempo_aux)--;
	if ((*tiempo_aux))
		temp.TimerStart(1, temp_CallBack, Timer::SEG);

}

bool potenciaSeleccionada(uint8_t tecla) {
	bool retorno = false;
	if (tecla >= MINIMA && tecla <= MAXIMA) {
		retorno = true;
	}
	return retorno;
}

bool encendidoPulsado(uint8_t tecla) {
	bool retorno = false;
	if (tecla == ENCENDIDO) {
		retorno = true;
	}
	return retorno;
}

bool cancelarPulsado(uint8_t tecla) {
	bool retorno = false;
	if (tecla == CANCELAR) {
		retorno = true;
	}
	return retorno;
}

bool numSeleccionado(uint8_t tecla) {
	bool retorno = false;
	if (tecla >= 0 && tecla <= 9) {
		retorno = true;
	}
	return retorno;
}


