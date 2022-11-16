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

bool SerialBluetoothKit::isJson() {
    return buffer[0] == '{' && strchr(buffer, '}');
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

int SerialBluetoothKit::cmdavailable() {
    if(!buff_available && BluetoothSerial::available()) {
        int _2byte = BluetoothSerial::read();
        if(_2byte == 0) {
            buffer[buffp] = '\0';
            buffp = 0;
            buff_available = 1;
        } else {
            buffer[buffp++] = (char) lowByte(_2byte);
        }
    }
    return buff_available;
}

char* SerialBluetoothKit::readcmd() {
    if(buff_available) {
        return buffer;
    }
    return nullptr;
}

direction_t SerialBluetoothKit::readSpeed() {
    if(buff_available) {
        bool isLeft;
        bool isValue;
        String value;
        int l, r;
        for(int n = 0; n < strlen(buffer); n++) {
            if (buffer[n] == ',' || buffer[n] == '}') {
                isValue = false;
                if(isLeft) {
                    l = value.toInt();
                } else {
                    r = value.toInt();
                }
                value = "";
                if(buffer[n] == '}') {
                    break;
                }
            } else if(isValue) {
                value += buffer[n];
            } else if(buffer[n] == 'l') {
                isLeft = true;
            } else if(buffer[n] == 'r') {
                isLeft = false;
            } else if(buffer[n] == '=') {
                isValue = true;
            }
        }
        finish_cmd_handling();
        return {l, r};
    }
    return {0, 0};
}

void SerialBluetoothKit::finish_cmd_handling() {
    buff_available = 0;
}
