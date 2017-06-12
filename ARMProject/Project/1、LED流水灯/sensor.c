#include "stm32f10x.h"
#include <math.h>
void delay(u32 time);
void delay_us(u32 time);
extern u32 sysTime;
extern float VCC;
/*******************************************************************************
BMP085 气压传感器相关函数
*******************************************************************************/
#define BMP085_ADDR 0xEE 
//校正参数结构体
struct
{
  s16 AC1;
  s16 AC2;
  s16 AC3;
  u16 AC4;
  u16 AC5;
  u16 AC6;
  s16 B1;
  s16 B2;
  s16 MB;
  s16 MC;
  s16 MD;
} BMP085_CaliCoeff;
s32 B5;
//读取寄存器函数
void BMP085_BufferRead(u8 *pBuffer,u8 ReadAddr,u8 NumByteToRead)
{
  
}
//读取校正系数
void BMP085_ReadCalCoeff(void)
{
  u8 buffer[22];
  
 
}
//读取温度,返回值精确到0.1度
u16 BMP085_ReadTemp(void)
{
  u8 buffer[2];
  s32 X1,X2,T;
  
  
  return (s16)T;
}
//读取气压
u32 BMP085_ReadPressure(void)
{
  u8 buffer[3];
  s32 UP,B3,B6,X1,X2,X3,P;
  u32 B4,B7;
  u8 oss=3;
  
  
  
  return P;
}

/*******************************************************************************
SHT1x 气压传感器相关函数
*******************************************************************************/
//设置Data管脚为输出
void SHT1x_DataOut(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA,&GPIO_InitStructure);
}
//设置Data管脚为输入
void SHT1x_DataIn(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA,&GPIO_InitStructure);
}
#define SHT1x_DATA1 GPIO_SetBits(GPIOA,GPIO_Pin_9)
#define SHT1x_DATA0 GPIO_ResetBits(GPIOA,GPIO_Pin_9)
#define SHT1x_SCK1 GPIO_SetBits(GPIOC,GPIO_Pin_14)
#define SHT1x_SCK0 GPIO_ResetBits(GPIOC,GPIO_Pin_14)
#define SHT1x_DATA_IN GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_9)
//字节写入,原理详见SHT1x数据手册第3章节
u8 SHT1x_WriteByte(u8 value)
{
  u8 i, error=0;
  
  
  
  return error;
}
//字节读出,原理详见SHT1x数据手册第3章节
u8 SHT1x_ReadByte(u8 ack)
{
  u8 i,val=0;
  
   
  return val;
}
//起始信号,原理详见SHT1x数据手册第3.2章节
void SHT1x_TransStart(void)
{
 
}
//连接复位信号,原理详见SHT1x数据手册第3.4章节
void SHT1x_ConnectionReset(void)
{
  u8 i;
  
 
}
//读状态寄存器，,原理详见SHT1x数据手册第3.6章节
u8 SHT1x_ReadReg(u8 *pvalue)
{
  u8 error=0,crc;
  
  
  return error;
}

//写状态寄存器,原理详见SHT1x数据手册第3.6章节
u8 SHT1x_WriteReg(u8 value)
{
  u8 error=0;
  
  
  return error;
}
//测量温湿度,原理详见SHT1x数据手册第3.6章节
u8 SHT1x_Measure(u16 *pvalue, u8 mode)
{
  u8 error=0,crc;
  
  u32 delaytime;
  
  
  
  return error;
}

//读取温湿度，原理详见SHT1x数据手册第4章节
u8 SHT1x_ReadTempHumi(float *ptemp,float *phumi)
{
  u8 error = 0;
  u16 result;

  
  return error;
}


/*******************************************************************************
GL5528 光敏传感器相关函数
*******************************************************************************/
//ADC读取函数
u16 Read_ADC(u8 channel)
{
  ADC_RegularChannelConfig(ADC1,channel,1,ADC_SampleTime_239Cycles5);
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
  while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);
  return (ADC_GetConversionValue(ADC1));
}

//GL5528采样与亮度计算
float GL5528_ReadLight(void)
{
  u16 value;
  float R,V;
  

}
/*******************************************************************************
雨量传感器相关函数,输出百分比
*******************************************************************************/
u16 Rain_Read(void)
{
  u16 value;
  float R,V;
  
}