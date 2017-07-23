#include"stm32f10x.h"
#include"key.h"

void key_Init(void){
    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
}

u8 key_Scanner(void){
    u8 key_cou=0;
    if(KEY0_Read == 0){
        Delay(10000);
        if(KEY0_Read== 0){
            while(KEY0_Read == 0);
            key_cou++;
            if(key_cou>2)
                key_cou = 1;
            return key_cou;
        }
    }
    return 0;
}
