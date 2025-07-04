#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#include "logic.h"
#include "shift_register.h" 
#include "keypad.h"

AppState currentAppState;
char inputBuffer[10];
uint8_t inputBufferIndex;
double currentAmount;
char sourceCurrency;
char targetCurrency;

void resetConverter(void) {
	currentAppState = STATE_INPUT_AMOUNT;
	memset(inputBuffer, 0, sizeof(inputBuffer));
	inputBufferIndex = 0;
	currentAmount = 0.0;
	sourceCurrency = KEY_NONE;
	targetCurrency = KEY_NONE;
	
	clearSevenSegments();
	setSevenSegmentNumber(0, -1);
}

double getRateToPLN(char currency) {
	if (currency == KEY_USD) return 4.00;
	if (currency == KEY_EURO) return 4.35;
	if (currency == KEY_GBP) return 5.00;
	if (currency == KEY_PLN) return 1.0; //PLN => PLN: 1
	return 0.0; //UNDEFINED CURRENCY
}

double calculateConvertedAmount(double amount, char src, char trg) {
	if(src == trg){
		return amount;
	}

	double amount_in_pln;

	//SOURCE CURRENCY => PLN
	amount_in_pln = amount * getRateToPLN(src);
	
	//PLN => TARGET CURRENCY
	double target_rate_to_pln = getRateToPLN(trg);
	if(target_rate_to_pln == 0.0){
		return 0.0;
	}

	return amount_in_pln / target_rate_to_pln;
}