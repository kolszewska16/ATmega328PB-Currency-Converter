#define F_CPU 16000000UL
#include <avr/io.h>
#include<util/delay.h>
#include "keypad.h"

/*
		KEYPAD LAYOUT
	
		+-----+-----+-----+-----+
		|  1  |  2  |  3  |  U  |
		+-----+-----+-----+-----+
		|  4  |  5  |  6  |  E  |
		+-----+-----+-----+-----+
		|  7  |  8  |  9  |  G  |
		+-----+-----+-----+-----+
		|  C  |  0  |  .  |  P  |
		+-----+-----+-----+-----+
		
		U - USD
		E - EURO
		G - GBP
		P - PLN
		C - CLEAR
		
		CONNECTIONS:
			COLUMNS: COL1 - COL4 => PD0 - PD3
			ROWS: ROW1 - ROW4 => PD4 - PD7
*/

const char keypad_map[4][4] = {
	{KEY_USD, '3', '2', '1'},
	{KEY_EURO, '6', '5', '4'},
	{KEY_GBP, '9', '8', '7'},
	{KEY_PLN, KEY_DECIMAL, '0', KEY_CLEAR}
};                  

void keypadInit(void){
	KEYPAD_DDR &= ~(1 << C1_PIN) | (1 << C2_PIN) | (1 << C3_PIN) | (1 << C4_PIN);
	KEYPAD_PORT |= (1 << C1_PIN) | (1 << C2_PIN) | (1 << C3_PIN) | (1 << C4_PIN);
	
	KEYPAD_DDR |= (1 << R1_PIN) | (1 << R2_PIN) | (1 << R3_PIN) | (1 << R4_PIN);
	KEYPAD_PORT |= (1 << R1_PIN) | (1 << R2_PIN) | (1 << R3_PIN) | (1 << R4_PIN);
}

char getKey(void){
	for(uint8_t row = 0; row < 4; row++){
		KEYPAD_PORT |= (1 << R1_PIN) | (1 << R2_PIN) | (1 << R3_PIN) | (1 << R4_PIN);
		KEYPAD_PORT &= ~(1 << (R1_PIN + row));
		
		_delay_us(10);

		for(uint8_t col = 0; col < 4; col++){
			if(!(KEYPAD_PIN & (1 << (C1_PIN + col)))){
				_delay_ms(50);
				while(!(KEYPAD_PIN & (1 << (C1_PIN + col))));
				_delay_ms(50);
				return keypad_map[row][col];
			}
		}
	}
	return KEY_NONE;
}