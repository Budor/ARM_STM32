#include "public.h"
#include "systick.h"
#include "usart.h"
#include "led.h"


extern u8  USART1_RX_BUF[USART1_MAX_RECV_LEN];      //���ջ���,���USART1_MAX_RECV_LEN�ֽ�
extern u8  USART1_TX_BUF[USART1_MAX_SEND_LEN];


/****************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/
int main(){	
    SystemInit();	//ϵͳʱ�ӳ�ʼ��
    LED_Init();
    USART1_Init(9600);
	while(1){
        u8 i;
        for(i = 0;i < USART1_MAX_RECV_LEN ;i++){
            u2_printf("%d",*(USART1_RX_BUF + i));
        }
        u2_printf("\r\n///////////////////////////////////////////////////////////\r\n");
        
        for(i = 0;i < USART1_MAX_SEND_LEN ;i++){
            u2_printf("%d",*(USART1_TX_BUF + i));
        }

        GPIO_SetBits(GPIOC,LED);
        delay_ms(5000);
  
        GPIO_ResetBits(GPIOC,LED);
        delay_ms(5000);
        	
    }
}



