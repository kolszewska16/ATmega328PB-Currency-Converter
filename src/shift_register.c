#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#include "shift_register.h"    
#include "logic.h"             

/*
		   SHIFT REGISTER 74HC595N
		       +------------+
		B   <= |QB       VCC| =>   VCC
		C   <= |QC        QA| =>   A
		D   <= |QD        SI| =>   PB2
		E   <= |QE        ~G| =>   GND
		F   <= |QF       RCK| =>   PB1
		G   <= |QG       SCK| =>   PB0
		H   <= |QH     ~SCLR| =>   VCC
		GND <= |GND      QH'| 
		       +------------+
			   
				     SEVEN-SEGMENT DISPLAY
		+----------------------------------------------------------+
		| VCC  D1   D2   D3   D4   A   B   C   D   E   F   G   DP  |
		+----------------------------------------------------------+
		   |    |    |    |    |   |   |   |   |   |   |   |    |
		  VCC  PE1  PE2  PE3  PE4  QA  QB  QC  QD  QE  QF  QG   QH
		
*/

uint8_t sevenSegmentDigits[4] = {10, 10, 10, 10};
int8_t sevenSegmentDotPosition = -1;

const uint8_t segment_map[11] = {
	0b11000000,     //0
	0b11111001,     //1
	0b10100100,     //2
	0b10110000,     //3
	0b10011001,     //4
	0b10010010,     //5
	0b10000010,     //6
	0b11111000,     //7
	0b10000000,     //8
	0b10010000,     //9
	0b11111111      //10
};


void shiftRegisterInit(void){
	DATA_DDR |= (1 << DATA_PIN);
	CLOCK_DDR |= (1 << CLOCK_PIN);
	LATCH_DDR |= (1 << LATCH_PIN);
	DIGIT_DDR |= (1 << DIGIT0_PIN) | (1 << DIGIT1_PIN) | (1 << DIGIT2_PIN) | (1 << DIGIT3_PIN);
	
	//LATCH & CLOCK
	LATCH_PORT &= ~(1 << LATCH_PIN);
	CLOCK_PORT &= ~(1 << CLOCK_PIN);

	DIGIT_PORT |= (1 << DIGIT0_PIN) | (1 << DIGIT1_PIN) | (1 << DIGIT2_PIN) | (1 << DIGIT3_PIN);
}

void shiftOut(uint8_t data){
	LATCH_PORT &= ~(1 << LATCH_PIN); //RCK = 0
	_delay_us(10);

	for(uint8_t i = 0; i < 8; i++){
		if(data & (1<<(7 - i))){
			DATA_PORT |= (1 << DATA_PIN);
		} else{
			DATA_PORT &= ~(1 << DATA_PIN);
		}

		CLOCK_PORT |= (1 << CLOCK_PIN); //SCK = 1
		_delay_us(1); 
		CLOCK_PORT &= ~(1 << CLOCK_PIN); //SCK = 0
		_delay_us(1); 
	}

	LATCH_PORT |= (1 << LATCH_PIN);
	_delay_us(10);
}

void selectDigit(uint8_t digit_index){
	DIGIT_PORT |= (1 << DIGIT0_PIN) | (1 << DIGIT1_PIN) | (1 << DIGIT2_PIN) | (1 << DIGIT3_PIN);

	switch(digit_index){
		case 0: DIGIT_PORT &= ~(1 << DIGIT0_PIN); break;
		case 1: DIGIT_PORT &= ~(1 << DIGIT1_PIN); break;
		case 2: DIGIT_PORT &= ~(1 << DIGIT2_PIN); break;
		case 3: DIGIT_PORT &= ~(1 << DIGIT3_PIN); break;
	}
}

void displayMultiplexedNumber(){
	static uint8_t currentDigitIndex = 0;
	uint8_t segmentData;

	DIGIT_PORT |= (1 << DIGIT0_PIN) | (1 << DIGIT1_PIN) | (1 << DIGIT2_PIN) | (1 << DIGIT3_PIN);

	if(sevenSegmentDigits[currentDigitIndex] <= 9){
		segmentData = segment_map[sevenSegmentDigits[currentDigitIndex]];
	} else{
		segmentData = segment_map[10];
	}
	
	if(sevenSegmentDotPosition != -1){
		if(currentDigitIndex == (4 - sevenSegmentDotPosition - 1)){
			segmentData &= ~(1 << DP_BIT);
		}
	}
	
	shiftOut(segmentData);
	selectDigit(currentDigitIndex);
	_delay_us(1000);
	currentDigitIndex++;
	if(currentDigitIndex >= 4){
		currentDigitIndex = 0;
	}
}

void setSevenSegmentNumber(int32_t number, int8_t dot_pos) {
	if(number < 0 || number > 9999){
		sevenSegmentDigits[0] = 10;
		sevenSegmentDigits[1] = 10;
		sevenSegmentDigits[2] = 10;
		sevenSegmentDigits[3] = 10;
		sevenSegmentDotPosition = -1;
		return;
	}

	for(int i = 3; i >= 0; i--){
		sevenSegmentDigits[i] = number % 10;
		number /= 10; 

		if(number == 0 && i > 0){
			for(int j = 0; j < i; j++){
				sevenSegmentDigits[j] = 10;
			}
			break;
		}
	}
	sevenSegmentDotPosition = dot_pos;
}

void clearSevenSegments() {
	sevenSegmentDigits[0] = 10;
	sevenSegmentDigits[1] = 10;
	sevenSegmentDigits[2] = 10;
	sevenSegmentDigits[3] = 10;
	sevenSegmentDotPosition = -1;
}