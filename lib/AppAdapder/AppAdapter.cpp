#include "AppAdapter.h"

#include <utility>
 AppAdapter::AppAdapter(): BluetoothSerial() {
    for(char e : buffer) {
        e = ' ';
    }
}

void AppAdapter::begin(String name) {
    BluetoothSerial::begin(std::move(name));
}

void AppAdapter::begin() {
    BluetoothSerial::begin("EL-Roboter");
}

bool AppAdapter::isSpeed() {
    return (strchr(buffer, '{') && strchr(buffer, 'r') && strchr(buffer, 'l') && strchr(buffer, '}'));
}

bool AppAdapter::isMsg() {
    return (strchr(buffer, '{') && strchr(buffer, 'm') && strchr(buffer, '}') );
}

bool AppAdapter::isColor() {
    return (strchr(buffer, '{') && strchr(buffer, 'r') && strchr(buffer, 'g') && strchr(buffer, 'b') && strchr(buffer, '}'));
}

direction_t AppAdapter::decodeSpeed(const String& raw) {
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

int AppAdapter::dataAvailable() {
    if(!buff_available && BluetoothSerial::available()) {
        int _2byte = BluetoothSerial::read();
        if(_2byte == 13) {
            buffer[buffp] = '\0';
            buffp = 0;
            buff_available = 1;
        } else {
            buffer[buffp++] = (char) lowByte(_2byte);
        }
    }
    return buff_available;
}

String AppAdapter::readMsg() {
    if(buff_available) {
        bool isValue;
        bool isMsg;
        String value;
        String msg = "";
        for(int n = 0; n < strlen(buffer); n++) {
            if (buffer[n] == ',' || buffer[n] == '}') {
                isValue = false;
                if(isMsg) {
                    msg = value;
                    isMsg = false;
                }
                value = "";
                if(buffer[n] == '}') {
                    break;
                }
            } else if(isValue) {
                value += buffer[n];
            } else if (buffer[n] == 'm') {
                isMsg = true;
            } else if(buffer[n] == ':') {
                isValue = true;
            }
        }
        finish_data_handling();
        return msg;
    }
    return "";
}

direction_t AppAdapter::readSpeed() {
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
            } else if(buffer[n] == ':') {
                isValue = true;
            }
        }
        finish_data_handling();
        return {l, r};
    }
    return {0, 0};
}

CRGB AppAdapter::readColor() {
    if(buff_available) {
        bool isRed, isGreen;
        bool isValue;
        String value;
        uint8_t r, g, b;
        for(int n = 0; n < strlen(buffer); n++) {
            if (buffer[n] == ',' || buffer[n] == '}') {
                isValue = false;
                if(isRed) {
                    r = value.toInt();
                } if(isGreen) {
                    g = value.toInt();
                } else {
                    b = value.toInt();
                }
                value = "";
                if(buffer[n] == '}') {
                    break;
                }
            } else if(isValue) {
                value += buffer[n];
            } else if(buffer[n] == 'r') {
                isRed = true;
            } else if(buffer[n] == 'g') {
                isGreen = true;
            } else if(buffer[n] == 'b') {
                isRed = false;
                isGreen = false;
            } else if(buffer[n] == ':') {
                isValue = true;
            }
        }
        finish_data_handling();
        return {r,g,b};
    }
    return {0, 0, 0};
}

void AppAdapter::finish_data_handling() {
    buff_available = 0;
}

char *AppAdapter::readBuffer() {
    if(buff_available) {
        return buffer;
    }
    return nullptr;
}

int AppAdapter::writeMsg(String msg) {
    return this->print("{m:" + msg + "}");
}

int AppAdapter::writeBatteryStatus(int prozent) {
    return this->print("{b:" + String(prozent) + "}");
}
