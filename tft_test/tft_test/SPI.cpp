/*
 * SPI.cpp
 *
 * Created: 5-12-2018 10:32:33
 *  Author: joost
 */ 

#include "SPI.h"

volatile uint8_t *CS_stateReg;
volatile uint8_t *CS_portReg;
volatile uint8_t CS_pinNumber;

volatile bool spi_available = 0;

SPI::SPI(volatile uint8_t *CS_state,volatile uint8_t *CS_PORT, uint8_t CS_portNumber){
	CS_stateReg = CS_state;
	CS_portReg = CS_PORT;
	CS_pinNumber = CS_portNumber;
	
}
void SPI::transmit(uint8_t b){
	/* Start transmission */
	SPDR = b;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)))
	;
	
}
void SPI::transmit16(uint16_t i){
	transmit((i) >> 8);
	transmit(i);
}
void SPI::transmit32(uint32_t l){
	transmit((l) >> 24);
	transmit((l) >> 16);
	transmit((l) >> 8);
	transmit(l);
}
void SPI::init(){
	//set cs port to output
	
	//DDRB |= (1 << CS_pin);
	//PORTB &= ~(1 << CS_pin);
	*CS_stateReg |= (1 << CS_pinNumber);
	*CS_portReg &= ~(1 << CS_pinNumber);
	
	
	
	/* Set SS, MOSI and SCK output, all others input */
	DDRB |= (1<<PORTB3)|(1<<PORTB5);
	// set miso to input
	DDRB &= ~(1<<PORTB4);
	/* Enable SPI, Master, set clock rate fck/4 enable interrupt */
	SPCR |= (1<<SPE)|(1<<MSTR)|(1<<SPIE);
	SPCR &= ~(1 << SPR0);
	SPCR &= ~(1 << SPR1);
	
	//enable interrupts for data transfer
	//sei();
}
void SPI::beginTransmission(){
	//set chip select to low (active) for this spi instance
	*CS_portReg &= ~(1 << CS_pinNumber);
}
void SPI::endTransmission(){
	//set chip select to HIGH (deaf) for this spi instance
	
	*CS_portReg |= (1 << CS_pinNumber);
}
uint8_t SPI::read(uint8_t out){
	/* Start transmission */
	SPDR = out;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)))
	;
	//read data
	return SPDR;
}

