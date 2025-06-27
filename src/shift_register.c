#define F_CPU 16000000UL
#include <avr/io.h>
#include<util/delay.h>
#include "shift_register.h"	

const uint8_t segment_map[10] = {
	0b11000000,	//0
	0b11111001,	//1
	0b10100100,	//2
	0b10110000,	//3
	0b10011001,	//4
	0b10010010,	//5
	0b10000010,	//6
	0b11111000,	//7
	0b10000000,	//8
	0b10010000	//9
};

void shiftRegisterInit(void){
	DATA_DDR |= (1 << DATA_PIN);
	CLOCK_DDR |= (1 << CLOCK_PIN);
	LATCH_DDR |= (1 << LATCH_PIN);
	DIGIT_DDR |= (1 << DIGIT0_PIN) | (1 << DIGIT1_PIN) | (1 << DIGIT2_PIN) | (1 << DIGIT3_PIN);
	DIGIT_PORT |= (1 << DIGIT0_PIN) | (1 << DIGIT1_PIN) | (1 << DIGIT2_PIN) | (1 << DIGIT3_PIN);
}

void shiftOut(uint8_t data){
	for(uint8_t i = 0; i < 8; i++){
		if(data & (1<<(7 - i))){
			DATA_PORT |= (1 << DATA_PIN);
		}
		else{
			DATA_PORT &= ~(1 << DATA_PIN);
		}
		
		//clock - high
		DATA_PORT |= (1 << CLOCK_PIN);
		
		//clock - low
		DATA_PORT &= ~(1 << CLOCK_PIN);
	}
	
	//latch
	DATA_PORT |= (1 << LATCH_PIN);
	DATA_PORT &= ~(1 << LATCH_PIN);
}

//0 - 3
void selectDigit(uint8_t digit_index){
	DIGIT_PORT |= (1 << DIGIT0_PIN) | (1 << DIGIT1_PIN) | (1 << DIGIT2_PIN) | (1 << DIGIT3_PIN);
	switch(digit_index){
		case 0: DIGIT_PORT &= ~(1 << DIGIT0_PIN); break;
		case 1: DIGIT_PORT &= ~(1 << DIGIT1_PIN); break;
		case 2: DIGIT_PORT &= ~(1 << DIGIT2_PIN); break;
		case 3: DIGIT_PORT &= ~(1 << DIGIT3_PIN); break;
	}
}

void displayNumber(uint16_t number){
	uint8_t digits[4];
	digits[0] = number / 1000;
	digits[1] = (number / 100) % 10;
	digits[2] = (number / 10) % 10;
	digits[3] = number % 10;
	
	for(uint8_t i = 0; i < 4; i++){
		shiftOut(segment_map[digits[i]]);
		selectDigit(i);
		_delay_ms(1);
	}
}
	
void displayNumberWithDot(uint16_t number, uint8_t dot_position){
	uint8_t digits[4];
	digits[0] = number / 1000;
	digits[1] = (number / 100) % 10;
	digits[2] = (number / 10) % 10;
	digits[3] = number % 10;
	
	for(uint8_t i = 0; i < 4; i++){
		uint8_t segment_data = segment_map[digits[i]];
		if(i == dot_position){
			segment_data &= ~(1 << 7);
		}
		shiftOut(segment_data);
		selectDigit(i);
		_delay_ms(2);
	}
}