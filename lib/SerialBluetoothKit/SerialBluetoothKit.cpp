#include "SerialBluetoothKit.h"

#include <utility>
 SerialBluetoothKit::SerialBluetoothKit():BluetoothSerial() {
    buffer = "";
}

void SerialBluetoothKit::begin(String name) {
    BluetoothSerial::begin(std::move(name));
}

void SerialBluetoothKit::begin() {
    BluetoothSerial::begin("EL-Roboter");
}

direction_t SerialBluetoothKit::readSpeed() {
    String raw;
    if(buffer == "") {
        raw = BluetoothSerial::readString();
    } else {
        raw = SerialBluetoothKit::buffer;
        buffer = "";
    }

    bool isLeft;
    bool isValue;
    String value;
    int l, r;
    for(int n = 0; n < raw.length(); n++) {
        if (raw[n] == ',' || raw[n] == '}') {
            for(char c : value) {
                Serial.print(c);
            }
            isValue = false;
            if(isLeft) {
                l = value.toInt();
            } else {
                r = value.toInt();
            }
            value = "";
            if(raw[n] == '}') {
                buffer = "";
                for(int i = n; n < raw.length(); i++) {
                    buffer[i-n] = raw[n];
                }
                break;
            }
        } else if(isValue) {
            value += raw[n];
        } else if(raw[n] == 'l') {
            isLeft = true;
        } else if(raw[n] == 'r') {
            isLeft = false;
        } else if(raw[n] == '=') {
            isValue = true;
        }
    }
    return {l, r};
}

bool SerialBluetoothKit::isJson() {
    buffer = BluetoothSerial::readString();
    return buffer[0] == '{' && buffer[buffer.length()-1] == '}';
}

direction_t SerialBluetoothKit::decodeSpeed(const String& raw) {
    bool isLeft;
    bool isValue;
    String value;
    int l, r;
    for(char n : raw) {
        if (n == ',' || n == '}') {
            isValue = false;
            if(isLeft) {
                l = value.toInt();
            } else {
                r = value.toInt();
            }
            value = "";
            if(n == '}') {
                break;
            }
        } else if(isValue) {
            value += n;
        } else if(n == 'l') {
            isLeft = true;
        } else if(n == 'r') {
            isLeft = false;
        } else if(n == '=') {
            isValue = true;
        }
    }
    return {l, r};
}
