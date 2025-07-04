#ifndef SHIFT_REGISTER_H
#define SHIFT_REGISTER_H

#include <avr/io.h>

#define DIGIT_PORT PORTE
#define DIGIT_DDR DDRE

#define DIGIT0_PIN PE0
#define DIGIT1_PIN PE1
#define DIGIT2_PIN PE2
#define DIGIT3_PIN PE3

#define DATA_PORT PORTB
#define DATA_DDR DDRB
#define DATA_PIN PB2

#define CLOCK_PORT PORTB
#define CLOCK_DDR DDRB
#define CLOCK_PIN PB0

#define LATCH_PORT PORTB
#define LATCH_DDR DDRB
#define LATCH_PIN PB1

#define DP_BIT 7

extern const uint8_t segment_map[11];

extern uint8_t sevenSegmentDigits[4];      
extern int8_t sevenSegmentDotPosition;    

void shiftOut(uint8_t data);
void shiftRegisterInit(void);
void selectDigit(uint8_t digit_index);
void displayMultiplexedNumber(void);
void setSevenSegmentNumber(int32_t number, int8_t dot_pos);
void clearSevenSegments(void);

#endif