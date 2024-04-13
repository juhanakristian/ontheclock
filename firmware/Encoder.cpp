//
// Created by Juhana Jauhiainen on 13.4.2024.
//


#include <avr/io.h>

#include "Encoder.h"

#define R_START 0x0
#define R_CW_FINAL 0x1
#define R_CW_BEGIN 0x2
#define R_CW_NEXT 0x3
#define R_CCW_BEGIN 0x4
#define R_CCW_FINAL 0x5
#define R_CCW_NEXT 0x6

const unsigned char ttable[7][4] = {
        // R_START
        {R_START,    R_CW_BEGIN,  R_CCW_BEGIN, R_START},
        // R_CW_FINAL
        {R_CW_NEXT,  R_START,     R_CW_FINAL,  R_START | DIR_CW},
        // R_CW_BEGIN
        {R_CW_NEXT,  R_CW_BEGIN,  R_START,     R_START},
        // R_CW_NEXT
        {R_CW_NEXT,  R_CW_BEGIN,  R_CW_FINAL,  R_START},
        // R_CCW_BEGIN
        {R_CCW_NEXT, R_START,     R_CCW_BEGIN, R_START},
        // R_CCW_FINAL
        {R_CCW_NEXT, R_CCW_FINAL, R_START,     R_START | DIR_CCW},
        // R_CCW_NEXT
        {R_CCW_NEXT, R_CCW_FINAL, R_CCW_BEGIN, R_START},
};


Encoder::Encoder() {
    state = R_START;
}

void Encoder::init() {
//PORTD |= (1 << PD6) & (1 << PD7);
    DDRD &= ~(1 << PD6);  // Set PD6 as input
    DDRD &= ~(1 << PD7);  // Set PD7 as input
}

unsigned char Encoder::process() {
    unsigned char pinstate = 0;
    pinstate |= (PIND & (1 << PD6)) >> PD6;  // Read PD6 and shift it to the bit 0 position
    pinstate |= (PIND & (1 << PD7)) >> (PD7 - 1);  // Read PD7, shift it to the bit 1 position

    state = ttable[state & 0xf][pinstate];
    return state & 0x30;
}