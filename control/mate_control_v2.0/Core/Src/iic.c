#include "./iic.h"

#define I2C_SCL_PIN        GPIO_PIN_8
#define I2C_SCL_PORT       GPIOA
#define I2C_SDA_PIN        GPIO_PIN_9
#define I2C_SDA_PORT       GPIOC

#define I2C_SCL_H()        HAL_GPIO_WritePin(I2C_SCL_PORT, I2C_SCL_PIN, GPIO_PIN_SET)
#define I2C_SCL_L()        HAL_GPIO_WritePin(I2C_SCL_PORT, I2C_SCL_PIN, GPIO_PIN_RESET)
#define I2C_SDA_H()        HAL_GPIO_WritePin(I2C_SDA_PORT, I2C_SDA_PIN, GPIO_PIN_SET)
#define I2C_SDA_L()        HAL_GPIO_WritePin(I2C_SDA_PORT, I2C_SDA_PIN, GPIO_PIN_RESET)
#define I2C_SDA_READ()     HAL_GPIO_ReadPin(I2C_SDA_PORT, I2C_SDA_PIN)

void I2C_Init(void)
{
    I2C_SCL_H();
    I2C_SDA_H();
}

void I2C_Delay(void)
{
    volatile int i = 350;
    while (i--);
}

void I2C_Start(void)
{
    I2C_SDA_H();
    I2C_SCL_H();
    I2C_Delay();
    I2C_SDA_L();
    I2C_Delay();
    I2C_SCL_L();
}

void I2C_Stop(void)
{
    I2C_SCL_L();
    I2C_SDA_L();
    I2C_Delay();
    I2C_SCL_H();
    I2C_SDA_H();
    I2C_Delay();
}

void I2C_Ack(void)
{
    I2C_SCL_L();
    I2C_Delay();
    I2C_SDA_L();
    I2C_Delay();
    I2C_SCL_H();
    I2C_Delay();
    I2C_SCL_L();
}

void I2C_NoAck(void)
{
    I2C_SCL_L();
    I2C_SDA_H();
    I2C_Delay();
    I2C_SCL_H();
    I2C_Delay();
    I2C_SCL_L();
}

uint8_t I2C_WaitAck(void)
{
    uint8_t ucErrTime=0; 
    I2C_SDA_H();
    I2C_Delay();

    if (I2C_SDA_READ())
    {
        ucErrTime++;
        if(ucErrTime>50)
        {
            I2C_Stop();
            return 1;
        }
        I2C_Delay();
    }
    I2C_SCL_H();
    I2C_Delay();
    I2C_SCL_L();
    return 0;
}

void I2C_SendByte(uint8_t byte)
{
    uint8_t i = 8;
    I2C_SCL_L();
    while (i--)
    {
        if (byte & 0x80)
            I2C_SDA_H();
        else
            I2C_SDA_L();
        byte <<= 1;
        I2C_Delay();
        I2C_SCL_H();
        I2C_Delay();
        I2C_SCL_L();
    }
}

uint8_t I2C_ReadByte(void)
{
    uint8_t i = 8;
    uint8_t byte = 0;
    while (i--)
    {
        I2C_SCL_L();
        I2C_Delay();
        I2C_SCL_H();
        byte <<= 1;
        if (I2C_SDA_READ())
            byte ++;
        I2C_Delay();
    }

    return byte;
}

int32_t I2C_Write(uint8_t addr, uint8_t reg, uint8_t*data,uint32_t length)
{
    uint32_t count = 0;
    I2C_Start();
    I2C_SendByte(addr);
    I2C_WaitAck();
    I2C_SendByte(reg);
    I2C_WaitAck();
    for(count=0; count<length; count++)
    {
        I2C_SendByte(data[count]);
        if(I2C_WaitAck() == 1)return 0;
    }
    I2C_Stop();
    return 1;
}

int32_t I2C_Read(uint8_t addr, uint8_t reg,uint8_t *data, uint32_t length)
{
    uint32_t count = 0;

    I2C_Start();
    I2C_SendByte(addr);
    I2C_WaitAck();
    I2C_SendByte(reg);
    I2C_WaitAck();
    I2C_Start();
    I2C_SendByte(addr + 1);
    I2C_WaitAck();
    for(count=0; count<length; count++)
    {
        if(count!=length-1)
        {
            data[count] = I2C_ReadByte();	
            I2C_Ack(); 
        }
        else data[count] = I2C_ReadByte();

    }
    I2C_NoAck();
    I2C_Stop();
    return 1;
}
