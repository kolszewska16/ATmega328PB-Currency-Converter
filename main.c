#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h> 
#include <stdlib.h> 
#include <math.h>   

#include "keypad.h"
#include "shift_register.h"
#include "logic.h"

int main(void){
	//initialization
	keypadInit();
	shiftRegisterInit();
	
	//initial state
	resetConverter();
	
	while(1){
		displayMultiplexedNumber();
		
		char key = getKey();
		if(key != KEY_NONE){
			if(key == KEY_CLEAR){
				resetConverter();
			}
		else if((key >= '0' && key <= '9') || key == KEY_DECIMAL){
			if(currentAppState == STATE_INPUT_AMOUNT){
				if(inputBufferIndex < sizeof(inputBuffer) - 1){
					if(key == KEY_DECIMAL && strchr(inputBuffer, KEY_DECIMAL) != NULL){
						//
						} else{
							inputBuffer[inputBufferIndex++] = key;
							inputBuffer[inputBufferIndex] = '\0';
							
							int32_t num_to_display = 0;
							int8_t dot_pos_7seg = -1;

							char* decimal_point_ptr = strchr(inputBuffer, KEY_DECIMAL);
							if (decimal_point_ptr != NULL){
								int num_digits_after_dot = inputBufferIndex - (decimal_point_ptr - inputBuffer + 1);
								num_to_display = (int32_t)(atof(inputBuffer) * pow(10, num_digits_after_dot));
								
								/*
								0: kropka po jednoœciach (np. 1234.)
								1: kropka po dziesi¹tych (np. 123.4)
								2: kropka po setnych (np. 12.34)
								3: kropka po tysiêcznych (np. 1.234)
								*/
								
								dot_pos_7seg = num_digits_after_dot;
								} else{
									num_to_display = atoi(inputBuffer);
									dot_pos_7seg = -1;
								}
							
							if (num_to_display > 9999) {
								setSevenSegmentNumber(-1, -1);
								} else{
									setSevenSegmentNumber(num_to_display, dot_pos_7seg);
								}
							}
					}
				} else{
					resetConverter();
					if (inputBufferIndex < sizeof(inputBuffer) - 1){
						if(key != KEY_DECIMAL || strchr(inputBuffer, KEY_DECIMAL) == NULL){ 
							inputBuffer[inputBufferIndex++] = key;
							inputBuffer[inputBufferIndex] = '\0';
							setSevenSegmentNumber(atoi(inputBuffer), -1);
						}
					}
				}
			}
			
			//CURRENCIES
			else if(key == KEY_USD || key == KEY_EURO || key == KEY_GBP || key == KEY_PLN){
				if(currentAppState == STATE_INPUT_AMOUNT){
					if(inputBufferIndex > 0 && strcmp(inputBuffer, ".") != 0){
						currentAmount = atof(inputBuffer);
						sourceCurrency = key;
						currentAppState = STATE_SELECT_SOURCE_CURRENCY; 
						clearSevenSegments();
					}
				}
				else if(currentAppState == STATE_SELECT_SOURCE_CURRENCY || currentAppState == STATE_DISPLAY_RESULT){
					if(key != sourceCurrency){
						targetCurrency = key;
						
						double result_double = calculateConvertedAmount(currentAmount, sourceCurrency, targetCurrency);
						
						int32_t result_7seg = (int32_t)(result_double * 100.0 + 0.5);
						int8_t dot_pos_7seg = 2;
						
						if(result_7seg > 9999 || result_double < 0){
							setSevenSegmentNumber(-1, -1);
						}else{
							setSevenSegmentNumber(result_7seg, dot_pos_7seg);
						}
						currentAppState = STATE_DISPLAY_RESULT;
						} else {
							//
						}
				}
			}
		}
	}
}