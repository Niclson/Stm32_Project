#ifndef _POLLING_I2C_PERIHP_DRIVER_H_
#define _POLLING_I2C_PERIHP_DRIVER_H_

#include <stdint.h>
#include <stddef.h>
#include "stm32f4xx.h"

#define POLLING_I2C_ERROR    (-1)
#define POLLING_I2C_OK       ( 0)

int PollingI2C_Init(I2C_TypeDef *regInst, uint32_t sysFreq, uint32_t outBusFreq);

int PollingI2C_SendData(I2C_TypeDef *regInst, uint8_t addrSlave, uint8_t *pData, size_t sizeData);
int PollingI2C_SendByte(I2C_TypeDef *regInst, uint8_t addrSlave, uint8_t dataByte);

int     PollingI2C_RecvData(I2C_TypeDef *regInst, uint8_t addrSlave, uint8_t *pRecvBuffer, size_t recvLength);
uint8_t PollingI2C_RecvByte(I2C_TypeDef *regInst, uint8_t addrSlave);


#endif /* _POLLING_I2C_PERIHP_DRIVER_H_ */
