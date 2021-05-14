/*
 * stm32f413zh_i2c.h
 */

#ifndef INC_STM32F413ZH_I2C_H_
#define INC_STM32F413ZH_I2C_H_

#include "stm32f413xx.h"
#include "stm32f4xx.h"
#include "polling_i2c.h"

void I2C_StartBit (void);
void I2C_StopBit(void);

// RW_bit = 0 (запись(write))
void I2C_Transmit_AddrSlave(uint8_t AddrSlave_7bit, uint8_t RW_bit);

void I2C_Transmit_byte(uint8_t Byte);


#endif /* INC_STM32F413ZH_I2C_H_ */
