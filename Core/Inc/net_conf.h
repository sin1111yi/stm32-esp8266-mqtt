#ifndef __NET_CONF_H
#define __NET_CONF_H

#define WIFI_TAR 1

#if WIFI_TAR == 0

#define AP_SSID     (const char*)"HUAWEI-0H19GM"
#define AP_PSWD     (const char*)"20001023yzy"

#elif WIFI_TAR == 1

#define AP_SSID     (const char*)"BugMaker"
#define AP_PSWD     (const char*)"20001023"

#endif

#define CONNECT_MODE 0

#if CONNECT_MODE == 0

#define IpServer       "a160ea5b2d.iot-mqtts.cn-north-4.myhuaweicloud.com"
#define ServerPort     "1883"

#elif CONNECT_MODE == 1

#define IpServer       "192.168.3.20"
#define ServerPort     "8086"

#endif

#define MQTT_DEVICE_ID (uint8_t*)"60f7fc27aa3bcc0287387b2e_c8t6_esp8266"
#define MQTT_SECRET    (uint8_t*)"20001023"

#define MQTT_CLIENTID   "60f7fc27aa3bcc0287387b2e_c8t6_esp8266_0_0_2021072621"
#define MQTT_USERNAME   "60f7fc27aa3bcc0287387b2e_c8t6_esp8266"
#define MQTT_PASSWORD   "7d5266b4cf8e035c553393cd5da4181737f88e84aef34bc6c2750b172c0c1003"

#endif
