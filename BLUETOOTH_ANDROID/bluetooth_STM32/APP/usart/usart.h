#ifndef __USART_H
#define __USART_H  

#include "stm32f10x.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

//对USART1相关参数的一层封装
//**********************************************************************************
#define RCC_USART  RCC_APB2Periph_GPIOA
#define RCC_TX  RCC_APB2Periph_GPIOA
#define RCC_RX RCC_APB2Periph_GPIOA
#define USART USART1
#define USART_TX_Pin GPIO_Pin_9; //USART1_TX   PA.2
#define USART_RX_Pin GPIO_Pin_10; //USART1_RX   PA.3
//**********************************************************************************

#define USART1_MAX_RECV_LEN     200                 //最大接收缓存字节数
#define USART1_MAX_SEND_LEN     200                 //最大发送缓存字节数


#define USART1_RX_EN            1                   //0,不接收;1,接收.

extern u8  USART1_RX_BUF[USART1_MAX_RECV_LEN];      //接收缓冲,最大USART1_MAX_RECV_LEN字节
extern u8  USART1_TX_BUF[USART1_MAX_SEND_LEN];      //发送缓冲,最大USART1_MAX_SEND_LEN字节
extern u16 USART1_RX_STA;                           //接收数据状态

void TIM4_Set(u8 sta);
void TIM4_Init(u16 arr,u16 psc);
void u2_printf(char* fmt, ...);
void USART1_Init(u32);

#endif
