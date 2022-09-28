#ifndef APP_ADAPTER
#define APP_ADAPTER

#include <BluetoothSerial.h>

class AppAdapter
{
private:
    BluetoothSerial BTAdapter;
    String apname;
    boolean connected;
    int left;
    int right;
public:
    AppAdapter();
    AppAdapter(const String);
    char connect();
    int getLeft();
    int getRight();
    void getData();
    friend void onEvent(esp_spp_cb_event_t, esp_spp_cb_param_t *);
};




#endif