#include "servo.h"
#include <ESP32Servo.h>
#include <Arduino.h>
#include "hcsr04handler.h"


Servo gateServo;
unsigned long gateOpenMillis = 0;
bool gateIsOpen = false;

void initServo(int pin) {
    gateServo.setPeriodHertz(50);
    gateServo.attach(pin, 500, 2400);
    gateServo.write(0); // Đóng cổng ban đầu
}

void openGate() {
    Serial.println("openGate called");
    gateServo.write(90); // Mở cổng
    gateOpenMillis = millis();
    gateIsOpen = true;
}

void handleGate() {
    if (gateIsOpen) {
        float distance = getDistanceCM();
        if (distance > 0 && distance < 20) {
            // Có vật cản, giữ cổng mở
            gateOpenMillis = millis(); // Reset thời gian mở
        } else if (millis() - gateOpenMillis > 500) { // Không còn vật cản, đợi 0.5s rồi đóng
            closeGate();
            gateIsOpen = false;
        }
    }
}

void closeGate() {
    gateServo.write(0); // Đóng cổng
}