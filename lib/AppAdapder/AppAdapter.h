#ifndef APP_ADAPTER
#define APP_ADAPTER

#include <BluetoothSerial.h>
#include "direction_t.h"
#include "fastled_config.h"


class AppAdapter : public BluetoothSerial
{
private:
    String apname;
    char buffer[256];
    int buffp;
    int buff_available;
public:
    AppAdapter();
    void begin(String);
    void begin();

    bool isSpeed();
    bool isMsg();
    bool isColor();
    int dataAvailable();

    static direction_t decodeSpeed(const String&);

    char* readBuffer();
    direction_t readSpeed();
    CRGB readColor();
    String readMsg();

    int writeMsg(String);
    int writeBatteryStatus(int);

    void finish_data_handling();

};


#endif