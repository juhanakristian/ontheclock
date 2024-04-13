#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <util/delay.h>

#include "Encoder.h"

extern "C" {
#include "uart.h"
}

#include "lcd.h"

const static uint8_t BPM_POT = 7;
const static uint8_t BPM_MAX = 200;
const static uint8_t BPM_MIN = 30;

Encoder encoder = Encoder();

void init_encoder() {
    // Setup interrupt
    // Enable pin change interrupt on PD6 and PD7
    PCMSK2 |= (1 << PCINT22) | (1 << PCINT23);

    // Enable pin change interrupt for port D
    PCICR |= (1 << PCIE2);

    encoder.init();
}

volatile uint8_t bpm = 60;

int main(void) {
    // Enabled USART and redirect standard input/output to UART
    Uart::init();
    stdout = uart_output;
    stdin = uart_input;

    sei(); // turn on interrupts

    /* set pin 5 of PORTB for output*/
    DDRD |= _BV(PD3);

    uint16_t counter = 0;

    init_encoder();

    lcd_init(LCD_DISP_ON);
    lcd_charMode(DOUBLESIZE);

    while (1) {
        printf("BPM %i\n", bpm);
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

        char text[4];
        sprintf(text, "%i", bpm);
        lcd_clrscr();
        lcd_puts(text);
    }
}

ISR(PCINT2_vect) {
    unsigned char result = encoder.process();

    if (result == DIR_CW) {
        bpm++;
    } else if (result == DIR_CCW) {
        bpm--;
    }
}

