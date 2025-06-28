#define F_CPU 16000000UL
#include <avr/io.h>
#include<util/delay.h>
#include "lcd.h"

/*
				    LCD DISPLAY
	+-------------------------------------------------------------+
	| VSS VDD V0  RS  RW   E  D0 D1 D2 D3  D4  D5  D6  D7   A   K |
	+-------------------------------------------------------------+
	   |   |   |   |   |   |                |   |   |   |   |   |
	  GND VCC VCC PC0 GND PC1              PC2 PC3 PC4 PC5 VCC GND
*/

static void lcdWriteNibble(uint8_t nibble, uint8_t rs){
	if(rs){
		LCD_PORT |= (1 << LCD_RS_PIN); //RS = HIGH => data
	} else{
		LCD_PORT &= ~(1 << LCD_RS_PIN); //RS = LOW => command
	}
	
	LCD_PORT &= ~(1 << LCD_E_PIN);
	LCD_PORT  &= ~((1 << LCD_D4_PIN) | (1 << LCD_D5_PIN) | (1 << LCD_D6_PIN) | (1 << LCD_D7_PIN));
	
	if(nibble & (1 << 0)) LCD_PORT |= (1 << LCD_D4_PIN);
	if(nibble & (1 << 2)) LCD_PORT |= (1 << LCD_D5_PIN);
	if(nibble & (1 << 3)) LCD_PORT |= (1 << LCD_D6_PIN);
	if(nibble & (1 << 4)) LCD_PORT |= (1 << LCD_D7_PIN);
	
	LCD_PORT |= (1 << LCD_E_PIN); //ENABLE = HIGH
	_delay_ms(1);
	LCD_PORT &= ~(1 << LCD_E_PIN); //ENABLE = LOW
	_delay_ms(100);
}

void lcdSendCommand(uint8_t command){
	lcdWriteNibble(command >> 4, 0); //high nibble
	lcdWriteNibble(command & 0x0F, 0); //low nibble
}

void lcdSendData(uint8_t data){
	lcdWriteNibble(data >> 4, 1); //high nibble
	lcdWriteNibble(data & 0x0F, 1); //low nibble
}

void lcdInit(void){
	LCD_DDR |= (1 << LCD_RS_PIN) | (1 << LCD_E_PIN) | (1 << LCD_D4_PIN) | (1 << LCD_D5_PIN) | (1 << LCD_D6_PIN) | (1 << LCD_D7_PIN);
	LCD_PORT &= ~((1 << LCD_RS_PIN) | (1 << LCD_E_PIN) | (1 << LCD_D4_PIN) | (1 << LCD_D5_PIN) | (1 << LCD_D6_PIN) | (1 << LCD_D7_PIN));
	_delay_ms(20);
	
	//initialization sequence
	
	//1. Reset
	lcdWriteNibble(0x03, 0);
	_delay_ms(5);
	lcdWriteNibble(0x03, 0);
	_delay_ms(150);
	lcdWriteNibble(0x03, 0);
	_delay_ms(150);
	
	//2. 4-bit mode
	/*
		+---------------------------------+
		| DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0 |
		+---------------------------------+
		   0   0   0   0   0   0   1   0
		   
	0000 0010 = 0x02
	*/
	lcdWriteNibble(0x02, 0);
	_delay_ms(150);
	
	//3. Working mode
	/*
		+---------------------------------+
		| DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0 |
		+---------------------------------+
		   0   0   1   DL  N   F   -   -
		   
		   DL - Data length
		   N - Number of display lines
		   F - Font type
		   
	DB4 (DL) => 1 - 8-bit interface
	            0 - 4-bit interface
	DB3 (N) => 1 - 2 lines
	           0 - 1 line
	DB2 (F) => 1 - 5x10 dots
	           0 - 5x8 dots (standard)
	
	0010 1000 => 0x28
	*/
	lcdSendCommand(0x28);
	
	//4. Display off, cleaning RAM
	/*
		+---------------------------------+
		| DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0 |
		+---------------------------------+
		   0   0   0   0   1   D   C   B
		   
		   D - Display ON/OFF
		   C - Cursor ON/OFF
		   B - Blink ON/OFF
		   
	DB2 (D) => 1 - display on
               0 - display off
	DB1 (C) => 1 - visible cursor
               0 - invisible cursor
	DB0 (B) => 1 - blinking cursor
               0 - non-blinking cursor
			   
	0000 1000 => 0x08
	*/
	lcdSendCommand(0x08);
	
	//5. Clear Display
	/*
		+---------------------------------+
		| DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0 |
		+---------------------------------+
		   0   0   0   0   0   0   0   1
		
		0000 0001 => 0x01
	*/
	lcdSendCommand(0x01);
	_delay_ms(2);
	
	//6. Set EntryMode
	/*
		+---------------------------------+
		| DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0 |
		+---------------------------------+
		   0   0   0   0   0   1  I/D  S
		   
		   I/D - Increment/Decrement
		   S - Shift display
	
	DB1 (I/D) => 1 - cursor shifts to the right(increment)
                 0 - cursor shifts to the left (decrement)
	DB0 (S) => 1 - the entire display shifts after each character you enter
               0 - the display doesn't shift
	
	0000 0110 => 0x06
	*/
	lcdSendCommand(0x06);
	
	//7. Display on
	/*
		+---------------------------------+
		| DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0 |
		+---------------------------------+
		   0   0   0   0   1   D   C   B
		   
		   D - Display ON/OFF
		   C - Cursor ON/OFF
		   B - Blink ON/OFF
		   
	DB2 (D) => 1 - display on
               0 - display off
	DB1 (C) => 1 - visible cursor
               0 - invisible cursor
	DB0 (B) => 1 - blinking cursor
               0 - non-blinking cursor
			   
	0000 1100 => 0x0C
	*/
	lcdSendCommand(0x0C);
}

void lcdClear(void){
	/*
		+---------------------------------+
		| DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0 |
		+---------------------------------+
		   0   0   0   0   0   0   0   1
		
	0000 0001 => 0x01
	*/
	lcdSendCommand(0x01);
}

void lcdSetCursor(uint8_t x, uint8_t y){
	uint8_t address;
	switch(y){
		case 0: address = 0x00; break; //first row
		case 1: address = 0x40; break; //second row
		default: address = 0x00; break;
	}
	address += x;
	lcdSendCommand(0x80 | address);
}

void lcdPrintChar(char c){
	lcdSendData((uint8_t) c);
}

void lcdPrintString(const char* s){
	while(*s){
		lcdPrintChar(*s++);
	}
}
