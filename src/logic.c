#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "shift_register.h"
#include "keypad.h"
#include "lcd.h"
#include "logic.h"

AppState currentAppState = STATE_INPUT_AMOUNT;

char inputBuffer[17];
int inputBufferIndex = 0;
double currentAmount = 0.0;
char sourceCurrency = KEY_NONE;
char targetCurrency = KEY_NONE;
char lastPressedCurrencyKey = KEY_NONE;

#define RATE_USD_PLN 4.00 //1 USD = 4.00 PLN
#define RATE_EURO_PLN 4.35 //1 EURO = 4.35 PLN
#define RATE_GBP_PLN 5.00 //1 GBP = 5.00 PLN

void clearInputBuffer(void){
	memset(inputBuffer, 0, sizeof(inputBuffer));
	inputBufferIndex = 0;
}

void resetConverter(void){
	clearInputBuffer();
	currentAmount = 0.0;
	sourceCurrency = KEY_NONE;
	targetCurrency = KEY_NONE;
	lastPressedCurrencyKey = KEY_NONE;
	currentAppState = STATE_INPUT_AMOUNT;
	displayNumber(0);
	updateDisplay();
}

void updateDisplay(void){
	char lcd_line1[LCD_COL + 1];
	char lcd_line2[LCD_COL + 1];
	
	lcdClear();
	
	switch(currentAppState){
		case STATE_INPUT_AMOUNT:
		{
			lcdPrintString("Wpisz kwotê");
			if(inputBufferIndex == 0){
				lcdPrintString("0");
				displayNumber(0);
				} else{
				lcdPrintString(inputBuffer);
				long val = (long)(atof(inputBuffer) * 100);
				int dotPositon = 0;
				char *dotPtr = strchr(inputBuffer, KEY_DECIMAL);
				if(dotPtr != NULL){
					dotPositon = strlen(dotPtr) - 1;
				}
				displayNumberWithDot(val, dotPositon);
			}
		}
		break;
		
		case STATE_SELECT_SOURCE_CURRENCY:
		{
			snprintf(lcd_line1, sizeof(lcd_line1), "Z %c %.2f", sourceCurrency, currentAmount);
			lcdPrintString(lcd_line1);
			lcdSetCursor(0, 1);
			lcdPrintString("Wybierz docelow¹:");
			long val_src = (long)(currentAmount * 100);
			displayNumberWithDot(val_src, 2);
		}
		break;
		
		case STATE_DISPLAY_RATE:
		{
			double rate = getExchangeRate(sourceCurrency, targetCurrency);
			if(rate > 0){
				snprintf(lcd_line1, sizeof(lcd_line1), "%c do %c", sourceCurrency, targetCurrency);
				snprintf(lcd_line2, sizeof(lcd_line2), "Kurs: %.4f", rate);
				lcdPrintString(lcd_line1);
				lcdSetCursor(0, 1);
				lcdPrintString(lcd_line2);
				displayNumber(0);
				} else{
				lcdPrintString("B³êdny kurs!");
				lcdSetCursor(0, 1);
				lcdPrintString("Spróbuj ponownie");
				displayNumber(0);
			}
		}
		break;
		
		case STATE_DISPLAY_RESULT:
		{
			double result = convertCurrency(currentAmount, sourceCurrency, targetCurrency);
			snprintf(lcd_line1, sizeof(lcd_line1), "%.2f %c ->", currentAmount, sourceCurrency);
			snprintf(lcd_line2, sizeof(lcd_line2), "%.2f %c", result, targetCurrency);
			lcdPrintString(lcd_line1);
			lcdSetCursor(0, 1);
			lcdPrintString(lcd_line2);
			displayNumberWithDot((long)(result * 100), 2);
		}
		break;
		
		default:
		{
			lcdPrintString("Stan b³êdny!");
			displayNumber(0);
		}
		break;
	}
}

double getExchangeRate(char from, char to){
	if(from == to) return 1.0;
	
	double rate_from_to_pln = 0.0;
	if(from == KEY_PLN) rate_from_to_pln = 1.0;
	else if(from == KEY_USD) rate_from_to_pln = RATE_USD_PLN;
	else if(from == KEY_EURO) rate_from_to_pln = RATE_EURO_PLN;
	else if(from == KEY_GBP) rate_from_to_pln = RATE_GBP_PLN;
	else return 0.0;
	
	double rate_to_to_pln = 0.0;
	if(to == KEY_PLN) rate_to_to_pln = 1.0;
	else if(to == KEY_USD) rate_to_to_pln = RATE_USD_PLN;
	else if(to == KEY_EURO) rate_to_to_pln = RATE_EURO_PLN;
	else if(to == KEY_GBP) rate_to_to_pln = RATE_GBP_PLN;
	else return 0.0;
	
	return rate_from_to_pln / rate_to_to_pln;
}

double convertCurrency(double amount, char src, char trg){
	if(src == trg){
		return amount;
	}
	
	double rate = getExchangeRate(src, trg);
	if(rate == 0.0){
		return 0.0;
	}
	return amount * rate;
}
