#ifndef _motor_h
#define _motor_h
#include "stm32f10x.h"

#define mot_h  GPIO_Pin_4
#define mot_l  GPIO_Pin_5
#define Rea_red GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)
////////////////////////////
//马达驱动初始化函数
///////////////////////////
void Motor_Init(void);


#endif
