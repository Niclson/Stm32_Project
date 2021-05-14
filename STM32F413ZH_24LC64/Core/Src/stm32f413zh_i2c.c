#include "stm32f413zh_i2c.h"

void I2C_StartBit (void){
	I2C1 -> CR1 |= I2C_CR1_START; // Оритягивает шины к минусу
	while (!(I2C1 -> SR1 & I2C_SR1_SB));
}

void I2C_StopBit(void){
	I2C1 -> CR1 |= I2C_CR1_STOP;
	while(I2C1 -> CR1 & I2C_CR1_STOP);
}

void I2C_Transmit_AddrSlave(uint8_t AddrSlave_7bit, uint8_t RW_bit){

	I2C_StartBit();

	I2C1 -> DR = (AddrSlave_7bit << 1) | (RW_bit);

	//while (!(I2C1->SR1 & I2C_SR1_ADDR) || !(I2C1->SR2));

	/* Согласно Application Note AN2824,
	 * необходимо отключить прерывания в этом критическом
	 * месте */
	__disable_irq();
	   while (!(I2C1->SR1 & I2C_SR1_ADDR) || !(I2C1->SR2));
	   //I2C1->CR1 |=  I2C_CR1_STOP;
	__enable_irq();

}

void I2C_Transmit_byte(uint8_t Byte){

	while(!(I2C1->SR1 & I2C_SR1_TXE));
	I2C1->DR   =  Byte;
	while(!(I2C1->SR1 & I2C_SR1_BTF));
}
