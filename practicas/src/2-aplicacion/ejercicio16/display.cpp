/*
 * display.cpp
 *
 *  Created on: 12 ago. 2025
 *      Author: ojuan
 */
#include "Timer.h"
#include "Display7Segmentos.h"

void actualizarDisplay(void);
void tempCallBack(void);

Timer T_ActualizarDisplay(Timer::SEG, actualizarDisplay);
Timer temp(Timer::SEG, tempCallBack);
Display7Segmentos Display();

volatile uint32_t tiempo = 0;

enum estados {
	RAPIDO, TIEMPO, CALENTANDO
};

int main(void) {
	T_ActualizarDisplay.TimerStart(1);
	while (1) {
		switch (estados) {
		case RAPIDO:
			// transicion a CALENTANDO -> temp.TimerStart(1);
			break;
		case TIEMPO:
			// transicion a CALENTANDO -> temp.TimerStart(1);
			break;
		case CALENTANDO:

			break;

		}
	}
	return 0;
}

void handlers(void) {
	for (int i = 0; i < PerifericoTemporizado::m_countPerifericosTemporizados;
			i++) {
		g_perifericosTemporizados[i]->HandlerDelPeriferico();
	}
	temp.TmrEvent();
	T_ActualizarDisplay.TmrEvent();
}

void tempCallBack(void) {
	temp.TimerStart(1);
	tiempo--;
}

void actualizarDisplay(void) {
	T_ActualizarDisplay.TimerStart(1);
	Display.SetDisplay(tiempo, Display7Segmentos::DSP0);
}

