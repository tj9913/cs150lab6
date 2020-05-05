/*	Author: terry
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn(){
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;

	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}

void TimerOff(){
	TCCR1B = 0x00;
}

void TimerISR(){
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect){
	_avr_timer_cntcurr--;
	if(_avr_timer_cntcurr == 0){
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M){
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}
unsigned int i = 0;
enum states{ start, pb0, pb1, pb2} state;

void tick(){
	switch(state){
		case start:
			state = pb0;
			break;
		case pb0:
			state = pb1;
			break;
		case pb1: 
			state = pb2;
			break;
		case pb2:
			state = pb0;
			break;
		default:
			state = start;
			break;
	}
	switch(state){
		case pb0:
			PORTB = 0x01;
			break;
		case pb1:
			PORTB = 0x02;
			break;
		case pb2:
			PORTB = 0x04;
			break;
		default:
			break;
	}
}
int main(void) {
    /* Insert DDR and PORT initializations */
    DDRB = 0xFF;//set port B to output
    PORTB = 0x00;
    TimerSet(1000);
    TimerOn();
    state = start;
    /* Insert your solution below */
    while (1) {
	tick();
	while(!TimerFlag){}
	TimerFlag = 0;
    }
    return 1;
}
