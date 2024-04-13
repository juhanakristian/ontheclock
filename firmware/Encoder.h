//
// Created by Juhana Jauhiainen on 13.4.2024.
//

#ifndef FIRMWARE_ENCODER_H
#define FIRMWARE_ENCODER_H
// Values returned by 'process'
// No complete step yet.
#define DIR_NONE 0x0
// Clockwise step.
#define DIR_CW 0x10
// Anti-clockwise step.
#define DIR_CCW 0x20

class Encoder {
public:
    Encoder();
    void init();
    unsigned char process();
private:
    unsigned char state;
};


#endif //FIRMWARE_ENCODER_H
