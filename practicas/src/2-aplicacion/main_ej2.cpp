#include "intext.h"
#include "Gpio.h"
#include "LPC845.h"

#define PIN_BOTON 4
#define PIN_LED 0

enum Estados{APAGADO, ENCENDIDO};

void botonPresionado(void);
void apagarLed(void);
void prenderLed(void);

Intext boton(Intext::PINT0, Gpio::PORT0, PIN_BOTON, botonPresionado);
Gpio led(Gpio::PORT0, PIN_LED, Gpio::PUSHPULL, Gpio::HIGH, Gpio::OUTPUT);

volatile Estados estado;
int main(){
	boton.set_flanco(Intext::ASCENDENTE);
	apagarLed();

	while(1){

	}
	return 0;
}

void botonPresionado(void){
	if(estado){
		apagarLed();
	}else{
		prenderLed();
	}
}

void apagarLed(void){
	led.ClrPin();
	estado = APAGADO;
}

void prenderLed(void){
	led.SetPin();
	estado = ENCENDIDO;
}
