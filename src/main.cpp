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

int cmdavailable = 0;
int cmd;
int cmdp = 0;
char command[50] = "";

void loop() {
    if (Bluetooth.available() && !cmdavailable) {
        cmd = Bluetooth.read();
        if (cmd == 0) {
            command[cmdp] = '\0';
            cmdp = 0;
            cmdavailable = 1;
        } else {
            command[cmdp++] = (char) lowByte(cmd);
        }
    }

    // Command Handling
    if (cmdavailable) {
        cmdavailable = 0;
        Serial.println(command);
        driveAdapter.setSpeed(SerialBluetoothKit::decodeSpeed(command));
    }
    if (Serial.available()) {
        Bluetooth.write(Serial.read());
    }
}
