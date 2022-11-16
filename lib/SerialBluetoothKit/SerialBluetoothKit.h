#ifndef APP_ADAPTER
#define APP_ADAPTER

#include <BluetoothSerial.h>
#include "../DriveAdapter/direction_t.h"


class SerialBluetoothKit : public BluetoothSerial
{
private:
    String apname;
    char buffer[128];
    int buffp;
    int buff_available;
public:
    SerialBluetoothKit();
    void begin(String);
    void begin();
    bool isJson();
    static direction_t decodeSpeed(const String&);
    direction_t readSpeed();
    int cmdavailable();
    char* readcmd();
    void finish_cmd_handling();

};




#endif