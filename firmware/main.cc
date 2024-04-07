#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <util/delay.h>

extern "C" {
#include "uart.h"
}

const static uint8_t BPM_POT = 7;
const static uint8_t BPM_MAX = 200;
const static uint8_t BPM_MIN = 30;

uint16_t scale_bpm(uint16_t value) {
  return (uint16_t)((((long)value - BPM_MIN) * (BPM_MAX-BPM_MIN) / 1023) + BPM_MIN);
}
 

void init_adc() {
  // voltage reference
  ADMUX |= (1<<REFS0);
  // setup prescaler to 128 and enable ADC (ADEN)
  ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0) | (1<<ADEN);
}

int read_adc(uint8_t channel) {
  // Select ADC channel with safety mask
  ADMUX = (ADMUX & 0xF0) | (channel & 0x07);
  ADMUX &= ~((1 << MUX0) | (1 << MUX1) | (1 << MUX2) | (1 << MUX3));
  // Start single conversion
  ADCSRA |= (1<<ADSC);
  // Wait for conversion to complete
  while (ADCSRA & (1<<ADSC))
    ;
  // Return the ADC value of the chosen channel
  uint8_t low = ADCL;
  uint8_t high = ADCH;
  return (high << 8) | low;
}
 
int main (void) {
  // Enabled USART and redirect standard input/output to UART
  Uart::init();
  stdout = uart_output;
  stdin = uart_input;


 /* set pin 5 of PORTB for output*/
 DDRD |= _BV(PD3);

 init_adc();

 uint16_t bpm = 120;
 uint16_t counter = 0;

 while(1) {
  int value = read_adc(BPM_POT);

  bpm = scale_bpm(value);
  printf("value %i\n", value);


   if (counter > (60000 / bpm / 2 / 10)) {
     if (PORTD & (1 << PD3)) {
       PORTD &= ~_BV(PD3);
     } else {
       PORTD |= _BV(PD3);
     }
     counter = 0;
   }

  counter++;

  _delay_ms(10);
 }
}


ISR(TIMER2_COMPA_vect) {
}
