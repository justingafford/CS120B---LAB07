/*	Partner 1's Name & E-mail: Justin Gafford jgaff001@ucr.edu
 *  Partner 2's Name & E-mail: Wang Sitao 934415487@qq.com
 *	Lab Section: 022
 *	Assignment: Lab #7 Exercise #1
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include "io.c"
#include <avr/interrupt.h>

#define b0 (~PINA & 0x01)
//#define b1 (~PINA & 0x02)

enum states {pc0_on,wait1,pc1_on,wait2,pc2_on,wait3} state;
//Timer code from lab 6
volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
unsigned char tmpA = 0x00;
unsigned char tmpC = 0x00;
unsigned char score = 0x00;
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

void light_game() {
	switch(state) {
		case pc0_on:
			if(b0) {
				state = wait1;
				if(score > 0) {
					score = score - 1;
				}
			}
			else {
				state = pc1_on;
			}
			break;
		case wait1:
			if(b0) {
				state = pc0_on;
			}
			else {
				state = wait1;
			}
			break;
		case pc1_on:
			if(b0) {
				state = wait2;
				if(score < 9) {
					score = score + 1;
				}
			}
			else {
				state = pc2_on;
			}
			break;
		case wait2:
			if(b0) {
				state = pc1_on;
			}
			else {
				state = wait2;
			}
			break;
		case pc2_on:
			if(b0) {
				state = wait3;
				if(score > 0) {
					score = score - 1;
				}
			}
			else {
				state = pc0_on;
			}
			break;
		case wait3:
			if(b0) {
				state = pc2_on;
			}
			else {
				state = wait3;
			}
			break;
		default:
			state = pc0_on;
			break;
	}
	switch(state) {
		case pc0_on:
			tmpC = 0x01;
			break;
		case wait1:
			tmpC = 0x01;
			break;
		case pc1_on:
			tmpC = 0x02;
			break;
		case wait2:
			tmpC = 0x02;
			break;
		case pc2_on:
			tmpC = 0x04;
			break;
		case wait3:
			tmpC = 0x04;
			break;
	}
}

int main () {
	DDRA = 0x00; PORTA = 0x00; // user inputs
	DDRB = 0xFF; PORTB = 0x00;  // interface outputs
	DDRC = 0xFF; PORTC = 0x00; // DATA LINES
	DDRD = 0xFF; PORTD = 0x00;  // CONTROL LINES
	
	TimerSet(300);
	TimerOn();
	state = pc0_on;
	LCD_init();
	LCD_ClearScreen();
	score = 0x05;
	
	while(1) {
			LCD_Cursor(1);
			if (score == 9) {
				LCD_DisplayString(1, "Game won."); 
			}
			else {
				LCD_WriteData(score + '0');
			}
			light_game();
			while(!TimerFlag) {}
			TimerFlag = 0;
			PORTB = tmpC;
	}
	return 1;
}

