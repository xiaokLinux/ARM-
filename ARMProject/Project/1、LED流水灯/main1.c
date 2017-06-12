/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/main.c 
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
//#include "stm32_eval_lcd.h"
#include "CPUInit.h"
#include "ili9320.h"
#include "TouchScreen.h"
#include <stdio.h>


/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
 

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/


///////////////////////////////////////LED//////////////////////////////////////


static void Delay_ARMJISHU(__IO uint32_t nCount)
{
  for (; nCount != 0; nCount--);
}

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */

void ili9320_ColorScreen(void)
{
  u16 i, j, k, Color;
  
  for(i=0,k=0; i<320; i++)
  {
      if(!(k & 0x3F))
      {
          k= k + 19;
      }
      else
      {
          k++;
      }
      j = (k>>1) & 0x1F;
      switch(k>>6)
      {
         case 0:
              Color = (j<<11) ;
              break;
         case 1:
              Color = (j<<11) | (j<<6) ;
              break;
         case 2:
              Color =  (j<<6) ;
              break;
         case 3:
              Color =  (j<<6) | j;
              break;
         case 4:
              Color = j;              
              break;
         case 5:
              Color = (j<<11) | j;
              break;
         default:
              Color = (j<<11) |(j<<6) | j;
      }
      GUI_Line(i, 0, i, 239, Color);
  }
}

int main(void)
{
  u8 i;

  /* ´¥ÃþÆÁ³õÊ¼»¯ */
  ADS7843_Init();

  /* TFT LCD³õÊ¼»¯ */
  ili9320_Initializtion();

  /* Main loop */
  while (1) 
  {
    
      ili9320_ColorScreen(); //ÆÁÄ»ÏÔÊ¾²ÊÌõ
      Delay_ARMJISHU(10000000);
  }

 // CPU_Init();
  ADS7843_Init();
  ili9320_Initializtion();
  
  ili9320_Clear(Blue);
  /*
  for(i=0;i<4;i++)
    ili9320_PutHz24(76+48*i,60,i,Red,HyalineBackColor);
  for(i=0;i<10;i++)
    ili9320_PutHz16(44+24*i,120,i,Red,HyalineBackColor);
  
  ili9320_PutHz12(3,160,0,Red,HyalineBackColor);
  ili9320_PutHz12(23,160,1,Red,HyalineBackColor);

  ili9320_PutChar(43,160,'1',Red,HyalineBackColor);
  ili9320_PutChar(53,160,'5',Red,HyalineBackColor);
  ili9320_PutChar(63,160,'8',Red,HyalineBackColor);
  ili9320_PutChar(73,160,'6',Red,HyalineBackColor);
  ili9320_PutChar(83,160,'8',Red,HyalineBackColor);
  ili9320_PutChar(93,160,'5',Red,HyalineBackColor);
  ili9320_PutChar(103,160,'3',Red,HyalineBackColor);
  ili9320_PutChar(113,160,'6',Red,HyalineBackColor);
  ili9320_PutChar(123,160,'3',Red,HyalineBackColor);
  ili9320_PutChar(133,160,'3',Red,HyalineBackColor);
  ili9320_PutChar(143,160,'1',Red,HyalineBackColor);

  ili9320_PutChar(159,160,'x',Red,HyalineBackColor);
  ili9320_PutChar(169,160,'u',Red,HyalineBackColor);
  ili9320_PutChar(179,160,'y',Red,HyalineBackColor);
  ili9320_PutChar(189,160,'u',Red,HyalineBackColor);
  ili9320_PutChar(199,160,'_',Red,HyalineBackColor);
  ili9320_PutChar(209,160,'w',Red,HyalineBackColor);
  ili9320_PutChar(219,160,'z',Red,HyalineBackColor);
  ili9320_PutChar(229,160,'u',Red,HyalineBackColor);
  ili9320_PutChar(239,160,'@',Red,HyalineBackColor);
  ili9320_PutChar(249,160,'1',Red,HyalineBackColor);
  ili9320_PutChar(259,160,'6',Red,HyalineBackColor);
  ili9320_PutChar(269,160,'3',Red,HyalineBackColor);
  ili9320_PutChar(279,160,'.',Red,HyalineBackColor);
  ili9320_PutChar(289,160,'c',Red,HyalineBackColor);
  ili9320_PutChar(299,160,'o',Red,HyalineBackColor);
  ili9320_PutChar(309,160,'m',Red,HyalineBackColor);
  Delay_ARMJISHU(1000000);
  ili9320_Clear(Blue);
  GUI_Rectangle(100, 50, 200, 100,Red,0);
  Delay_ARMJISHU(1000000);
  GUI_Rectangle(100, 50, 200, 100,Red,1);
  Delay_ARMJISHU(1000000);
  GUI_Circle(100,100,50,Red,0);
  Delay_ARMJISHU(1000000);
  GUI_Circle(100,100,50,Red,1);*/
  /* Main loop */
  while (1)
  {
     ili9320_ColorScreen(); //ÆÁÄ»ÏÔÊ¾²ÊÌõ
      //ili9320_PutStr_16x24_Center(108, c, len,White, HyalineBackColor); //ÏÔÊ¾"www.armjishu.com"
      Delay_ARMJISHU(10000000);

  }  
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
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


/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
