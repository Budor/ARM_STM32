#include "stm32f10x.h"
#include "motor.h"
void Delay(int n)
{
   for(;n!=0;n--);
}
//////////////////////////////////
void RCC_Configuration(void)
{
   RCC_DeInit();                                    //������RCC�Ĵ�������Ϊȱʡֵ
   RCC_HSEConfig(RCC_HSE_ON);                    //�����ⲿ���پ���HSE��
   RCC_WaitForHSEStartUp();    //�ȴ�HSE����
	if(RCC_WaitForHSEStartUp()== SUCCESS)               //���ɹ����񣬣�����Ϊϵͳ����ʱ�����ã�
	{
		 RCC_PLLConfig(RCC_PLLSource_HSE_Div2,RCC_PLLMul_8);  //HSE����Ƶ��ΪPLLʱ��Դ��PLL�˱�Ƶ
		 RCC_PLLCmd(ENABLE);         //����PLLʱ��
		 RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);    //��PLLʱ����Ϊϵͳʱ��
		 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); //ʹ��APB2��GPIOB��ʱ��Դ
	 }
} 

int main()
{    
    RCC_Configuration() ;
    Motor_Init();
    while(1){
        Delay(100000);
        if(Rea_red == 0){
            GPIO_SetBits(GPIOB,GPIO_Pin_5);  
        }
        else 
            GPIO_ResetBits(GPIOB,GPIO_Pin_5);
    }
}
