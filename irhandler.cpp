#include "irhandler.h"
#include <IRremote.hpp>

static int irPin = 13; // Giá trị mặc định, sẽ được set lại khi initIR

void initIR(int pin) {
    irPin = pin;
    IrReceiver.begin(irPin, ENABLE_LED_FEEDBACK);
}

bool getIRKey(uint64_t &keyCode) {
    if (IrReceiver.decode()) {
        keyCode = IrReceiver.decodedIRData.decodedRawData; // Lấy rawData 64 bit
        IrReceiver.resume();
        return true;
    }
    return false;
}