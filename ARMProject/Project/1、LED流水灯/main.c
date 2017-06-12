/**
  ******************************************************************************
  * @file    USART/Printf/main.c 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    04/16/2010
  * @brief   Main program body
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32_eval.h"
#include "ili9320.h"
#include "ili9320_api.h"
#include <stdio.h>
#include "TouchScreen.h"
#include "CPUInit.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dac.h"
#include "sensor.h"
/*******************************************************************************
中断缓冲器发送
*******************************************************************************/
#define USART2_TX_BUFFER_SIZE 50   //缓冲池大小
u8 USART2_Tx_Buffer[USART2_TX_BUFFER_SIZE+1];//定义缓冲池
u8 USART2_Tx_Rd_Index=0;//缓冲池读取索引号，中断服务程序用
u8 USART2_Tx_Wr_Index=0;//缓冲池写入索引号，putchar函数用，用于标识缓冲区中
u8 USART2_Tx_Counter=0;//缓冲池中数据量、未发送字节数


float VCC;
u16 DAC_data=0;
u8 USART2_RX_Buf=0;
u32 sysTime = 0;//系统时钟


void USART2_Putchar(u8 c)
{
  while(USART2_Tx_Counter>(USART2_TX_BUFFER_SIZE));
  __disable_irq();//将当前程序优先级提到最高，禁止其他中断打断
  
  /*if(USART2_Tx_Counter>(USART2_TX_BUFFER_SIZE))
  {// 缓冲区已满，不执行性任何操作返回
    __enable_irq();//降低当前程序优先级，允许中断
    return;//函数跳出
  }*/
 
  if(USART2_Tx_Counter)
  {//还有数据在发送，直接将要发送的字节写入缓冲区
    USART2_Tx_Buffer[USART2_Tx_Wr_Index]=c;  
  }
  else
  {//当前没有数据在发送，直接将数据写入数据寄存器，使能TC中断
    USART_ITConfig(USART2,USART_IT_TC,ENABLE);
    (void)(USART2->SR==0);
    USART2->DR=c;
  }
  //如果索引号超出范围，返回0
  if(++USART2_Tx_Wr_Index>USART2_TX_BUFFER_SIZE)
    USART2_Tx_Wr_Index=0;
  //数据数量+1
  USART2_Tx_Counter++;
  __enable_irq();
}

void USART2_TX_ISR(void)
{
  if(USART2_Tx_Counter)//发送完一个字节，数据数量-1
      USART2_Tx_Counter--;
  
  if(++USART2_Tx_Rd_Index>USART2_TX_BUFFER_SIZE)//如果索引号超出范围，返回0
      USART2_Tx_Rd_Index=0;
  
    if(USART2_Tx_Counter)
    {//还有数据要发送，继续向数据寄存器写入缓冲区中的数据
      USART2->DR=USART2_Tx_Buffer[USART2_Tx_Rd_Index]; 
    }
    else
    {//缓冲区中所有数据都发送完成，关TC中断
      USART_ITConfig(USART2,USART_IT_TC,DISABLE);
    }
}

/*******************************************************************************
串口中断接搜
*******************************************************************************/
void USART2_RX_ISR(void)
{
  USART2_RX_Buf = USART_ReceiveData(USART2);//将接收到的字符赋值给变量，RXNE标志在此过程中会自动清零
}
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */

void LCD_Putchar(u8 c)
{//每个字符为8X16，,320x240总共可以显示600个字符（15行，每行40个字符）
  static u16 index=0;
  
  if(index>=600)
  {//一屏显示满后清屏重新开始显示
    index = 0;
    ili9320_Clear(Blue);
  }
  if(c=='\r')
  {//回车键
    index = 40*((index/40)+1);
  }
  else
  {
    ili9320_PutChar(8*(index%40),16*(index/40),c,Red,Blue);//用索引号计算要显示的位置
    index++;
  }
  
}
void delay(u32 time)
{
  u32 next = sysTime+time-1;
  while((sysTime-next)&0x80000000);
}

void delay_us(u16 time)
{
  u16 next = TIM2->CNT+time-1;
  while((TIM2->CNT-next)&0x8000);
}

int main(void)
{
  s16 T;
  u32 P;
  float tmpr,humidity,lux,rain;
  u8 *p,checksum,i;
  u32 delaytime;
  /* 处理器初始化 */
  CPU_Init();
  /* 触摸屏初始化 */
  //ADS7843_Init();
  /* TFT LCD初始化 */
  ili9320_Initializtion();
  
  ili9320_Clear(Blue);

  BMP085_ReadCalCoeff();
  SHT1x_ConnectionReset();
  
  delaytime = sysTime + 999;
  /* Main loop */
  while (1) 
  {
      if((delaytime-sysTime)&0x80000000)
      {
        delaytime = sysTime+999;
        
        T = BMP085_ReadTemp();
        P = BMP085_ReadPressure();
        ili9320_PutChar(0,0,'B',Red,Blue);
        ili9320_PutChar(8,0,'M',Red,Blue);
        ili9320_PutChar(16,0,'P',Red,Blue);
        ili9320_PutChar(24,0,'0',Red,Blue);
        ili9320_PutChar(32,0,'8',Red,Blue);
        ili9320_PutChar(40,0,'5',Red,Blue);
        ili9320_PutChar(48,0,'-',Red,Blue);
        ili9320_PutChar(56,0,'-',Red,Blue);
        ili9320_PutChar(64,0,'T',Red,Blue);
        ili9320_PutChar(72,0,':',Red,Blue);
        ili9320_PutChar(80,0,'0'+T/100,Red,Blue);
        ili9320_PutChar(88,0,'0'+(T%100)/10,Red,Blue);
        ili9320_PutChar(96,0,'.',Red,Blue);
        ili9320_PutChar(104,0,'0'+(T%10),Red,Blue);
        ili9320_PutChar(112,0,'-',Red,Blue);
        ili9320_PutChar(120,0,'-',Red,Blue);
        ili9320_PutChar(128,0,'P',Red,Blue);
        ili9320_PutChar(136,0,':',Red,Blue);
        ili9320_PutChar(144,0,'0'+P/100000,Red,Blue);
        ili9320_PutChar(152,0,'0'+(P%100000)/10000,Red,Blue);
        ili9320_PutChar(160,0,'0'+(P%10000)/1000,Red,Blue);
        ili9320_PutChar(168,0,'0'+(P%1000)/100,Red,Blue);
        ili9320_PutChar(176,0,'0'+(P%100)/10,Red,Blue);
        ili9320_PutChar(184,0,'0'+(P%10),Red,Blue);
        
        //测量并显示温湿度
        if(SHT1x_ReadTempHumi(&tmpr,&humidity))
        {
          SHT1x_ConnectionReset();
          ili9320_PutChar(184,20,'-',Red,Blue);
          ili9320_PutChar(192,20,'-',Red,Blue);
          ili9320_PutChar(200,20,'E',Red,Blue);
          ili9320_PutChar(208,20,'R',Red,Blue);
          ili9320_PutChar(216,20,'R',Red,Blue);
        }
        else
        {
          ili9320_PutChar(0,20,'S',Red,Blue);
          ili9320_PutChar(8,20,'H',Red,Blue);
          ili9320_PutChar(16,20,'T',Red,Blue);
          ili9320_PutChar(24,20,'1',Red,Blue);
          ili9320_PutChar(32,20,'x',Red,Blue);
          ili9320_PutChar(40,20,'-',Red,Blue);
          ili9320_PutChar(48,20,'-',Red,Blue);
          ili9320_PutChar(56,20,'T',Red,Blue);
          ili9320_PutChar(64,20,':',Red,Blue);
          ili9320_PutChar(72,20,'0'+(u8)tmpr/10,Red,Blue);
          ili9320_PutChar(80,20,'0'+(u8)tmpr%10,Red,Blue);
          ili9320_PutChar(88,20,'.',Red,Blue);
          ili9320_PutChar(96,20,'0'+((u16)(10*tmpr))%10,Red,Blue);
          ili9320_PutChar(104,20,'-',Red,Blue);
          ili9320_PutChar(112,20,'-',Red,Blue);
          ili9320_PutChar(120,20,'H',Red,Blue);
          ili9320_PutChar(128,20,'R',Red,Blue);
          ili9320_PutChar(136,20,':',Red,Blue);
          ili9320_PutChar(144,20,'0'+(u8)humidity/10,Red,Blue);
          ili9320_PutChar(152,20,'0'+(u8)humidity%10,Red,Blue);
          ili9320_PutChar(160,20,'.',Red,Blue);
          ili9320_PutChar(168,20,'0'+((u16)(10*humidity))%10,Red,Blue);
          ili9320_PutChar(176,20,'%',Red,Blue);
        }
        //测量并显示亮度
        lux = GL5528_ReadLight();
        ili9320_PutChar(0,40,'L',Red,Blue);
        ili9320_PutChar(8,40,'i',Red,Blue);
        ili9320_PutChar(16,40,'g',Red,Blue);
        ili9320_PutChar(24,40,'h',Red,Blue);
        ili9320_PutChar(32,40,'t',Red,Blue);
        ili9320_PutChar(40,40,'-',Red,Blue);
        ili9320_PutChar(48,40,'-',Red,Blue);
        ili9320_PutChar(56,40,'0'+(u16)lux/100,Red,Blue);
        ili9320_PutChar(64,40,'0'+((u16)lux%100)/10,Red,Blue);
        ili9320_PutChar(72,40,'0'+(u16)lux%10,Red,Blue);
        ili9320_PutChar(80,40,'.',Red,Blue);
        ili9320_PutChar(88,40,'0'+((u16)(10*lux))%10,Red,Blue);
        ili9320_PutChar(96,40,'l',Red,Blue);
        ili9320_PutChar(104,40,'u',Red,Blue);
        ili9320_PutChar(112,40,'x',Red,Blue);
        
        //测量并显示雨量
        rain = Rain_Read();
        ili9320_PutChar(0,60,'R',Red,Blue);
        ili9320_PutChar(8,60,'a',Red,Blue);
        ili9320_PutChar(16,60,'i',Red,Blue);
        ili9320_PutChar(24,60,'n',Red,Blue);
        ili9320_PutChar(32,60,'-',Red,Blue);
        ili9320_PutChar(40,60,'-',Red,Blue);
        ili9320_PutChar(48,60,'0'+(u8)rain/10,Red,Blue);
        ili9320_PutChar(56,60,'0'+(u8)rain%10,Red,Blue);
        ili9320_PutChar(64,60,'.',Red,Blue);
        ili9320_PutChar(72,60,'0'+((u16)(10*rain))%10,Red,Blue);
        ili9320_PutChar(80,60,'%',Red,Blue);
        
        
        USART2_Putchar(0xB5);
        USART2_Putchar(0x62);
        USART2_Putchar(22);
        checksum = 22;
        
        p = (u8 *)(&T);
        for(i=0;i<2;i++)
        {
          USART2_Putchar(*(p+i));
          checksum ^= *(p+i);
        }
        
        p = (u8 *)(&P);
        for(i=0;i<4;i++)
        {
          USART2_Putchar(*(p+i));
          checksum ^= *(p+i);
        }
        
        p = (u8 *)(&tmpr);
        for(i=0;i<4;i++)
        {
          USART2_Putchar(*(p+i));
          checksum ^= *(p+i);
        }

        p = (u8 *)(&humidity);
        for(i=0;i<4;i++)
        {
          USART2_Putchar(*(p+i));
          checksum ^= *(p+i);
        }
        
        p = (u8 *)(&lux);
        for(i=0;i<4;i++)
        {
          USART2_Putchar(*(p+i));
          checksum ^= *(p+i);
        }
        
        p = (u8 *)(&rain);
        for(i=0;i<4;i++)
        {
          USART2_Putchar(*(p+i));
          checksum ^= *(p+i);
        }
        USART2_Putchar(checksum);
        
      }
  }
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
