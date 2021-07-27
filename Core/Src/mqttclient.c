#include <mqttclient.h>

uint32_t mqtt_msgId = 0;

int mqtt_buflen = MQTT_PacketBuffSize;
uint8_t mqtt_PacketBuf[MQTT_PacketBuffSize];

int mqtt_transport_sendPacketBuffer(uint8_t *buf, int buflen) {

	// MQTT Head may have 0x00
	USART2_RecvEndFlag = 0;
	memset(USART2_RxBUF, 0, USART2_MAX_RECVLEN);
	HAL_UART_Transmit(&huart2, buf, buflen, 0xff);

	return buflen;
}

int mqtt_transport_getdata(uint8_t *buf, int buflen) {

	if (MQTT_RecvEndFlag == 1) {
		memcpy(buf, (const char*) USART2_RxBUF, buflen);

		USART2_RxLen = 0;
		USART2_RecvEndFlag = 0;
		memset(USART2_RxBUF, 0, USART2_MAX_RECVLEN);
	}

	return buflen;
}
int mqtt_transport_open(uint8_t *host, int port) {
	return 1;
}
int mqtt_transport_close(int sock) {
	return 0;
}

uint8_t mqtt_ConnectServer() {

	uint8_t responMsg = 0xff;
	uint8_t sessionPresent = 0;
	uint8_t connack_rc = 0;

	mqtt_buflen = sizeof(mqtt_PacketBuf);

	MQTTPacket_connectData ConnectData = MQTTPacket_connectData_initializer;
	ConnectData.clientID.cstring = MQTT_CLIENTID;
	ConnectData.username.cstring = MQTT_USERNAME;
	ConnectData.password.cstring = MQTT_PASSWORD;
	ConnectData.keepAliveInterval = MQTT_KeepAliveInterval;
	ConnectData.MQTTVersion = 4;
	ConnectData.cleansession = 1;

	u1_printf("(DBG:) Trying to connect MQTT server\r\n\n");

	// Use connack_rc as command length here
	connack_rc = MQTTSerialize_connect(mqtt_PacketBuf, mqtt_buflen,
			&ConnectData); // build connect packet
	mqtt_transport_sendPacketBuffer(mqtt_PacketBuf, connack_rc);

	do {
		while (responMsg != CONNACK) {
			responMsg = MQTTPacket_read(mqtt_PacketBuf, mqtt_buflen,
					mqtt_transport_getdata);
		}

		HAL_UART_Receive_DMA(&huart2, USART2_RxBUF, USART2_MAX_RECVLEN);

	} while (MQTTDeserialize_connack(&sessionPresent, &connack_rc,
			mqtt_PacketBuf, mqtt_buflen) != 1 || connack_rc != 0);

	if (connack_rc != 0) {
		u1_printf("connack_rc:%uc\r\n", connack_rc);
	}

	u1_printf("Connect Success!\r\n");

	HAL_Delay(3000);
	connack_rc = MQTTSerialize_disconnect(mqtt_PacketBuf, mqtt_buflen);
	mqtt_transport_sendPacketBuffer(mqtt_PacketBuf, connack_rc);

	return 1;
}
