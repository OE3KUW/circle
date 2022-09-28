#include <AppAdapter.hpp>

AppAdapter::AppAdapter(const String n)
{
    apname = n;
}

AppAdapter::AppAdapter()
{
    apname = "CircleClient";
}

void onEvent(esp_spp_cb_event_t event, esp_spp_cb_param_t *param){
  if(event == ESP_SPP_SRV_OPEN_EVT) {
    Serial.println("[INFO] Client Connected");
  }
  else if(event == ESP_SPP_SRV_STOP_EVT) {
    Serial.println("[INFO] Client Disconnected");
  }
  Serial.println("[INFO] " + event);
}

char AppAdapter::connect()
{
    BTAdapter.register_callback(onEvent);
    if(!BTAdapter.begin(apname))
    {
        Serial.println("[ERROR] Connection failed");
        return 1;
    }
    Serial.println("[INFO] Connection started");
    return 0;
}

