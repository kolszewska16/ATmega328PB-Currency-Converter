#ifndef LCD_H_
#define LCD_H_

#include<avr/io.h>

#define LCD_PORT PORTC
#define LCD_DDR DDRC

#define LCD_RS_PIN PC0
#define LCD_E_PIN PC1

#define LCD_D4_PIN PC2
#define LCD_D5_PIN PC3
#define LCD_D6_PIN PC4
#define LCD_D7_PIN PC5

#define LCD_COL 16
#define LCD_ROW 2

void lcdInit(void);
void lcdClear(void);
void lcdSetCursor(uint8_t x, uint8_t y);
void lcdPrintChar(char c);
void lcdPrintString(const char* s);
void lcdSendCommand(uint8_t command);
void lcdSendData(uint8_t data);
#endif