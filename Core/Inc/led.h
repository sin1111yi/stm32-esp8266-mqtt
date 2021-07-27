#ifndef __LED_H
#define __LED_H

#include "main.h"

#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC

#define LED_Final_On  0
#define LED_Final_Off 1
#define LED_State_On  0
#define LED_State_Off 1

extern uint8_t LED_State;

void LED_On(void);
void LED_Off(void);
void LED_Toggle(void);
void LED_Blink(uint8_t times,uint16_t interval,uint8_t finalState);

#endif
