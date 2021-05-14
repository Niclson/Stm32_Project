#ifndef _SIMPLE_I2C_PERIHP_DRIVER_H_
#define _SIMPLE_I2C_PERIHP_DRIVER_H_

#include <stdint.h>
#include <stddef.h>
#include "stm32f4xx.h"

#define SIMPLE_I2C_ERROR    (-1)
#define SIMPLE_I2C_OK       ( 0)

typedef enum {
    I2C_SEND, I2C_RECV
} i2cCommType_t;

typedef enum {
    I2C_ACK, I2C_NACK
} i2cAck_t;

int SimpleI2C_Init(I2C_TypeDef *regInst, uint32_t sysFreq, uint32_t outBusFreq);


int SimpleI2C_StartComm(I2C_TypeDef *regInst, uint8_t addrSlave, i2cCommType_t typeComm);
int SimpleI2C_StopComm(I2C_TypeDef *regInst);

int     SimpleI2C_SendByte(I2C_TypeDef *regInst, uint8_t dataByte);
uint8_t SimpleI2C_RecvByte(I2C_TypeDef *regInst, i2cAck_t answerToSlave);

#endif /* _SIMPLE_I2C_PERIHP_DRIVER_H_ */
