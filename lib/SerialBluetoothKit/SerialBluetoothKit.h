#ifndef APP_ADAPTER
#define APP_ADAPTER

#include <BluetoothSerial.h>
#include "DriveAdapter.h"

class SerialBluetoothKit : public BluetoothSerial
{
private:
    String apname;
public:
    SerialBluetoothKit();
    void begin(String);
    void begin();
    direction_t readSpeed();
};




#endif