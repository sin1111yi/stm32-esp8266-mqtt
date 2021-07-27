#ifndef __NET_CONF_H
#define __NET_CONF_H

// Please modify those macro definitions to meet needs

#define WIFI_TAR 1

#if WIFI_TAR == 0

#define AP_SSID     (const char*)"YOUR OWN WIFI SSID"
#define AP_PSWD     (const char*)"YOUR OWN WIFI PSWD"

#elif WIFI_TAR == 1

#define AP_SSID     (const char*)"YOUR OWN PC AP SSID"
#define AP_PSWD     (const char*)"YOUR OWN PC AP PSWD"

#endif

#define CONNECT_MODE 0

#if CONNECT_MODE == 0

#define IpServer       "YOUR OWN MQTT SERVER ADDRESS"
#define ServerPort     "YOUR OWN MQTT SERVER PORT"

#elif CONNECT_MODE == 1

#define IpServer       "YOUR OWN PC TCP SERVER ADDRESS"
#define ServerPort     "YOUR OWN PC TCP SERVER PORT"

#endif

#define MQTT_DEVICE_ID (uint8_t*)"YOUR OWN MQTT DEVICE ID"
#define MQTT_SECRET    (uint8_t*)"YOUR OWN MQTT DEVICE SECRET CODE"

#define MQTT_CLIENTID   "YOUR OWN MQTT DEVICE CLIENTID"
#define MQTT_USERNAME   "YOUR OWN MQTT DEVICE USERNAME"
#define MQTT_PASSWORD   "YOUR OWN MQTT DEVICE PASSWORD"

#endif
