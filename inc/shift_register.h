#ifndef SHIFT_REGISTER_H
#define SHIFT_REGISTER_H

#include <avr/io.h>

#define DIGIT_PORT PORTE
#define DIGIT_DDR DDRE

#define DATA_PORT PORTD
#define DATA_DDR DDRD
#define DATA_PIN PD2

#define CLOCK_PORT PORTD
#define CLOCK_DDR DDRD
#define CLOCK_PIN PD0

#define LATCH_PORT PORTD
#define LATCH_DDR DDRD
#define LATCH_PIN PD1

#define DIGIT0_PIN PE0
#define DIGIT1_PIN PE1
#define DIGIT2_PIN PE2
#define DIGIT3_PIN PE3

void shiftOut(uint8_t data);
void shiftRegisterInit(void);
void selectDigit(uint8_t digit);
void displayNumber(uint16_t number);
void displayNumberWithDot(uint16_t number, uint8_t dot_position);

#endif