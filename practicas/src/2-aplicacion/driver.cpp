/*
 * driver.cpp
 *
 *  Created on: 11 ago. 2025
 *      Author: ojuan
 */
#include "driver.h"

Puerto::Puerto() {
}

bool Puerto::configure(unsigned int dataBits) {
	bool invalido = true;
	if (dataBits >= 5 && dataBits <= 8) {
		invalido = false;
		uint32_t CFG_DATABITS_SHIFT = 6;
		uint32_t CFG_DATABITS_MASK  = (0b11u << CFG_DATABITS_SHIFT);
		REG->CONFIG &= ~CFG_DATABITS_MASK;
		REG->CONFIG |= (dataBits - 5) << CFG_DATABITS_SHIFT;
	}
	return invalido;
}

bool Puerto::sendByte(unsigned char byte) {
	bool error = false;
	REG->TXDATA = byte;
	REG->STATUS = 1u;
	int cont = 0;
	while(!isTxReady() && cont < 1000){
		cont++;
	}
	if(cont >= 1000 || ERROR)
		error = true;
	return error;
}

bool Puerto::receiveByte(unsigned char &byte){
	bool error = false;
	int contStrobe = 0;
	while(!isRxReady() && contStrobe < 1000){
		contStrobe++;
	}
	if(contStrobe >= 1000 || ERROR){
		error = true;
	}
	else{
	byte = REG->RXDATA;
	}
	return error;
}

bool Puerto::sendString(const char *str){
	bool error = false;
	int len;
	for(len = 0; str[len] != '\0'; len++);
	for(int i = 0; i <= (len + 1) && !error; i++){
		if(!sendByte(str[i])){
			error = true;
		}
	}
	return error;
}

bool Puerto::receiveString(char *buffer, unsigned int bufferSize){
	bool error = false;
	unsigned int lenght;
	for(lenght = 0; REG->RXDATA != '\0' && lenght < bufferSize; lenght++){
		if(receiveByte((unsigned char&)buffer[lenght]))
			error = true;
	}
	if(lenght + 1 < bufferSize && buffer[lenght] != '\0' && !error){
		lenght++;
		buffer[lenght] = '\0';
	}
	return error;
}
bool Puerto::isTxReady() const{
	bool ACK = REG->STATUS & (1u << 1);
	return ACK != 0;
}

bool Puerto::isRxReady() const{
	bool STROBE = REG->STATUS & 1u;
	return STROBE;
}

void Puerto::enableTxInterrupt(){
	REG->CONTROL |= (1u << 2);
}

void Puerto::enableRxInterrupt(){
	REG->CONTROL |= (1u << 3);
}

void Puerto::disableTxInterrupt(){
	uint32_t mask = (1u << 2);
	mask = ~mask;
	REG->CONTROL &= mask;
}

void Puerto::disableRxInterrupt(){
	uint32_t mask = (1u << 3);
	mask = ~mask;
	REG->CONTROL &= mask;
}

void Puerto::clearInterrupts(){
	uint32_t mask = (1u << 2);
	REG->INTERRUPT = mask;
}

bool Puerto::getTxInterruptStatus(){
	return ((REG->INTERRUPT & 1u) != 0u);
}

bool Puerto::getRxInterruptStatus(){
	return ((REG->INTERRUPT & (1u << 1)) != 0u);
}


