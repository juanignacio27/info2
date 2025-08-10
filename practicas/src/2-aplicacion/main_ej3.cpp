#include "LPC845.h"
#include "digital_inputs.h"
#include "digital_outputs.h"
#include "Timer.h"
#include "systick.h"
#include "perifericotemporizado.h"

#define PIN_BOTON 4
#define PIN_LED 0
#define TIEMPO 1

enum Estados {
	ESTATICO, TITILAR
};
enum EstadosLed{
	APAGADO, ENCENDIDO
};

volatile Estados estado = ESTATICO;
volatile EstadosLed estadoLed = APAGADO;

DigitalInputs Boton(Gpio::PORT0, PIN_BOTON, Gpio::PULLUP, Gpio::LOW);
DigitalOutputs Led(Gpio::PORT0, PIN_LED, Gpio::PUSHPULL, Gpio::HIGH, Gpio::LOW);
Timer T(Timer::SEG);

void titilar(void);
void AtenderPerifericosTemporizados(void);

int main(void) {
	SysTick_InstalarCallBack(AtenderPerifericosTemporizados);
	SysTick_Inicializar(1);
	Led.clr();
	bool flag = true;
	while (1) {
		if (Boton) {
			if (flag) {
				estado = (estado == ESTATICO) ? TITILAR : ESTATICO;
				if(estado == TITILAR){
					T.TimerStart(TIEMPO, nullptr, Timer::SEG);
				}
				flag = false;
			}
		}
		else{
			flag = true;
		}

		if(estado == TITILAR){
			titilar();
		}
	}
	return 0;
}

void titilar(){
	if(T){
		if(estadoLed == ENCENDIDO){
			Led.clr();
			estadoLed = APAGADO;
		}else{
			Led.set();
			estadoLed = ENCENDIDO;
		}
		T.TimerStart(TIEMPO, nullptr, Timer::SEG);
	}
}

void AtenderPerifericosTemporizados(void) {
    for (uint8_t i = 0; i < PerifericoTemporizado::m_countPerifericosTemporizados; i++) {
        g_perifericosTemporizados[i]->HandlerDelPeriferico();
    }
}

