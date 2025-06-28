#ifndef KEYPAD_H_
#define KEYPAD_H_

#include <avr/io.h>

//columns
#define KEYPAD_C_PORT PORTB
#define KEYPAD_C_DDR DDRB
#define KEYPAD_C_PIN PINB

//rows
#define KEYPAD_R_PORT PORTD
#define KEYPAD_R_DDR DDRD
#define KEYPAD_R_PIN PIND

//columns
#define C1_PIN PB0
#define C2_PIN PB1
#define C3_PIN PB2
#define C4_PIN PB3

//rows
#define R1_PIN PD4
#define R2_PIN PD5
#define R3_PIN PD6
#define R4_PIN PD7

#define KEY_NONE 0
#define KEY_CLEAR 'C'
#define KEY_DECIMAL '.'
#define KEY_USD 'U'
#define KEY_EURO 'E'
#define KEY_GBP 'G'
#define KEY_PLN 'P'

extern const char keypad_map[4][4];

void keypadInit(void);
char getKey(void);

#endif