#include"stm32f10x.h"
//#include"stdio.h"
#define GPIO_Pin_all GPIO_Pin_14|GPIO_Pin_15

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
		 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); //ʹ��APB2��GPIOD��ʱ��Դ
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
