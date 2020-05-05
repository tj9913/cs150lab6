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
unsigned int j = 0;
unsigned char tmpA = 0x00;
enum states{ start, pb0, pb1, pb2, hold, release} state;

void tick(){
	switch(state){
		case start:
			state = pb0;
			break;
		case pb0:
			if(tmpA == 0x01 && j%2 == 0){
				state = hold;
			}
			else{
				state = pb1;
			}
			if(j%2 == 1){
				if(tmpA == 0){
					j++;
				}
			}
			break;
		case pb1: 
			if(tmpA == 0x01 && j%2 == 0){
                                state = hold;
                        }
			
                        else{
                                if(i%2 == 0){
					state = pb2;
					i++;
				}
				else{
					state = pb0;
					i++;
				}
		
                        }
			  if(j%2 == 1){
                                if(tmpA == 0){
                                        j++;
                                }
                        }

			break;
		case pb2:
			if(tmpA == 0x01 && j%2 == 0){
                                state = hold;
                        }
                        else{
                                state = pb1;
                        }
			  if(j%2 == 1){
                                if(tmpA == 0){
                                        j++;
                                }
                        }

			break;
		case hold:
			if(tmpA == 0x01){
				state = hold;
			}
			else{
				state = release;
			}
			break;
		case release:
			if(tmpA == 0x01){
				state = pb0;
				i = 0;
				j++;
			}
			else{
				state = release;
			}
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
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF;//set port B to output
    PORTB = 0x00;
    TimerSet(300);
    TimerOn();
    state = start;
    /* Insert your solution below */
    while (1) {
	tmpA = ~PINA;
	tick();
	while(!TimerFlag){}
	TimerFlag = 0;
    }
    return 1;
}
