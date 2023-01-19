//*****************************************************************************
// 11. Aug. 2022
//                               c i r c l e
//
//                                                               қuran wolfgang
//*****************************************************************************
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <FastLED.h>
#include "../lib/AppAdapder/AppAdapter.h"
#include "../lib/DriveAdapter/DriveAdapter.h"

const int motorpins[] = {2, 32, 33, 15};
const int frequency = 1000;
const int resolution = 8;

DriveAdapter driveAdapter;
AppAdapter appAdapter;

void setup() {
    Serial.begin(115200);
    appAdapter.begin("HTL-Robot");
    driveAdapter.setup(External_ic);
    Serial.println("The Roboter started, now you can pair it with bluetooth!");
}


void loop() {
    if (appAdapter.dataAvailable()) {
        if(appAdapter.isMsg()) {
            Serial.println(appAdapter.readMsg());
        } else if(appAdapter.isSpeed()) {
            Serial.println(appAdapter.readSpeed().toString());
        } else if(appAdapter.isColor()) {
            CRGB col = appAdapter.readColor();
            Serial.printf("r:%d g:%d b:%d\n",col.r, col.g, col.b);
        } else {
            Serial.println("Unknown Object: " + String(appAdapter.readBuffer()));
            appAdapter.finish_data_handling();
        }
    }
    if (Serial.available()) {
        appAdapter.write(Serial.read());
    }
}
