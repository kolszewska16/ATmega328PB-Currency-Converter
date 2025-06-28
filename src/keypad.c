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
			COLUMNS: COL1 - COL4 => PB0 - PB3
			ROWS: ROW1 - ROW4 => PD4 - PD7
*/

const char keypad_map[4][4] = {
	{'1', '2', '3', KEY_USD},
	{'4', '5', '6', KEY_EURO},
	{'7', '8', '9', KEY_GBP},
	{KEY_CLEAR, '0', KEY_DECIMAL, KEY_PLN}
};

void keypadInit(void){
	KEYPAD_C_DDR |= (1 << C1_PIN) | (1 << C2_PIN) | (1 << C3_PIN) | (1 << C4_PIN);
	KEYPAD_C_PORT |= (1 << C1_PIN) | (1 << C2_PIN) | (1 << C3_PIN) | (1 << C4_PIN);
	
	KEYPAD_R_DDR &= ~((1 << R1_PIN) | (1 << R2_PIN) | (1 << R3_PIN) | (1 << R4_PIN));
	KEYPAD_R_PORT |= (1 << R1_PIN) | (1 << R2_PIN) | (1 << R3_PIN) | (1 << R4_PIN);
}

char getKey(void){
	uint8_t rows[] = {R1_PIN, R2_PIN, R3_PIN, R4_PIN};
	uint8_t columns[] = {C1_PIN, C2_PIN, C3_PIN, C4_PIN};
	
	//scaning columns
	for(uint8_t col_idx = 0; col_idx < 4; col_idx++){
		KEYPAD_C_PORT |= (1 << C1_PIN) | (1 << C2_PIN) | (1 << C3_PIN) | (1 << C4_PIN);
		KEYPAD_C_PORT &= ~(1 << columns[col_idx]);
		
		_delay_ms(10);
		
		for(uint8_t row_idx = 0; row_idx < 4; row_idx++){
			if(!(KEYPAD_R_PIN & (1 << rows[row_idx]))){
				char pressed_key = keypad_map[row_idx][col_idx];
				
				while(!(KEYPAD_R_PIN & (1 << rows[row_idx]))){
					_delay_ms(10);
				}
				_delay_ms(50);
				return pressed_key;
			}
		}
	}
	return KEY_NONE;
}