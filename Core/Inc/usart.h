/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "led.h"
#include "tim.h"

/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

/* USER CODE BEGIN Private defines */
#define dbg_uart         huart1
#define esp8266_uart     huart2

#define USART1_MAX_SENDLEN  1024
#define USART1_MAX_RECVLEN  1024
#define USART2_MAX_SENDLEN  1024
#define USART2_MAX_RECVLEN  1024

/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);

/* USER CODE BEGIN Prototypes */

extern uint8_t USART1_TxBUF[USART1_MAX_SENDLEN];
extern uint8_t USART1_RxBUF[USART1_MAX_RECVLEN];

extern uint8_t USART2_TxBUF[USART2_MAX_SENDLEN];
extern uint8_t USART2_RxBUF[USART2_MAX_RECVLEN];
extern volatile uint8_t USART2_RxLen;
extern volatile uint8_t USART2_RecvEndFlag;

void u1_printf(char *fmt, ...);
void u2_transmit(char *fmt, ...);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
