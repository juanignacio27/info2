/*
 * seteo.h
 *
 *  Created on: 10 ago. 2025
 *      Author: ojuan
 */

#ifndef SRC_2_APLICACION_EJERCICIO16_SETEO_H_
#define SRC_2_APLICACION_EJERCICIO16_SETEO_H_

#include "Timer.h"

#define TIEMPO_RAPIDO 5
#define ENCENDIDO 13
#define CANCELAR 14

enum Estados{INICIO, RAPIDO, POTENCIA, TIEMPO, CALENTANDO};
enum Potencia{MINIMA = 10, MEDIA, MAXIMA};

extern Timer temp;

void setPotencia(Potencia p, Potencia& potencia);
void incrementarTiempo(uint32_t t, uint32_t& tiempo);
void reset(Potencia& potencia, uint32_t& tiempo);
void resetTiempo(uint32_t& tiempo);
void iniciarCalentamiento(Potencia p, uint32_t tiempo);
void tempEncRapido(void);
void mostrarError();
bool estadoTempEncRapido(void);

bool potenciaSeleccionada(uint8_t tecla);
bool encendidoPulsado(uint8_t tecla);
bool numSeleccionado(uint8_t tecla);
bool cancelarPulsado(uint8_t tecla);



#endif /* SRC_2_APLICACION_EJERCICIO16_SETEO_H_ */
