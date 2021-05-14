/*
 * stm32f414zh_i2c.c
 *
 *  Created on: May 4, 2021
 *      Author: опт
 */
#include "stm32f413zh_i2c.h"

void Init_i2c_1 (uint8_t freq){
	// PB8 I2C1_SCL
	// PB9 I2C1_SDA

	RCC -> APB1ENR |= RCC_APB1ENR_I2C1EN; // включение тактирования I2C_1

	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOBEN; // тактирование порта

	GPIOB -> MODER |= GPIO_MODER_MODER8_1; // альтернативная функция
	GPIOB -> OTYPER |= GPIO_OTYPER_OT8; // открытый сток
	GPIOB -> PUPDR |= GPIO_PUPDR_PUPD8_0; // подтяжка к питанию
	GPIOB -> OSPEEDR |= GPIO_OSPEEDER_OSPEEDR8_1 | GPIO_OSPEEDER_OSPEEDR8_0; // максимальная скорость
	GPIOB -> AFR[1] |= GPIO_AFRH_AFSEL8_2; //альтернативная функция4, пин 8

	GPIOB -> MODER |= GPIO_MODER_MODER9_1; // альтернативная функция
	GPIOB -> OTYPER |= GPIO_OTYPER_OT9; // открытый сток
	GPIOB -> PUPDR |= GPIO_PUPDR_PUPD9_0; // подтяжка к питанию
	GPIOB -> OSPEEDR |= GPIO_OSPEEDER_OSPEEDR9_1 | GPIO_OSPEEDER_OSPEEDR9_0; // максимальная скорость
	GPIOB -> AFR[1] |= GPIO_AFRH_AFSEL9_2; //альтернативная функция4, пин 9

#if 1
	I2C1 -> CCR = 40; // частота кварца / на чатоту шины, 8000000/200000 = 40
	I2C1 -> TRISE = 9; // ХЗ. путь будет как в даташите = 9

	I2C1 -> CR2 |= (0x3F & freq);// Тактовая частота I2C_1. Такая же как и у шины APB и равна 8

	I2C1 -> CR1 |= I2C_CR1_PE;	// enable

	I2C1 -> CR1 |= I2C_CR1_START;

	I2C1 -> DR = 0xf;

	while(!(I2C1 -> SR1 & (I2C_SR1_ADDR)));


#endif

}
