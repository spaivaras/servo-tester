# Servo-Tester
A simple servo motors tester based around atmega8. Atmega8 is an overkill for this, but its the chip I had laying around,
the same principles can be used on any a AVR core, with appropriate features.

Uses 16bit timer for generating 50hz pulses of changeable high time of pulse from 1ms to 2ms*
Pot on ADC5 controls the high time, pot on ADC4 controls the min and max boundaries, as many servos will happily go beyond 1-2ms range. Keep in mind that for dumb analog servos too much of a swing can burn them out.

Chip configured for internal 8MHz operation. Maximum prescalers used for ADC accuracy  and low timer frequency of 50hz. ADC source AVCC with cap to ground on AVREF.

Inputs:

- __ADC5__ Main pot for controlling HIGH pulse width
- __ADC4__ Reduces Min boundary of 1ms and increases the Max of 2ms

Outpus:

- __OC1A__ Servo control wire out