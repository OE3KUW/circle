#ifndef APP_ADAPTER
#define APP_ADAPTER

#include <BluetoothSerial.h>
#include "../DriveAdapter/direction_t.h"


class SerialBluetoothKit : public BluetoothSerial
{
private:
    String apname;
    String buffer;
public:
    SerialBluetoothKit();
    void begin(String);
    void begin();
    bool isJson();
    direction_t readSpeed();
};




#endif