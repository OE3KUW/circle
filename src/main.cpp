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
#include "../lib/SerialBluetoothKit/SerialBluetoothKit.h"
#include "../lib/DriveAdapter/DriveAdapter.h"

const int motorpins[] = {2, 32, 33, 15};
const int frequency = 1000;
const int resolution = 8;

DriveAdapter driveAdapter;
SerialBluetoothKit Bluetooth;

void setup() {
    Serial.begin(115200);
    Bluetooth.begin("HTL-Robot");
    driveAdapter.setup(External_ic);
    Serial.println("The Roboter started, now you can pair it with bluetooth!");
}


void loop() {
    if (Bluetooth.cmdavailable()) {
        Serial.println(Bluetooth.readcmd());
        driveAdapter.setSpeed(Bluetooth.readSpeed());
    }
    if (Serial.available()) {
        Bluetooth.write(Serial.read());
    }
}
