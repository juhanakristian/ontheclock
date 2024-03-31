#include <avr/io.h>
#include <util/delay.h>
 
#define BLINK_DELAY_MS 10000
 
int main (void)
{
 /* set pin 5 of PORTB for output*/
 DDRD |= _BV(PD3);
 
 while(1) {
  /* set pin 5 high to turn led on */
  PORTD |= _BV(PD3);
  _delay_ms(BLINK_DELAY_MS);
 
  /* set pin 5 low to turn led off */
  PORTD &= ~_BV(PD3);
  _delay_ms(BLINK_DELAY_MS);
 }
}
