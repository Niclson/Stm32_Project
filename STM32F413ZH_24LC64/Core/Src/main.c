#include "main.h"

uint8_t buff_transmit[32]={11,22,33,44,55,66,77,88,99,110,120,130,140,150,160,170,180,190,200,210,220};
uint8_t buff_read[40];


int main(void) {

	uint8_t chipData[8];

	SysClock_Init();
	SysClock_PeriphEnable();

	/* Pin config by i2c */
	GpioCtrl_Config(GPIOB, 9, ALTFUNC_OPEN_DRAIN, PULL_UP, 4);
	GpioCtrl_Config(GPIOB, 8, ALTFUNC_OPEN_DRAIN, PULL_UP, 4);

	PollingI2C_Init(I2C1, SysClock_GetFreq(APB1),10000);

	uint16_t AddrByte = 255;

	Eeprom_24lc64_Transmit_Byte(0b10111011, AddrByte);

	uint8_t Byte = Eeprom_24lc64_Read_Byte(AddrByte);

	Eeprom_24lc64_Transmit_32byte_massive (0x00, &buff_transmit, sizeof(buff_transmit));

	//Eeprom_24lc64_Read_massive_byte(0x00, sizeof(buff_read), buff_read);





	//memset(chipData, 0xFF, sizeof(chipData));

	/* Отправка содержимого массива chipData */
	//PollingI2C_SendData(I2C1, I2C_ADDRESS_SLAVE, chipData, sizeof(chipData));

	/* Отправка байта 0x81 */
	//PollingI2C_SendByte(I2C1, I2C_ADDRESS_SLAVE, 0x81);

	/* Прием sizeof(chipData) байт данных в буфер chipData */
	//PollingI2C_RecvData(I2C1, I2C_ADDRESS_SLAVE, chipData, sizeof(chipData));

	/* Прием 2 байт данных в буфер chipData */
	//PollingI2C_RecvData(I2C1, I2C_ADDRESS_SLAVE, chipData, 2);

	/* Прием 1 байта */
	//chipData[0] = PollingI2C_RecvByte(I2C1, I2C_ADDRESS_SLAVE);
/*
	I2C1->CR1 |= I2C_CR1_START;
	while (!(I2C1->SR1 & I2C_SR1_SB));

	//while(!(I2C1->SR1 & I2C_SR1_TXE));
	I2C1->DR   = 0b10100001;
	I2C1->CR1 |= I2C_CR1_POS;
    //while(!(I2C1->SR1 & I2C_SR1_BTF));
    while(I2C1->CR1 & I2C_CR1_STOP);
////
	__disable_irq();
	        while (!(I2C1->SR1 & I2C_SR1_ADDR) || !(I2C1->SR2));
	        I2C1->CR1 &= ~I2C_CR1_ACK;
	__enable_irq();
	 while(!(I2C1->SR1 & I2C_SR1_BTF));
///
/*
	__disable_irq();
		I2C1->CR1  |= I2C_CR1_STOP;
	    pRecvBuffer[0] = I2C1->DR;
	__enable_irq();
	pRecvBuffer[1] = I2C1->DR;

	while(I2C1->CR1 & I2C_CR1_STOP);

	I2C1->CR1 &= ~I2C_CR1_POS;
*/
/*
	I2C1->DR   = 0b11000011;
	I2C1->CR1 |= I2C_CR1_POS;
*/
	while (1)
	{

	}

}
