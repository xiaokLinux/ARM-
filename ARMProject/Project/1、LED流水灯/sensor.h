#ifndef __SENSOR_H
#define __SENSOR_H
/* Exported functions ------------------------------------------------------- */
void BMP085_ReadCalCoeff(void);
u16 BMP085_ReadTemp(void);
u32 BMP085_ReadPressure(void);
u8 SHT1x_ReadTempHumi(float *ptemp,float *phumi);
void SHT1x_ConnectionReset(void);
float GL5528_ReadLight(void);
u16 Rain_Read(void);
#endif