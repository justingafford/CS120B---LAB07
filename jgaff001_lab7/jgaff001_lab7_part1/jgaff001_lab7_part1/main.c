/*	Partner 1's Name & E-mail: Justin Gafford jgaff001@ucr.edu
 *  Partner 2's Name & E-mail: Wang Sitao 934415487@qq.com
 *	Lab Section: 022
 *	Assignment: Lab #6 Exercise #1
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include "io.c"
#include <avr/interrupt.h>

#define b0 (~PINA & 0x01)
#define b1 (~PINA & 0x02)

enum states {wait,incr,decr,zero} state;
//Timer code from lab 6
volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
unsigned char tmpA = 0x00;
unsigned char tmpC = 0x00;
void TimerOn(){
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}

void TimerOff(){
	TCCR1B = 0X00;
}

void TimerISR(){
	TimerFlag = 1;
}
ISR(TIMER1_COMPA_vect){
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0){
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M){
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

unsigned char tmpC;

void incrDecr() {
	switch(state) {
		case wait:
			if(b0 && b1) {
				state = zero;
			}
			else if (!b0 && b1) {
				state = decr;
			}
			else if(b0 && !b1) {
				state = incr;
			}
			else {
				state = wait;
			}
			break;
		case incr:
			if(b0 && b1) {
				state = zero;
			}
			else if (!b0 && b1) {
				state = incr;
			}
			else {
				state = wait;
			}
			break;
		case decr:
			if(b0 && b1) {
				state = zero;
			}
			else if (!b0 && b1) {
				state = decr;
			}
			else {
				state = wait;
			}
			break;
		case zero:
			if(b0 && b1) {
				state = zero;
			}
			else {
				state = wait;
			
			}
			break;
		default:
			state = wait;
			break;
	}
	switch(state) {
		case wait:
			break;
		case incr:
			if(tmpC < 9) {
				tmpC = tmpC + 1;
			}
			break;
		case decr:
			if (tmpC > 0) {
				tmpC = tmpC - 1;
			}
			break;
		case zero:
			tmpC =0;
			break;
		
	}
}
int main(void)
{
    DDRA = 0x00; PORTA = 0xFF; //inputs
	DDRC = 0xFF; PORTC = 0x00; //outputs(data lines)
	DDRC = 0xFF; PORTC = 0x00; //outputs(control lines)
	
	TimerSet(1000);
	TimerOn();
	
	LCD_init();
	LCD_ClearScreen();
	
	state = wait;
	tmpC = 0x00;
    while (1) 
    {
		LCD_Cursor(1);
		incrDecr();
		LCD_WriteData(tmpC + '0');
		while (!TimerFlag) {}
		TimerFlag = 0;
    }
}

