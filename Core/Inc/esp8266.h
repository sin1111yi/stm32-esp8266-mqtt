#ifndef __ESP8266_H
#define __ESP8266_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "net_conf.h"
#include <string.h>
#include <stdlib.h>
#include "usart.h"

#define OK_ACK            (uint8_t*)"OK"
#define TRANS_QUIT_CMD    (uint8_t*)"+++"

#define WITH_NEWLINE     1
#define WITHOUT_NEWLINE  0

typedef enum __WIFI_StateTypDef {
	_FAILED = 0,        // failed
	_SUCCEED = 1,       // succeed
	_TIMEOUT = 2,       // wait timeout
	_MATCHERROR = 3,    // cmd match error
	_MATCHOK = 4,       // cmd match ok
	_ONLINE = 5,        // wifi connect
	_OFFLINE = 6,       // wifi disconnect
	_CONNECTED = 7,     // server connected
	_DISCONNECTED = 8,  // server disconnected
	_TRANS_ENBALE = 9,
	_TRANS_DISABLE = 10,
	_UNKNOWN_STATE = 0xee,
	_UNKNOWN_ERROR = 0xff
} WIFI_StateTypeDef;

#define ESP8266_MAX_TIMEOUT     (uint16_t)0xffff
#define ESP8266_MAX_RETRY_TIME  10

#define WIFI_RST_Pin         GPIO_PIN_15
#define WIFI_RST_GPIO_Port   GPIOB

#define WIFI_RST_Enable()    HAL_GPIO_WritePin(WIFI_RST_GPIO_Port,WIFI_RST_Pin,RESET)
#define WIFI_RST_Disable() 	HAL_GPIO_WritePin(WIFI_RST_GPIO_Port, WIFI_RST_Pin, SET);

extern uint8_t *esp8266_ack;

WIFI_StateTypeDef esp8266_CheckRespond(uint8_t *str);
WIFI_StateTypeDef esp8266_TransmitCmd(uint8_t *cmd, uint8_t *ack,
		uint32_t waittime, uint8_t newline);
WIFI_StateTypeDef esp8266_HardwareReset(uint32_t timeout);
WIFI_StateTypeDef esp8266_QuitTrans(void);
WIFI_StateTypeDef esp8266_ConnectAP(void);
WIFI_StateTypeDef esp8266_ConnectServer(void);
WIFI_StateTypeDef esp8266_SetUpTCPConnection(void);

#ifdef __cplusplus
}
#endif

#endif
