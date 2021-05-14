
#ifndef INC_STM32F413ZH_24LC64_H_
#define INC_STM32F413ZH_24LC64_H_

#include "stm32f413xx.h"
#include "stm32f4xx.h"
#include "stm32f413zh_i2c.h"

#define ADDR_24LC64 (0b1010000)

void Eeprom_24lc64_Transmit_Byte (uint8_t Byte, uint16_t AddrByte);
uint8_t Eeprom_24lc64_Read_Byte (uint8_t AddrByte);
void Eeprom_24lc64_Transmit_32byte_massive (uint8_t Addr_0_Byte, uint8_t *mas, uint8_t size);

#endif /* INC_STM32F413ZH_24LC64_H_ */
