/*
 * servo.c
 *
 *  Created on: 2016-02-19
 *      Author: Aiwa
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define SERVO_MIN 126 // 1ms at 50hz
#define SERVO_MAX 252 // 2ms at 50hz

volatile uint8_t offset = 0;
volatile uint8_t discard = 1;

void initPwm(void)
{
	// OC1A as output
	DDRB |= _BV(PIN1);

	// PWM Mode 14. Fast PWM, phase correct, prescaler 64
	TCCR1A |= _BV(COM1A1) | _BV(WGM11);
	TCCR1B |= _BV(WGM12) | _BV(WGM13) | _BV(CS11) | _BV(CS10);

	// 50hz 20ms, hand calibrated
	ICR1 = 2530;
}

void initADC(void)
{
	// AVCC with external cap on AREF, ADC5 as input
	ADMUX |= _BV(REFS0) | _BV(MUX2)| _BV(MUX0);

	// Enable, start, interrupt enabled, prescaler 128
	ADCSRA |= _BV(ADEN) | _BV(ADSC) | _BV(ADIE) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
}

// Change ADC channel measured
void selectADC(uint8_t channel)
{
    ADMUX = (ADMUX & 0xE0) | (channel & 0x1F);
}

// Proportionally map given value from range inLow-inHigh to outLow-outHigh
long map(long value, long inLow, long inHigh, long outLow, long outHigh)
{
  return (value - inLow) * (outHigh - outLow) / (inHigh - inLow) + outLow;
}

// ADC conversion done interrupt runtine
ISR(ADC_vect){
    // Get current channel
    uint8_t currentChannel = ADMUX & 0x0F;

    // Read out the current value
    uint16_t temp = ADC;

    // Discard first conversion after channel change
    if (discard) {
    	discard = 0;
    	ADCSRA |= 1<<ADSC;

    	return;
    }

    // Set PWM pulse or offset of min max values, based on which ADC channel is measured
    switch(currentChannel){
        case 5:
        		OCR1A = map(temp, 0, 1023, SERVO_MIN - offset, SERVO_MAX + offset);
        		selectADC(4);
            break;
        case 4:
        		offset = map(temp, 0, 1023, 0, 126);
        		selectADC(5);
        	break;
    }

    // Restart conversion
    discard = 1;
    ADCSRA |= 1<<ADSC;
}

int main(void)
{
	sei();
	initPwm();
	initADC();

	while (1) {}

	return 0;
}
