//*****************************************************************************
// 11. Aug. 2022
//                               c i r c l e
//
//                                                               қuran wolfgang
//*****************************************************************************
// TODO: add Libraries NeoPixelBus Version 2.7
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <FastLED.h>
#include <BluetoothSerial.h>
#include <DriveAdapter.hpp>
#include "Motor.h"

const int motorpins[] = {2, 32, 33, 15};
const int frequency = 1000;
const int resolution = 8;


BluetoothSerial SerialBT;
DriveAdapter driveAdapter;
hw_timer_t *timer;

/*
Motor mRight(motorpins[0], motorpins[1], frequency, resolution);
Motor mLeft(motorpins[2], motorpins[3], frequency, resolution);



void onIntervallUpdate() {
    mRight.adjustCurrentSpeed();
    mLeft.adjustCurrentSpeed();
}
*/

void setup() {
    Serial.begin(115200);
    SerialBT.begin("Test");

    driveAdapter.setup(External_ic);

    /*for(int i = 0; i < 4; i++) {
        pinMode(motorpins[i], OUTPUT);
    }

    timer = timerBegin(0, 240, true);
    timerAttachInterrupt(timer, onIntervallUpdate, true);
    timerAlarmWrite(timer, Motor::updatedelay, true);
    timerAlarmEnable(timer);
*/
    Serial.println("The Roboter started, now you can pair it with bluetooth!");

}

void loop() {
    if (SerialBT.available()) {
        int spd = SerialBT.read();
        Serial.println(spd);
        Serial.println(spd);
    }

    if (SerialBT.available()) {
        Serial.write(SerialBT.read());
    }
    for (int i = -1024; i < 1023; i++) {
        driveAdapter.setSpeed(direction_t(i, i));
        delay(10);
        Serial.printf("[speed] %d\n", i);
    }
}
