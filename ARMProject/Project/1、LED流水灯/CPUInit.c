#include "stm32f10x.h"
extern float VCC;
void RCC_Configuration(void);
void GPIO_Configuration(void);
void EXTI_Configuration(void);
void NVIC_Configuration(void);
void ADC_Configuration(void);
void DAC_Configuration(void);
void USART_Configuration(void);
void SysTick_Configuration(void);
void I2C_Configuration(void);
void TIM_Configuration(void);
void CPU_Init(void)
{
  RCC_Configuration();
  GPIO_Configuration();
  EXTI_Configuration();
  ADC_Configuration();
  USART_Configuration();
  I2C_Configuration();
  TIM_Configuration();
  NVIC_Configuration();
  SysTick_Configuration();
}

void RCC_Configuration(void)
{
  ErrorStatus HSEStartUpStatus;
  
  /*****************************************************************************
  1、将所有RCC外设寄存器重设为默认值
  *****************************************************************************/
  RCC_DeInit();
  
  /*****************************************************************************
  2、启用外部高速晶振
  *****************************************************************************/
  RCC_HSEConfig(RCC_HSE_ON);

  /*****************************************************************************
  3、等待直到外部高速晶振稳定
  *****************************************************************************/
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {
    /*****************************************************************************
    4、设置预取指缓存使能和代码延时值
    *****************************************************************************/
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);
 
    /*****************************************************************************
    5、设置AHB时钟(HCLK)为系统时钟(SYSCLK)
    *****************************************************************************/
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
  
    /*****************************************************************************
    6、设置APB2时钟(PCLK2)为系统时钟(SYSCLK)
    *****************************************************************************/
    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1); 
    
    /*****************************************************************************
    7、设置APB1时钟(HCLK)为系统时钟/2(SYSCLK/2)
    *****************************************************************************/
    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);
    
    /*****************************************************************************
    设置ADC时钟(ADCLK)为系统时钟/6(SYSCLK/6)=12MHz
    *****************************************************************************/
    RCC_ADCCLKConfig(RCC_PCLK2_Div6); 
    
    /*****************************************************************************
    8、设置PLL2倍频系数，PLL2CLK = (HSE / 5) * 8 = 40 MHz
    *****************************************************************************/
    RCC_PREDIV2Config(RCC_PREDIV2_Div5);//HSE/5
    RCC_PLL2Config(RCC_PLL2Mul_8);//(HSE/5)*8

    /*****************************************************************************
    9、使能PLL2，等待PLL2就绪
    *****************************************************************************/
    RCC_PLL2Cmd(ENABLE);//使能PLL2
    while (RCC_GetFlagStatus(RCC_FLAG_PLL2RDY) == RESET);//等待PLL2就绪

    /*****************************************************************************
    10、设置PLL时钟源与倍频系数，PLLCLK = (PLL2CLK / 5) * 9 = 72 MHz
    *****************************************************************************/
    RCC_PREDIV1Config(RCC_PREDIV1_Source_PLL2, RCC_PREDIV1_Div5);//PLL2CLK/5
    RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_9);//(PLL2CLK / 5) * 9


    /*****************************************************************************
    11、使能PLL，等待PLL就绪
    *****************************************************************************/
    RCC_PLLCmd(ENABLE);//使能PLL
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);//等待PLL就绪

    /*****************************************************************************
    12、设置PLL为系统时钟源，等待其就绪
    *****************************************************************************/
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);//设置PLL为时钟源
    while(RCC_GetSYSCLKSource() != 0x08);//等待系统时钟就绪
  }
  else
  { //HSE启动失败，进入死循环
    while (1);
  }
  
  /*****************************************************************************
    13、开启外设时钟
  *****************************************************************************/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_I2C1|RCC_APB1Periph_USART2,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1|RCC_APB2Periph_ADC2|RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD,ENABLE);
}

void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  GPIO_DeInit(GPIOA);
  GPIO_DeInit(GPIOB);
  GPIO_DeInit(GPIOC);
  GPIO_DeInit(GPIOD);
  GPIO_DeInit(GPIOE);
  
  //USART2端口重映射
  GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE);
  
  //初始化蜂鸣器对应的管脚为开漏输出，并将管脚置高
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA,&GPIO_InitStructure);
  GPIO_SetBits(GPIOA,GPIO_Pin_3);
  
  //初始化四个LED对应的管脚为推挽输出，并将管脚置高  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOD,&GPIO_InitStructure);
  GPIO_SetBits(GPIOD,GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_7);
  
  //将4个按键对应的IO口设成浮空输入模式
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA,&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_Init(GPIOB,&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_13;
  GPIO_Init(GPIOC,&GPIO_InitStructure);
  
  //将4个按键对应的IO口设为外部中断输入管脚
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource10);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource4);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource13);
   
  /*****************************************************************************
  ADC输入管脚设置 PA0（雨量传感）：PB0（光敏传感器））
  *****************************************************************************/
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /*****************************************************************************
  USART管脚设置
  *****************************************************************************/
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;//PD5设成AFPP用于发送
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;//PD6设成IN_FLOATING用于接收
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  //I2C1管脚初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB,&GPIO_InitStructure);
  
  //SHT1x SCK管脚初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC,&GPIO_InitStructure);
  
  //SHT1x DATA管脚初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA,&GPIO_InitStructure);
}


void EXTI_Configuration(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
  
  EXTI_DeInit();
  //初始化外部中断为下降沿触发
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  EXTI_InitStructure.EXTI_Line = EXTI_Line4;
  EXTI_Init(&EXTI_InitStructure);
  
  EXTI_InitStructure.EXTI_Line = EXTI_Line10;
  EXTI_Init(&EXTI_InitStructure);
  
  EXTI_InitStructure.EXTI_Line = EXTI_Line13;
  EXTI_Init(&EXTI_InitStructure);
}


void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);
  //设置优先级分组，两位抢占优先级，两位响应优先级
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  //设置按键输入中断
  //KEY4
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  //KEY1
  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  //KEY2、KEY3
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  //USART2
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void ADC_Configuration(void)
{
  ADC_InitTypeDef ADC_InitStructure;  
  u8 i;
  u32 result;
  //将ADC1的寄存器复位到默认值
  ADC_DeInit(ADC1);
  
  //初始化ADC
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//独立工作模式
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;//扫描模式不使能
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//连续工作模式不使能
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//没有外部触发源
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//转换结果右对齐
  ADC_InitStructure.ADC_NbrOfChannel = 1;//共有1个通道要转换
  ADC_Init(ADC1, &ADC_InitStructure);

  //打开温度传感器/内部电压参考
  ADC_TempSensorVrefintCmd(ENABLE);

  //打开ADC
  ADC_Cmd(ADC1, ENABLE);
  
  //重置ADC1校准寄存器
  ADC_ResetCalibration(ADC1);
  //等待ADC1校准寄存器重置完毕
  while(ADC_GetResetCalibrationStatus(ADC1));

  //开始校准ADC1
  ADC_StartCalibration(ADC1);
  //等待ADC1校准完毕
  while(ADC_GetCalibrationStatus(ADC1));
  result = 0;
  ADC_RegularChannelConfig(ADC1,ADC_Channel_17,1,ADC_SampleTime_71Cycles5);
  for(i=0;i<16;i++)
  {
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);
    result += (ADC_GetConversionValue(ADC1));
  }
  
  VCC = 1.2*16*4096/result; 
}

void USART_Configuration(void)
{
  USART_InitTypeDef USART_InitStructure;
    
  USART_InitStructure.USART_BaudRate = 9600;//波特率9600
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;//数据长度8位
  USART_InitStructure.USART_StopBits = USART_StopBits_1;//1位停止位
  USART_InitStructure.USART_Parity = USART_Parity_No;//没有奇偶校验
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件流控制
  USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;//发送和接收使能

  
  //UART2
  USART_DeInit(USART2);//复位
  USART_Init(USART2, &USART_InitStructure);//初始化
  USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
  //使能USART2
  USART_Cmd(USART2, ENABLE); 
}
/*******************************************************************************
SysTick初始化函数
*******************************************************************************/
void SysTick_Configuration(void)
{
  /*****************************************************************************
  时钟运行频率与CPU频率（72MHz）一样，即定时器每秒钟跳动72000000，假设要每秒钟产
  生n次中断，则SysTick_Config的参数应设为CPUClock/n
  *****************************************************************************/
  if(SysTick_Config(72000000/1000))
    while(1);
  /*****************************************************************************
  将SysTick中断优先级设为最高，避免在其他中断处理函数中调用delay函数时由于SysTick
  无法进入中断而使得delay函数不能退出，注意在与SysTick抢占式优先级一样或更高的中
  断处理程序中不能调用delay函数
  *****************************************************************************/
  NVIC_SetPriority(SysTick_IRQn,0x0);    
}

/*******************************************************************************
I2C1初始化函数
*******************************************************************************/
void I2C_Configuration(void)
{
  I2C_InitTypeDef  I2C_InitStructure; 

  I2C_DeInit(I2C1);
  /* I2C configuration */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;//I2C模式
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;//占空比
  I2C_InitStructure.I2C_OwnAddress1 = 0xA0;//设备地址
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;//Ack使能
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;//7位地址模式
  I2C_InitStructure.I2C_ClockSpeed = 400000;//400KHz
  
  /* 使能I2C */
  I2C_Cmd(I2C1, ENABLE);
  /* 初始化I2C */
  I2C_Init(I2C1, &I2C_InitStructure);

  /*允许1字节1应答模式*/
  I2C_AcknowledgeConfig(I2C1, ENABLE);   
}

void TIM_Configuration(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  
  TIM_TimeBaseStructure.TIM_Period = 65535;
  TIM_TimeBaseStructure.TIM_Prescaler = 71;//预分频值为72，定时器时钟为72MHz/72=1 MHz
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;//不分频，ETR、TIx做输入时才有用
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数模式

  TIM_DeInit(TIM2);//复位初始化
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);//时基初始化
  TIM_Cmd(TIM2,ENABLE);//使能定时器2
}