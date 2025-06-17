#ifndef IR_HANDLER_H
#define IR_HANDLER_H

#include <Arduino.h>

// Định nghĩa mã hex cho từng nút IR remote
#define IR_CODE_0   3910593792UL
#define IR_CODE_1   4077710080UL
#define IR_CODE_2   3877175040UL
#define IR_CODE_3   2707356160UL
#define IR_CODE_4   4144561920UL
#define IR_CODE_5   3803742720UL
#define IR_CODE_6   2774208000UL
#define IR_CODE_7   3175283200UL
#define IR_CODE_8   2907920640UL
#define IR_CODE_9   3048569600UL
#define IR_CODE_OK  3927310080UL
#define IR_CODE_DEL 3168569600UL

void initIR(int pin);
bool getIRKey(uint32_t &keyCode);

#endif