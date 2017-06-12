/**
  ******************************************************************************
  * @file    USART/Printf/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    04/16/2010
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
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
#include "stm32f10x_it.h"
#include "stm32f10x_GPIO.h"
#include "stm32f10x_EXTI.h"
#include <stdio.h>


/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup USART_Printf
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
extern void Led_Turn_on_1(void);
extern void Led_Turn_on_2(void);
extern void Led_Turn_on_3(void);
extern void Led_Turn_on_4(void);
extern void Led_Turn_off_all(void);
extern void USART2_RX_ISR(void);
extern delay(u32 time);
extern u32 sysTime;
/* Private functions ---------------------------------------------------------*/
extern u16 DAC_data;
/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
/*******************************************************************************
SysTick中断处理函数
*******************************************************************************/
void SysTick_Handler(void)
{
  sysTime++;
}


/*******************************************************************************
* Function Name  : EXTI9_5_IRQHandler
* Description    : This function handles External lines 9 to 5 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

/*******************************************************************************
* Function Name  : EXTI0_IRQHandler
* Description    : This function handles External lines 0 interrupt request.
* Input          : None
* Output         : None
* Return         : None
KEY4 键盘输入中断处理程序
*******************************************************************************/
void EXTI0_IRQHandler(void) /* TouchScreen */
{
  delay(20);
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
    if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==RESET)
        GPIO_ResetBits(GPIOD,GPIO_Pin_7);
    else
        GPIO_SetBits(GPIOD,GPIO_Pin_7);

      EXTI_ClearITPendingBit(EXTI_Line0);
  }
  
}
/*******************************************************************************
* Function Name  : EXTI4_IRQHandler
* Description    : This function handles External lines 4 interrupt request.
* Input          : None
* Output         : None
* Return         : None
KEY1 键盘输入中断处理程序
*******************************************************************************/
void EXTI4_IRQHandler(void) 
{
  delay(20);
  if(EXTI_GetITStatus(EXTI_Line4) != RESET)
  {
    if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4)==RESET)
    {
        GPIO_ResetBits(GPIOD,GPIO_Pin_2);
        delay(20);
        if(DAC_data<3995)DAC_data += 100;
        GPIO_SetBits(GPIOD,GPIO_Pin_2);
    }
      EXTI_ClearITPendingBit(EXTI_Line4);

  }
  }
/*******************************************************************************
* Function Name  : EXTI15_10_IRQHandler
* Description    : This function handles External lines 15 to 10 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI15_10_IRQHandler(void) /* TouchScreen */
{
  delay(20);
  if(EXTI_GetITStatus(EXTI_Line10) != RESET)
  {//KEY2
    if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)==RESET)
    {
        GPIO_ResetBits(GPIOD,GPIO_Pin_3);
        if(DAC_data>=100) DAC_data -= 100;
        delay(20);
        GPIO_SetBits(GPIOD,GPIO_Pin_3);
    }

      EXTI_ClearITPendingBit(EXTI_Line10);
  }
  
  if(EXTI_GetITStatus(EXTI_Line13) != RESET)
  {//KEY3
    if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)==RESET)
        GPIO_ResetBits(GPIOD,GPIO_Pin_4);
      else
        GPIO_SetBits(GPIOD,GPIO_Pin_4);
      
      EXTI_ClearITPendingBit(EXTI_Line13);
  }
}
/*******************************************************************************
* Function Name  : USART2_IRQHandler
* Description    : This function handles USART2 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART2_IRQHandler(void)
{
  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  {//判断是否为RXNE中断
    USART2_RX_ISR();  
  }  
  if(USART_GetITStatus(USART2, USART_IT_TC) != RESET)
  {
    USART2_TX_ISR();  
  }  
}
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
