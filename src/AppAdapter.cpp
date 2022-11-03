#include <AppAdapter.hpp>

AppAdapter::AppAdapter(const String n)
{
	apname = n;
	BTAdapter = new BluetoothSerial;
}

AppAdapter::AppAdapter()
{
	apname = "CircleClient";
	BTAdapter = new BluetoothSerial;
}

AppAdapter::AppAdapter(BluetoothSerial *_instance)
{
	apname = "CircleClient";
	BTAdapter = _instance;
}

void onEvent(esp_spp_cb_event_t event, esp_spp_cb_param_t *param)
{
	if (event == ESP_SPP_SRV_OPEN_EVT)
	{
		Serial.println("[INFO] Client Connected");
	}
	else if (event == ESP_SPP_SRV_STOP_EVT)
	{
		Serial.println("[INFO] Client Disconnected");
	}
	Serial.println("[INFO] " + event);
}

char AppAdapter::begin()
{
	BTAdapter->register_callback(onEvent);
	if (!BTAdapter->begin(apname))
	{
		Serial.println("[ERROR] Connection failed");
		return 1;
	}
	Serial.println("[INFO] Connection started");
	return 0;
}

int AppAdapter::isAvailable() {
	available = BTAdapter->available();
	if(available) {
		return 1;
	}
	return 0;
}
