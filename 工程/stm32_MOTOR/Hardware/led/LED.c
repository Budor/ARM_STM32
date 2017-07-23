#include "stm32f10x.h"
#include "LED.h"
void LED_GPIO_Init(void){
    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_InitStructure.GPIO_Pin = LED1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_10MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
}
