#include"stm32f10x.h"
//#include"stdio.h"
#define GPIO_Pin_all GPIO_Pin_14|GPIO_Pin_15

void RCC_Configuration(void)
{
   RCC_DeInit();                                    //将外设RCC寄存器重设为缺省值
   RCC_HSEConfig(RCC_HSE_ON);                    //开启外部高速晶振（HSE）
   RCC_WaitForHSEStartUp();    //等待HSE起振
	if(RCC_WaitForHSEStartUp()== SUCCESS)               //若成功起振，（下面为系统总线时钟设置）
	{
		 RCC_PLLConfig(RCC_PLLSource_HSE_Div2,RCC_PLLMul_8);  //HSE二分频作为PLL时钟源。PLL八倍频
		 RCC_PLLCmd(ENABLE);         //开启PLL时钟
		 RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);    //将PLL时钟作为系统时钟
		 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); //使能APB2的GPIOD的时钟源
	 }
}  
void GPIO_configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;    
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
}
void Delay(int n)
{
   for(;n!=0;n--);
}



int main()
{    
     RCC_Configuration() ;
     GPIO_configuration ();
 
     while(1)
    { 
			int i=2;
		GPIO_ResetBits(GPIOB,GPIO_Pin_14);     
			Delay(1000000);
		GPIO_SetBits(GPIOB, GPIO_Pin_14);  
			Delay(1000000);
		GPIO_ResetBits(GPIOB,GPIO_Pin_15); 
			Delay(1000000);        
		GPIO_SetBits(GPIOB, GPIO_Pin_15);
			Delay(1000000);
		for(;i!=0;i--)
		{
			 GPIO_ResetBits(GPIOB, GPIO_Pin_all);
					Delay(1000000);    
			 GPIO_SetBits(GPIOB, GPIO_Pin_all);
					Delay(1000000) ;
		}  
	}
}
