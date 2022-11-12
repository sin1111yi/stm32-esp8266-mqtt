#ifndef __MQTT_PUBLISH_H
#define __MQTT_PUBLISH_H

#include "MQTTPacket.h"
#include "cJSON.h"

#include <string.h>

#include "net_conf.h"
#include "mqttclient.h"

#include "temperature.h"

#define device_id MQTT_DEVICE_ID

#define mqtt_topic_PropReport "$oc/devices/60f7fc27aa3bcc0287387b2e_c8t6_esp8266/sys/properties/report"

void MQTT_DevicePropReportTest(void);
int MQTT_DevicePropReport_UploadTestService(char *testString, int testInteger,
		double testFloat);
int MQTT_DevicePropReport_CpuProperties(double cpu_temperature,
		int cpu_messionRound);

#endif
