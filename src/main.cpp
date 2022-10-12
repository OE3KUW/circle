//*****************************************************************************
// 11. Aug. 2022
//                               c i r c l e
//
//                                                               қuran wolfgang
//*****************************************************************************
// add Libraries NeoPixelBus Version 2.7 <-- Todo oder woffuer?
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <FastLED.h>
#include <BluetoothSerial.h>
#include <DriveAdapter.hpp>

BluetoothSerial SerialBT;
DriveAdapter driveAdapter;

void setup()
{
    Serial.begin(115200);
    SerialBT.begin("Test");

    driveAdapter.setup(External_ic);
}

void loop()
{
  if(SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
  for(int i = -1024; i < 1023; i++) {
    driveAdapter.setSpeed(i, i);
    delay(10);
    Serial.printf("[speed] %d\n", i);
  }

}
