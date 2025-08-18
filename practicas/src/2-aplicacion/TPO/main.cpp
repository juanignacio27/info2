/*
 * main.cpp
 *
 *  Created on: 16 ago. 2025
 *      Author: ojuan
 */
#include "ultrasonico.h"
#include "Gpio.h"
#include "Systick.h"
#include "perifericotemporizado.h"

Gpio led(Gpio::B_PORT0, 18, Gpio::PUSHPULL, Gpio::HIGH, Gpio::OUTPUT);
Ultrasonico ultrasonico(Gpio::PORT0 ,16 ,Gpio::PORT0 ,17);

void Systick_Callback(void);

int main(void){
	SysTick_InstalarCallBack(Systick_Callback);
	SysTick_Inicializar(1, SYSTICK_US);

	while(1){
		ultrasonico.medir();
		if(ultrasonico.getDistancia() <= 30){
			led.SetPin();
		}else{
			led.ClrPin();
		}
	}
	return 0;
}

void Systick_Callback(void){
	for(int i = 0; i < PerifericoTemporizado::m_countPerifericosTemporizados; i++){
		g_perifericosTemporizados[i]->HandlerDelPeriferico();
	}
	ultrasonico.timerCallBacks();
}


