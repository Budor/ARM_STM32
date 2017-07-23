#ifndef key_h
#define key_h
# include"stm32f10x.h"
#define KEY0_Read  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)//¶ÁÈ¡°´¼ü 0
 
void key_Init(void);
u8 key_Scanner(void);
extern void Delay(int n);
#endif

