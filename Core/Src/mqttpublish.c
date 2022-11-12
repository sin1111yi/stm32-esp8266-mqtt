#include "mqttpublish.h"

char *pbuf = NULL;

int MQTT_DevicePropReport_UploadTestService(char *testString, int testInteger,
		double testFloat) {
	cJSON *RepoetMsg = NULL;
	cJSON *properties = NULL;
	cJSON *services = NULL;
	cJSON *MsgStruct = NULL;

	if (pbuf == NULL)
		pbuf = (char*) malloc(MQTT_PacketBuffSize * sizeof(char));
	if (pbuf == NULL) {
		u1_printf("(DBG:) Not enough memory");
		Error_Handler();
	}

	MQTTString TopicName = MQTTString_initializer;
	TopicName.cstring = (char*) mqtt_topic_PropReport;

	RepoetMsg = cJSON_CreateObject();
	properties = cJSON_CreateObject();
	services = cJSON_CreateArray();
	MsgStruct = cJSON_CreateObject();

	cJSON_AddStringToObject(MsgStruct, "service_id", "UploadTestService");
	cJSON_AddStringToObject(properties, "TestUploadString", testString);
	cJSON_AddNumberToObject(properties, "TestUploadInteger", testInteger);
	cJSON_AddNumberToObject(properties, "TestUploadFloat", testFloat);
	cJSON_AddItemToObject(MsgStruct, "properties", properties);

	cJSON_AddItemToArray(services, MsgStruct);

	cJSON_AddItemToObject(RepoetMsg, "services", services);

	pbuf = cJSON_Print(RepoetMsg);
	cJSON_Delete(RepoetMsg);

	memcpy(mqtt_PacketBuf, pbuf, MQTT_PacketBuffSize);

	//u1_printf("(Json:)\r\n%s\r\n", pbuf);

	mqtt_serialLen = MQTTSerialize_publish(mqtt_PacketBuf, MQTT_PacketBuffSize,
			0, 0, 0, 1, TopicName, pbuf, strlen(pbuf));

	free(pbuf);
	return mqtt_transport_sendPacketBuffer(mqtt_PacketBuf, mqtt_serialLen);

}

int MQTT_DevicePropReport_CpuProperties(double cpu_temperature,
		int cpu_messionRound) {
	cJSON *RepoetMsg = NULL;
	cJSON *properties = NULL;
	cJSON *services = NULL;
	cJSON *MsgStruct = NULL;

	if (pbuf == NULL)
		pbuf = (char*) malloc(MQTT_PacketBuffSize * sizeof(char));
	if (pbuf == NULL) {
		u1_printf("(DBG:) Not enough memory");
		Error_Handler();
	}

	MQTTString TopicName = MQTTString_initializer;
	TopicName.cstring = (char*) mqtt_topic_PropReport;

	RepoetMsg = cJSON_CreateObject();
	properties = cJSON_CreateObject();
	services = cJSON_CreateArray();
	MsgStruct = cJSON_CreateObject();

	cJSON_AddStringToObject(MsgStruct, "service_id", "cpu_properties");
	if (cpu_temperature != ABSOLUTE_ZERO)
		cJSON_AddNumberToObject(properties, "cpu_temperature", cpu_temperature);
	cJSON_AddNumberToObject(properties, "cpu_messionRound", cpu_messionRound);
	cJSON_AddItemToObject(MsgStruct, "properties", properties);

	cJSON_AddItemToArray(services, MsgStruct);

	cJSON_AddItemToObject(RepoetMsg, "services", services);

	memset(mqtt_PacketBuf, 0, MQTT_PacketBuffSize);

	pbuf = cJSON_Print(RepoetMsg);
	cJSON_Delete(RepoetMsg);

	memcpy(mqtt_PacketBuf, pbuf, MQTT_PacketBuffSize);

	//u1_printf("(Json:)\r\n%s\r\n", pbuf); // will cause HardFault at No.35 call

	mqtt_serialLen = MQTTSerialize_publish(mqtt_PacketBuf, MQTT_PacketBuffSize,
			0, 0, 0, 1, TopicName, pbuf, strlen(pbuf));

	free(pbuf);
	return mqtt_transport_sendPacketBuffer(mqtt_PacketBuf, mqtt_serialLen);
}
