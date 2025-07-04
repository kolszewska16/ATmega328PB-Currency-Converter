#ifndef LOGIC_H_
#define LOGIC_H_

#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum {
	STATE_INPUT_AMOUNT,         
	STATE_SELECT_SOURCE_CURRENCY, 
	STATE_SELECT_TARGET_CURRENCY,
	STATE_DISPLAY_RESULT       
} AppState;

extern AppState currentAppState;
extern char inputBuffer[10];     
extern uint8_t inputBufferIndex;
extern double currentAmount;
extern char sourceCurrency;
extern char targetCurrency;

static const double USD_TO_PLN_RATE = 3.60;
static const double EURO_TO_PLN_RATE = 4.25;
static const double GBP_TO_PLN_RATE = 4.92;
static const double PLN_TO_PLN_RATE = 1.00;

void resetConverter(void);
double getExchangeRate(char src, char trg);
double calculateConvertedAmount(double amount, char src, char trg);
double getRateToPLN(char currency);

#endif