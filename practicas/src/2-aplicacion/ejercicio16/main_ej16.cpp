/*
 * main_ej16.cpp
 *
 *  Created on: 10 ago. 2025
 *      Author: ojuan
 */

#include "LPC845.h"
#include "Timer.h"
#include "systick.h"
#include "seteo.h"
#include "Gpio.h"
#include "Teclado.h"
#include "perifericotemporizado.h"

#define T_MAX 8000
#define T_MIN 5
volatile Estados estadoSeteo = INICIO;

Gpio scn_0(Gpio::PORT0, 16, Gpio::PUSHPULL, Gpio::HIGH, Gpio::OUTPUT);
Gpio scn_1(Gpio::PORT0, 17, Gpio::PUSHPULL, Gpio::HIGH, Gpio::OUTPUT);
Gpio scn_2(Gpio::PORT0, 18, Gpio::PUSHPULL, Gpio::HIGH, Gpio::OUTPUT);
Gpio scn_3(Gpio::PORT0, 19, Gpio::PUSHPULL, Gpio::HIGH, Gpio::OUTPUT);
Gpio scn_4(Gpio::PORT0, 20, Gpio::PUSHPULL, Gpio::HIGH, Gpio::OUTPUT);

Gpio ret_0(Gpio::PORT0, 21, Gpio::PULLUP, Gpio::LOW, Gpio::INPUT);
Gpio ret_1(Gpio::PORT0, 22, Gpio::PULLUP, Gpio::LOW, Gpio::INPUT);
Gpio ret_2(Gpio::PORT0, 23, Gpio::PULLUP, Gpio::LOW, Gpio::INPUT);

Gpio *teclado_scn[] = { &scn_0, &scn_1, &scn_2, &scn_3, &scn_4, nullptr };
Gpio *teclado_ret[] = { &ret_0, &ret_1, &ret_2, nullptr };

Teclado Keyboard(teclado_ret, teclado_scn);

void handlers(void);

int main(void) {
	SysTick_InstalarCallBack(handlers);
	SysTick_Inicializar(1);
	Potencia potencia = MAXIMA;
	uint8_t tecla = NO_KEY;
	uint32_t tiempo = 0;
	while (1) {
		tecla = Keyboard.GetKey();
		switch (estadoSeteo) {
		case INICIO:
			if (potenciaSeleccionada(tecla)) {
				setPotencia((Potencia) tecla, potencia);
				estadoSeteo = POTENCIA;
			} else if (encendidoPulsado(tecla)) {
				incrementarTiempo(30, tiempo);
				tempEncRapido();
				setPotencia(MAXIMA, potencia);
				estadoSeteo = RAPIDO;
			}
			break;

		case RAPIDO:
			if (encendidoPulsado(tecla) && ((tiempo + 30) <= T_MAX)) {
				incrementarTiempo(30, tiempo);
				tempEncRapido();
			} else if (estadoTempEncRapido()) {
				iniciarCalentamiento(potencia, tiempo);
				estadoSeteo = CALENTANDO;
			} else if (cancelarPulsado(tecla)) {
				reset(potencia, tiempo);
				estadoSeteo = INICIO;
			}
			break;

		case POTENCIA:
			tiempo = 0;
			if (potenciaSeleccionada(tecla)) {
				setPotencia((Potencia) tecla, potencia);
			} else if (numSeleccionado(tecla)) {
				incrementarTiempo(tecla, tiempo);
				estadoSeteo = TIEMPO;
			} else if (cancelarPulsado(tecla)) {
				reset(potencia, tiempo);
				estadoSeteo = INICIO;
			}
			break;

		case TIEMPO:
			if (numSeleccionado(tecla) && tiempo <= T_MAX) {
				tiempo *= 10;
				incrementarTiempo(tecla, tiempo);
			} else if (encendidoPulsado(
					tecla) && tiempo <= T_MAX && tiempo >= T_MIN) {
				iniciarCalentamiento(potencia, tiempo);
				estadoSeteo = CALENTANDO;
			} else if (tiempo > T_MAX) {
				resetTiempo(tiempo);
				//mostrarError();
			} else if (cancelarPulsado(tecla)) {
				reset(potencia, tiempo);
				estadoSeteo = INICIO;
			}
			break;

		case CALENTANDO:
			if (cancelarPulsado(tecla)) {
				reset(potencia, tiempo);
				estadoSeteo = INICIO;
			}
			break;
		}
	}
	return 0;
}

void handlers(void){
	for(int i = 0; i < PerifericoTemporizado::m_countPerifericosTemporizados; i++){
		g_perifericosTemporizados[i]->HandlerDelPeriferico();
	}
}

