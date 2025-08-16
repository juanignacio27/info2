/*
 * driver.h
 *
 *  Created on: 11 ago. 2025
 *      Author: ojuan
 */

#ifndef _DRIVER_H_
#define _DRIVER_H_

#include "tipos.h"

class Puerto {
private:
#define CANT_BITS 8
#define OFFSET_REGISTROS (0x40000000UL)
#define ERROR ((REG->STATUS >> 3) & 1)

	typedef struct {
		volatile uint32_t CONFIG;
		volatile uint32_t CONTROL;
		volatile uint32_t STATUS;
		volatile uint32_t TXDATA;
		volatile uint32_t RXDATA;
		volatile uint32_t INTERRUPT;
	} registros;

#define REG ( (registros *) OFFSET_REGISTROS)

public:
	Puerto();
	bool configure(unsigned int dataBits);
	bool sendByte(unsigned char byte);
	bool receiveByte(unsigned char &byte);
	bool sendString(const char *str);
	bool receiveString(char *buffer, unsigned int bufferSize);
	bool isTxReady() const;
	bool isRxReady() const;
	void enableTxInterrupt();
	void disableTxInterrupt();
	void enableRxInterrupt();
	void disableRxInterrupt();
	void clearInterrupts();
	bool getTxInterruptStatus();
	bool getRxInterruptStatus();

};

#endif /* SRC_2_APLICACION_DRIVER_H_ */
