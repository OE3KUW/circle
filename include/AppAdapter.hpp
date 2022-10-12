#ifndef APP_ADAPTER
#define APP_ADAPTER

#include <BluetoothSerial.h>

class AppAdapter
{
private:
    BluetoothSerial *BTAdapter;
    String apname;
    int available;
    int left;
    int right;
public:
    AppAdapter();
    AppAdapter(const String);
    AppAdapter(BluetoothSerial*);
    char connect();
    int getLeft();
    int getRight();
    void addEventHandler();
    int isAvailable();
    void deserialize(String);
    friend void onEvent(esp_spp_cb_event_t, esp_spp_cb_param_t *);
};




#endif