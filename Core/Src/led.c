#include "led.h"

uint8_t LED_State = LED_State_Off;

void LED_On() {
	LED_State = LED_State_On;
	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, LED_State_On);
}
void LED_Off() {
	LED_State = LED_State_Off;
	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, LED_State_Off);
}

void LED_Toggle() {
	if (LED_State == LED_State_On) {
		LED_State = LED_State_Off;
		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, LED_State_Off);
	} else if (LED_State == LED_State_Off) {
		LED_State = LED_State_On;
		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, LED_State_On);
	}
}

void LED_Blink(uint8_t times, uint16_t interval, uint8_t finalState) {
	while (times > 0) {
		LED_Off();
		HAL_Delay(interval);
		LED_On();
		HAL_Delay(interval);
		times--;
	}

	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, finalState);
	LED_State = finalState;
}
