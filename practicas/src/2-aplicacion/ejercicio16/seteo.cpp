/*
 * seteo.cpp
 *
 *  Created on: 10 ago. 2025
 *      Author: ojuan
 */

#include "seteo.h"

Timer temp(Timer::SEG);

void tempEncRapido(void) {
	temp.TimerStart(TIEMPO_RAPIDO, nullptr, Timer::SEG);
}

bool estadoTempEncRapido(void) {
	return (bool)temp;
}

void setPotencia(Potencia p, Potencia &potencia) {
	potencia = p;
}

void incrementarTiempo(uint32_t t, uint32_t &tiempo) {
	tiempo += t;
}

void reset(Potencia &potencia, uint32_t &tiempo) {
	potencia = MAXIMA;
	tiempo = 0;
}

void resetTiempo(uint32_t &tiempo) {
	tiempo = 0;
}

void iniciarCalentamiento(Potencia p, uint32_t tiempo) {

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


