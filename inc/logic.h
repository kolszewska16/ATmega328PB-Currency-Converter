#ifndef LOGIC_H_
#define LOGIC_H_

#include <avr/io.h>

typedef enum{
	STATE_INPUT_AMOUNT,
	STATE_SELECT_SOURCE_CURRENCY,
	STATE_SELECT_TARGET_CURRENCY,
	STATE_DISPLAY_RATE,
	STATE_DISPLAY_RESULT
} AppState;

extern AppState currentAppState;

extern char inputBuffer[17];
extern int inputBufferIndex;
extern double currentAmount;
extern char sourceCurrency;
extern char targetCurrency;
extern char lastPressedCurrencyKey;

void clearInputBuffer(void);
void resetConverter(void);
void updateDisplay(void);
double getExchangeRate(char from, char to);
double convertCurrency(double amount, char src, char trg);

#endif