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
#include "display7Segmentos.h"

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

Gpio calentador_1(Gpio::PORT0, 0, Gpio::PUSHPULL, Gpio::HIGH, Gpio::OUTPUT);
Gpio calentador_2(Gpio::PORT0, 1, Gpio::PUSHPULL, Gpio::HIGH, Gpio::OUTPUT);
Gpio calentador_3(Gpio::PORT0, 2, Gpio::PUSHPULL, Gpio::HIGH, Gpio::OUTPUT);

Gpio termostato(Gpio::PORT0, 4, Gpio::PULLUP, Gpio::LOW, Gpio::INPUT);

Gpio *teclado_scn[] = { &scn_0, &scn_1, &scn_2, &scn_3, &scn_4, nullptr };
Gpio *teclado_ret[] = { &ret_0, &ret_1, &ret_2, nullptr };

Teclado Keyboard(teclado_ret, teclado_scn);

Timer tempAlarma(Timer::SEG);
Timer tempSirena(Timer::SEG);


Gpio Sirena(Gpio::PORT0, 3, Gpio::PUSHPULL, Gpio::HIGH, Gpio::OUTPUT);

Gpio g_segmento_a( Gpio::PORT1,18,Gpio::PUSHPULL,Gpio::OUTPUT,Gpio::LOW );
Gpio g_segmento_b( Gpio::PORT0,18,Gpio::PUSHPULL,Gpio::OUTPUT,Gpio::LOW );
Gpio g_segmento_c( Gpio::PORT0,20,Gpio::PUSHPULL,Gpio::OUTPUT,Gpio::LOW );
Gpio g_segmento_d( Gpio::PORT0,22,Gpio::PUSHPULL,Gpio::OUTPUT,Gpio::LOW );
Gpio g_segmento_e( Gpio::PORT0, 0,Gpio::PUSHPULL,Gpio::OUTPUT,Gpio::LOW );
Gpio g_segmento_f( Gpio::PORT1,19,Gpio::PUSHPULL,Gpio::OUTPUT,Gpio::LOW );
Gpio g_segmento_g( Gpio::PORT1, 5,Gpio::PUSHPULL,Gpio::OUTPUT,Gpio::LOW );
Gpio g_segmento_dp( Gpio::PORT0,19,Gpio::PUSHPULL,Gpio::OUTPUT,Gpio::LOW );
Gpio *g_uC_segmentos[] = { &g_segmento_a,&g_segmento_b ,&g_segmento_c,&g_segmento_d ,
&g_segmento_e,&g_segmento_f,&g_segmento_g,&g_segmento_dp};
uint8_t TablaDigitosBCD7seg[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07, 0x7f,0x6f};
uC_Segmentos uC_segmentos( g_uC_segmentos , TablaDigitosBCD7seg );

Gpio g_dgt0( Gpio::PORT1 , 8 , Gpio::PUSHPULL , Gpio::OUTPUT , Gpio::HIGH );
Gpio g_dgt1( Gpio::PORT1 , 7 , Gpio::PUSHPULL , Gpio::OUTPUT , Gpio::HIGH );
Gpio g_dgt2( Gpio::PORT1 , 6 , Gpio::PUSHPULL , Gpio::OUTPUT , Gpio::HIGH );
Gpio g_dgt3( Gpio::PORT1 , 20 , Gpio::PUSHPULL , Gpio::OUTPUT , Gpio::HIGH );
Gpio g_dgt4( Gpio::PORT1 , 21 , Gpio::PUSHPULL , Gpio::OUTPUT , Gpio::HIGH );
Gpio g_dgt5( Gpio::PORT1 , 16 , Gpio::PUSHPULL , Gpio::OUTPUT , Gpio::HIGH );
Gpio g_dgt6( Gpio::PORT0 , 30 , Gpio::PUSHPULL , Gpio::OUTPUT , Gpio::HIGH );
Gpio g_dgt7( Gpio::PORT0 , 21 , Gpio::PUSHPULL , Gpio::OUTPUT , Gpio::HIGH );
Gpio *g_uC_barrido[]={&g_dgt0,&g_dgt1,&g_dgt2,&g_dgt3,&g_dgt4,&g_dgt5,&g_dgt6,&g_dgt7};
uC_Barrido uC_barrido( g_uC_barrido );

GrupoDeDigitos Grupos[] =
{
{ 0 , 4 },
{-1 , -1 }
};

Display7Segmentos Display(&uC_segmentos , &uC_barrido , Grupos );

void handlers(void);
void actualizarDisplay(void);

Timer T_Display(Timer::SEG, actualizarDisplay);

volatile uint32_t tiempoDisplay = 0;
int main(void) {
       SysTick_InstalarCallBack(handlers);
       SysTick_Inicializar(1, SYSTICK_MS);
	Potencia potencia = MAXIMA;
	uint8_t tecla = NO_KEY;
	uint32_t tiempoInicial = 0;
	uint32_t tiempo = 0;
	T_Display.TimerStart(1);
	while (1) {
		tiempoDisplay = tiempo;

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
				iniciarCalentamiento(potencia, &tiempo, calentador_1, calentador_2, calentador_3);
				tiempoInicial = tiempo;
				estadoSeteo = CALENTANDO;
			} else if (cancelarPulsado(tecla)) {
				reset(potencia, tiempo, calentador_1, calentador_2,
						calentador_3, Sirena);
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
				reset(potencia, tiempo, calentador_1, calentador_2,
						calentador_3, Sirena);
				estadoSeteo = INICIO;
			}
			break;

		case TIEMPO:
			if (numSeleccionado(tecla) && tiempo <= T_MAX) {
				tiempo *= 10;
				incrementarTiempo(tecla, tiempo);
			} else if (encendidoPulsado(tecla) && tiempo <= T_MAX && tiempo >= T_MIN) {
				iniciarCalentamiento(potencia, &tiempo, calentador_1,
						calentador_2, calentador_3);
				tiempoInicial = tiempo;
				estadoSeteo = CALENTANDO;
			} else if (tiempo > T_MAX) {
				resetTiempo(tiempo);
			} else if (cancelarPulsado(tecla)) {
				reset(potencia, tiempo, calentador_1, calentador_2,
						calentador_3, Sirena);
				estadoSeteo = INICIO;
			}
			break;

		case CALENTANDO:

			if (cancelarPulsado(tecla) || !tiempo) {
				reset(potencia, tiempo, calentador_1, calentador_2,
						calentador_3, Sirena);
				estadoSeteo = INICIO;
			} else if ((termostato.GetPin() == Gpio::OFF) && ((tiempoInicial - tiempo) >= 30)) {
				estadoSeteo = ALARMA;
				tempAlarma.TimerStart(5, nullptr, Timer::SEG);
				tempSirena.TimerStart(1, nullptr, Timer::SEG);

			}
			break;

		case ALARMA:
			if(tempAlarma == 0){
				reset(potencia, tiempo, calentador_1, calentador_2, calentador_3, Sirena);
				estadoSeteo = INICIO;
			}else if(tempSirena == 0){
				tempSirena.TimerStart(1, nullptr, Timer::SEG);
				Sirena.SetTogglePin();
			}
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
	T_Display.TmrEvent();
}

void actualizarDisplay(void){
	T_Display.TimerStart(1);
	Display.SetDisplay(tiempoDisplay, Display7Segmentos::DSP0);
}

