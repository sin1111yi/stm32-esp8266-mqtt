#include "esp8266.h"

static WIFI_StateTypeDef wifi_state = _OFFLINE;
static WIFI_StateTypeDef trans_state = _UNKNOWN_STATE;
static uint8_t wifi_config_step = 0;

WIFI_StateTypeDef esp8266_CheckRespond(uint8_t *str) {
	u1_printf("(DBG:) usart2 recv:\r\n%s\r\n", USART2_RxBUF);
	if (strstr((const char*) USART2_RxBUF, (const char*) str) != NULL) {
		u1_printf("(DBG:) Match succeed\r\n");
		memset(USART2_RxBUF, 0, USART2_MAX_RECVLEN);
		return _MATCHOK;
	}

	memset(USART2_RxBUF, 0, USART2_MAX_RECVLEN);
	return _MATCHERROR;
}

WIFI_StateTypeDef esp8266_TransmitCmd(uint8_t *cmd, uint8_t *ack,
		uint32_t waitms, uint8_t newline) {
	int timeout = waitms;
	uint8_t check = 0;
	memset(USART2_RxBUF, 0, USART2_MAX_RECVLEN);
	u1_printf("\r\n(DBG:) Try to send cmd: %s\r\n", cmd);
	if (newline == 0)
		u2_transmit("%s", cmd);      // transmit cmd to usart2
	else
		u2_transmit("%s\r\n", cmd); // transmit cmd to usart2

	u1_printf("(DBG:) Waiting reply\r\n");
	while (timeout--) {
		// u1_printf("%d ", timeout);
		// finish dma receive
		if (USART2_RecvEndFlag == 1) {
			check = esp8266_CheckRespond(ack);
			if (check == _MATCHOK) {
				u1_printf("(DBG:) Command closed loop completed\r\n");
			}

			USART2_RxLen = 0;
			USART2_RecvEndFlag = 0;
			HAL_UART_Receive_DMA(&huart2, USART2_RxBUF, USART2_MAX_RECVLEN);

			break;
		}
		HAL_Delay(1);
	}

	if (check == _MATCHERROR) {
		u1_printf("\r\n(DBG:) Cmd match failed\r\n");
		return check;
	}
	if (timeout <= 0) {
		u1_printf("(DBG:) Finish waiting\r\n");
		u1_printf("\r\n(DBG:) Timeout\r\n");
		return _TIMEOUT;
	}
	u1_printf("(DBG:) Succeed\r\n");
	return _SUCCEED;
}

WIFI_StateTypeDef esp8266_HardwareReset(uint32_t waitms) {
	int timeout = waitms;
	WIFI_RST_Enable();
	HAL_Delay(500);
	WIFI_RST_Disable();
	while (timeout--) {
		if (USART2_RecvEndFlag == 1) {

			u1_printf("(DBG:) Hardware Reset OK!\r\n");
			HAL_Delay(100);

			USART2_RxLen = 0;
			USART2_RecvEndFlag = 0;
			HAL_UART_Receive_DMA(&huart2, USART2_RxBUF, USART2_MAX_RECVLEN);

			return _SUCCEED;
		}
		HAL_Delay(1);
	}
	if (timeout <= 0) {
		u1_printf("(DBG:) Finish waiting\r\n");
		u1_printf("\r\n(DBG:) Timeout\r\n");
		return _TIMEOUT;
	}

	return _UNKNOWN_ERROR;
}

WIFI_StateTypeDef esp8266_ConnectAP() {
	uint16_t cmd_len = strlen(AP_SSID) + strlen(AP_PSWD) + 30;
	uint8_t *cmd = (uint8_t*) malloc(cmd_len * sizeof(uint8_t));
	memset(cmd, 0, cmd_len);
	sprintf((char*) cmd, "AT+CWJAP_CUR=\"%s\",\"%s\"", AP_SSID, AP_PSWD);

	if (esp8266_TransmitCmd(cmd, (uint8_t*) "WIFI CONNECTED",
			3 * ESP8266_MAX_TIMEOUT, WITH_NEWLINE) == _SUCCEED)
		wifi_state = _ONLINE;
	else
		wifi_state = _OFFLINE;

	return wifi_state;
}

WIFI_StateTypeDef esp8266_ConnectServer() {
	uint16_t cmd_len = strlen(IpServer) + strlen(ServerPort) + 30;
	uint8_t *cmd = (uint8_t*) malloc(cmd_len * sizeof(uint8_t));
	memset(cmd, 0, cmd_len);
	sprintf((char*) cmd, "AT+CIPSTART=\"TCP\",\"%s\",%s", IpServer, ServerPort);

	if (esp8266_TransmitCmd(cmd, (uint8_t*) "CONNECT", 3 * ESP8266_MAX_TIMEOUT,
	WITH_NEWLINE) == _SUCCEED)
		wifi_state = _CONNECTED;
	else
		wifi_state = _DISCONNECTED;

	return wifi_state;
}

WIFI_StateTypeDef esp8266_SetUpTCPConnection() {
	uint8_t retry_count = 0;

	/* Reset esp8266 */
	u1_printf("(DBG:) Trying to reset esp8266\r\n");
	wifi_config_step++;
	while (esp8266_HardwareReset(500) != _SUCCEED) {
		retry_count++;
		HAL_Delay(1000);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			u1_printf("(DBG:) Reset failed\r\n");
			retry_count = 0;
			trans_state = _UNKNOWN_STATE;
			wifi_config_step--;
			return _FAILED;
		}
	}

	HAL_Delay(2000);		// wait 2 seconds
	retry_count = 0;		// reset retry count

	/* Disable transparent transmission */
	u1_printf("(DBG:) Trying to close transparent transmission\r\n");
	wifi_config_step++;
	while (esp8266_TransmitCmd(TRANS_QUIT_CMD, TRANS_QUIT_CMD,
	ESP8266_MAX_TIMEOUT,
	WITHOUT_NEWLINE) != _SUCCEED) {
		retry_count++;
		HAL_Delay(1500);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			u1_printf("(DBG:) Close transparent transmission failed\r\n");
			retry_count = 0;
			wifi_config_step--;
			return _FAILED;
		}
	}

	trans_state = _TRANS_DISABLE;
	HAL_Delay(1500);
	retry_count = 0;		// reset retry count

	/* Close echo */
	u1_printf("(DBG:) Trying to close echo\r\n");
	wifi_config_step++;
	while (esp8266_TransmitCmd((uint8_t*) "ATE0", OK_ACK, 500, WITH_NEWLINE)
			!= _SUCCEED) {
		retry_count++;
		HAL_Delay(1000);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			u1_printf("(DBG:) Close echo failed\r\n");
			retry_count = 0;
			wifi_config_step--;
			return _FAILED;
		}
	}

	HAL_Delay(100);
	retry_count = 0;		// reset retry count

	/* Set wifi mode 0:AP 1:STA 2:AP+STA */
	u1_printf("(DBG:) Trying to set Wifi mode\r\n");
	wifi_config_step++;
	while (esp8266_TransmitCmd((uint8_t*) "AT+CWMODE_CUR=1", OK_ACK, 500,
	WITH_NEWLINE) != _SUCCEED) {
		retry_count++;
		HAL_Delay(1000);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			u1_printf("(DBG:) Set Wifi mode failed\r\n");
			retry_count = 0;
			wifi_config_step--;
			return _FAILED;
		}
	}

	HAL_Delay(100);
	retry_count = 0;		// reset retry count

	/* Disable auto connect */
	u1_printf("(DBG:) Trying to close auto connect\r\n");
	wifi_config_step++;
	while (esp8266_TransmitCmd((uint8_t*) "AT+CWAUTOCONN=0", OK_ACK, 500,
	WITH_NEWLINE) != _SUCCEED) {
		retry_count++;
		HAL_Delay(1000);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			u1_printf("(DBG:) Close auto connect failed\r\n");
			retry_count = 0;
			wifi_config_step--;
			return _FAILED;
		}
	}

	HAL_Delay(100);
	retry_count = 0;		// reset retry count

	/* Connect to AP(Wifi) */
	u1_printf("(DBG:) Trying to connect to AP\r\n");
	wifi_config_step++;
	while (esp8266_ConnectAP() != _ONLINE) {
		retry_count++;
		HAL_Delay(1000);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			u1_printf("(DBG:) Connect to AP failed\r\n");
			retry_count = 0;
			wifi_config_step--;
			return _FAILED;
		}
	}

	HAL_Delay(1000);
	retry_count = 0;		// reset retry count

	/* Try to get AP info */
	if (wifi_state == _ONLINE) {
		while (esp8266_TransmitCmd((uint8_t*) "AT+CWJAP_CUR?", OK_ACK,
		ESP8266_MAX_TIMEOUT, WITH_NEWLINE) != _SUCCEED) {
			retry_count++;
			HAL_Delay(1000);
			if (retry_count > ESP8266_MAX_RETRY_TIME / 2) {
				u1_printf("(DBG:) Get AP msg failed\r\n");
				u1_printf(
						"(DBG:) Connect server process will not be terminated");
				retry_count = 0;
				wifi_config_step--;
				break;
			}
		}
	}

	HAL_Delay(1000);
	retry_count = 0;		// reset retry count

	/* Try to get IP info */
	if (wifi_state == _ONLINE) {
		while (esp8266_TransmitCmd((uint8_t*) "	AT+CIPSTA_CUR?", OK_ACK,
		ESP8266_MAX_TIMEOUT, WITH_NEWLINE) != _SUCCEED) {
			retry_count++;
			HAL_Delay(1000);
			if (retry_count > ESP8266_MAX_RETRY_TIME / 2) {
				u1_printf("(DBG:) Get IP info failed\r\n");
				u1_printf(
						"(DBG:) Connect server process will not be terminated");
				retry_count = 0;
				wifi_config_step--;
				break;
			}
		}
	}

	HAL_Delay(1000);
	retry_count = 0;		// reset retry count

	/* Set DHCP */
	u1_printf("(DBG:) Trying to set DHCP mode\r\n");
	wifi_config_step++;
	while (esp8266_TransmitCmd((uint8_t*) "AT+CWDHCP_CUR=1,1", OK_ACK, 1000,
	WITH_NEWLINE) != _SUCCEED) {
		retry_count++;
		HAL_Delay(1000);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			u1_printf("(DBG:) Set DHCP model failed\r\n");
			retry_count = 0;
			wifi_config_step--;
			return _FAILED;
		}
	}

	HAL_Delay(1000);
	retry_count = 0;

	/* Set single connection */
	u1_printf("(DBG:) Trying to set single connection\r\n");
	wifi_config_step++;
	while (esp8266_TransmitCmd((uint8_t*) "AT+CIPMUX=0", OK_ACK, 1000,
	WITH_NEWLINE) != _SUCCEED) {
		retry_count++;
		HAL_Delay(1000);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			u1_printf("(DBG:) Set single connection model failed\r\n");
			retry_count = 0;
			wifi_config_step--;
			return _FAILED;
		}
	}

	HAL_Delay(1000);
	retry_count = 0;

	/* Set transparent transmission */
	u1_printf("(DBG:) Trying to set transparent transmission mode\r\n");
	wifi_config_step++;
	while (esp8266_TransmitCmd((uint8_t*) "AT+CIPMODE=1", OK_ACK, 1000,
	WITH_NEWLINE) != _SUCCEED) {
		retry_count++;
		HAL_Delay(1000);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			u1_printf("(DBG:) Set transparent transmission mode failed\r\n");
			retry_count = 0;
			wifi_config_step--;
			return _FAILED;
		}
	}

	HAL_Delay(1000);
	retry_count = 0;

	/* Connect to TCP server */
	u1_printf("(DBG:) Trying to connect TCP server\r\n");
	wifi_config_step++;
	while (esp8266_ConnectServer() != _CONNECTED) {
		retry_count++;
		HAL_Delay(1000);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			u1_printf("(DBG:) Connect TCP server failed\r\n");
			retry_count = 0;
			wifi_config_step--;
			return _FAILED;
		}
	}

	HAL_Delay(1000);
	retry_count = 0;

	/* enable data send(transparent transmission) */
	u1_printf("(DBG:) Trying to enable data send\r\n");
	wifi_config_step++;
	while (esp8266_TransmitCmd((uint8_t*) "AT+CIPSEND", OK_ACK, 1000,
	WITH_NEWLINE) != _SUCCEED) {
		retry_count++;
		HAL_Delay(1000);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			u1_printf("(DBG:) Set transparent transmission mode failed\r\n");
			retry_count = 0;
			wifi_config_step--;
			return _FAILED;
		}
	}

	trans_state = _TRANS_ENBALE;
	HAL_Delay(1000);
	retry_count = 0;

	/* send test msg */
//	u1_printf("Test msg is sending to TCP Server\r\n");
//	u2_transmit("This msg means TCP connection has been set up\r\n");
//	u1_printf("Test msg has been send to TCP Server\r\n");
	return _SUCCEED;
}
