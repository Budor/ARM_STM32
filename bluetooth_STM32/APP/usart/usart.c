#include "usart.h"

//串口发送缓存区   
__align(8) u8 USART1_TX_BUF[USART1_MAX_SEND_LEN];   //发送缓冲,最大USART1_MAX_SEND_LEN字节
#ifdef USART1_RX_EN                                 //如果使能了接收         
//串口接收缓存区   
u8 USART1_RX_BUF[USART1_MAX_RECV_LEN];              //接收缓冲,最大USART1_MAX_RECV_LEN个字节.


//通过判断接收连续2个字符之间的时间差不大于10ms来决定是不是一次连续的数据.
//如果2个字符接收间隔超过10ms,则认为不是1次连续数据.也就是超过10ms没有接收到
//任何数据,则表示此次接收完毕.
//接收到的数据状态
//[15]:0,没有接收到数据;1,接收到了一批数据.
//[14:0]:接收到的数据长度
u16 USART1_RX_STA=0;    

void USART1_IRQHandler(void){
    u8 res;
    
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){//接收到数据
        res =USART_ReceiveData(USART1);     
        if(USART1_RX_STA < USART1_MAX_RECV_LEN){       //还可以接收数据
            TIM_SetCounter(TIM4,0);//计数器清空                       
            if(USART1_RX_STA == 0){
                TIM4_Set(1);        //使能定时器4的中断 
            }
            USART1_RX_BUF[USART1_RX_STA++]=res;     //记录接收到的值    
        }else{
            USART1_RX_STA|=1<<15;                   //强制标记接收完成
        } 
    }                                            
}   

//初始化USART1
//bound:波特率  
void USART1_Init(u32 bound){
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_USART,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_TX,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_RX,ENABLE);


    USART_DeInit(USART);  //复位串口

    //UART2_TX PA.2
    GPIO_InitStructure.GPIO_Pin=USART_TX_Pin;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA,&GPIO_InitStructure);

    //USART1_RX   PA.3
    GPIO_InitStructure.GPIO_Pin=USART_RX_Pin;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING; //浮空输入
    GPIO_Init(GPIOA,&GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式

    USART_Init(USART,&USART_InitStructure); //初始化串口

    //UART_DMA_Config(DMA1_Channel7,(u32)&USART1->DR,(u32)USART1_TX_BUF);//DMA1通道7,外设为串口2,存储器为USART1_TX_BUF 
    USART_Cmd(USART1, ENABLE);                    //使能串口 

    #ifdef USART1_RX_EN         //如果使能了接收
    //使能接收中断
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断   

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;      //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化VIC寄存器
    TIM4_Init(99,7199);     //10ms中断
    USART1_RX_STA=0;        //清零
    TIM4_Set(0);            //关闭定时器4
    #endif      
}

//arr：自动重装值。
//psc：时钟预分频数         
void TIM4_Init(u16 arr,u16 psc)
{   
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能//TIM4时钟使能    

    //定时器TIM4初始化
    TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值   
    TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位

    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能指定的TIM4中断,允许更新中断


    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;      //子优先级2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化VIC寄存器

}
//定时器4中断服务程序            
void TIM4_IRQHandler(void)
{   
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)//是更新中断
    {                  
        USART1_RX_STA|=1<<15;   //标记接收完成
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx更新中断标志    
        TIM4_Set(0);            //关闭TIM4  
    }       
}
//设置TIM4的开关
//sta:0，关闭;1,开启;
void TIM4_Set(u8 sta)
{
    if(sta)
    {

        TIM_SetCounter(TIM4,0);//计数器清空
        TIM_Cmd(TIM4, ENABLE);  //使能TIMx    
    }else TIM_Cmd(TIM4, DISABLE);//关闭定时器4      
}

//串口2,printf 函数
//确保一次发送数据不超过USART1_MAX_SEND_LEN字节
void u2_printf(char* fmt,...)  
{  
    va_list ap;
    va_start(ap,fmt);
    vsprintf((char*)USART1_TX_BUF,fmt,ap);
    va_end(ap);
    while(DMA_GetCurrDataCounter(DMA1_Channel7)!=0);    //等待通道7传输完成   
    UART_DMA_Enable(DMA1_Channel7,strlen((const char*)USART1_TX_BUF));  //通过dma发送出去
}

#endif
