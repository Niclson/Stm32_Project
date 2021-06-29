/*
 * stm32f413_spi.h
 *
 *  Created on: Jun 7, 2021
 *      Author: Nikolai
 */

#ifndef INC_STM32F413_SPI_H_
#define INC_STM32F413_SPI_H_

#include "stm32f413xx.h"

void Init_SPI_1 (void);
void SPI1_Transmit (uint8_t data_byte);

void Init_SPI_3 (void);

#endif /* INC_STM32F413_SPI_H_ */
