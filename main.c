#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "shift_register.h"
#include "keypad.h"
#include "lcd.h"
#include "logic.h"

int main(void){
	//initialization
	//lcdInit();
	keypadInit();
	shiftRegisterInit();
	/*
	lcdClear();
	lcdPrintString("Przelicznik walut");
	lcdSetCursor(0, 1);
	lcdPrintString("Inicjalizacja...");
	_delay_ms(2000);
	*/
	
	resetConverter();
	
	while(1){
		displayMultiplexedNumber();
		
		char key = getKey();
		if(key != KEY_NONE){
			if(key == KEY_CLEAR){
				resetConverter();
				lastPressedCurrencyKey = KEY_NONE;
			}
			else if(key == KEY_USD || key == KEY_EURO || key == KEY_GBP || key == KEY_PLN){
				if(currentAppState == STATE_INPUT_AMOUNT){
					if(inputBufferIndex > 0){
						currentAmount = atof(inputBuffer);
						sourceCurrency = key;
						currentAppState = STATE_SELECT_TARGET_CURRENCY;
					} else{
						/*
						lcdClear();
						lcdPrintString("Wpisz kwotê");
						_delay_ms(1000);
						*/
					}
				}
				else if(currentAppState == STATE_SELECT_TARGET_CURRENCY){
					targetCurrency = key;
					currentAppState = STATE_DISPLAY_RATE;
					lastPressedCurrencyKey = key;
				}
				else if(currentAppState == STATE_DISPLAY_RATE){
					if(key == lastPressedCurrencyKey){
						currentAppState = STATE_DISPLAY_RESULT;
					} else{
						targetCurrency = key;
						lastPressedCurrencyKey = key;
					}
				}
				else if(currentAppState == STATE_DISPLAY_RESULT){
					resetConverter();
				}
			}
			else if((key >= '0' && key <= '9') || key == KEY_DECIMAL){
				if(currentAppState == STATE_INPUT_AMOUNT){
					if(inputBufferIndex < sizeof(inputBuffer) - 1){
						if(key == KEY_DECIMAL && strchr(inputBuffer, KEY_DECIMAL) != NULL){
							//
						} else{
							inputBuffer[inputBufferIndex++] = key;
							inputBuffer[inputBufferIndex] = '\0';
						} 
					} 
				} else{
					resetConverter();
					if(inputBufferIndex < sizeof(inputBuffer) - 1){
						if(key == KEY_DECIMAL && strchr(inputBuffer, KEY_DECIMAL) != NULL){
							//
						} else{
							inputBuffer[inputBufferIndex++] = key;
							inputBuffer[inputBufferIndex] = '\0';
						}
					}
				}
			}
			//updateDisplay();
		}
	}
	return 0;
}