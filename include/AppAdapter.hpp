#ifndef APP_ADAPTER
#define APP_ADAPTER

#include <BluetoothSerial.h>
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

class AppAdapter
{
private:
    BluetoothSerial *BTAdapter;
    String apname;
    int available{};
    int left{};
    int right{};
public:
    AppAdapter();
    AppAdapter(const String);
    AppAdapter(BluetoothSerial*);
    char begin();
    int getLeft();
    int getRight();
    void addEventHandler();
    int isAvailable();
    void deserialize(String);
    friend void onEvent(esp_spp_cb_event_t, esp_spp_cb_param_t *);
};




#endif