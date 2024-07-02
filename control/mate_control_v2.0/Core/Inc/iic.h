#ifndef __IIC_H__
#define __IIC_H_

#include "gpio.h"
#include "stm32f4xx_hal.h"

void I2C_Init(void);
int32_t I2C_Read(uint8_t addr, uint8_t reg,uint8_t *data, uint32_t length);
int32_t I2C_Write(uint8_t addr, uint8_t reg, uint8_t *data,uint32_t length);

void I2C_Delay(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_Ack(void);
void I2C_NoAck(void);
uint8_t I2C_WaitAck(void);
void I2C_SendByte(uint8_t byte);
uint8_t I2C_ReadByte(void);
int32_t I2C_Write(uint8_t addr, uint8_t reg, uint8_t*data,uint32_t length);
int32_t I2C_Read(uint8_t addr, uint8_t reg,uint8_t *data, uint32_t length);



#endif // !__IIC_H__
