

#include "stm32f413zh_24lc64.h"

void Eeprom_24lc64_Transmit_Byte (uint8_t Byte, uint16_t AddrByte){
	uint8_t AddrByte_H, AddrByte_L;

	I2C_Transmit_AddrSlave(ADDR_24LC64, 0); //отправка адреса раба, бит 0 = запись

	AddrByte_H = (AddrByte >> 8); // смещение 16битного адреса для получение старшего байта с 3 пустыми битами (datasheet 24lc64, figure 3-3)
	I2C_Transmit_byte(AddrByte_H);

	AddrByte_L = AddrByte; // присваение значения младшему байту оставшейся части адреса байта
	I2C_Transmit_byte(AddrByte_L);

	I2C_Transmit_byte(Byte);

	I2C_StopBit();

}

uint8_t Eeprom_24lc64_Read_Byte (uint8_t AddrByte){
	uint8_t read_byte, AddrByte_H, AddrByte_L;

	I2C_Transmit_AddrSlave(ADDR_24LC64, 0); //отправка адреса раба, бит 0 = запись

	AddrByte_H = (AddrByte >> 8); // смещение 16битного адреса для получение старшего байта с 3 пустыми битами (datasheet 24lc64, figure 3-3)
	I2C_Transmit_byte(AddrByte_H);

	AddrByte_L = AddrByte; // присваение значения младшему байту оставшейся части адреса байта
	I2C_Transmit_byte(AddrByte_L);

	I2C_Transmit_AddrSlave(ADDR_24LC64, 1); //отправка адреса раба, бит 1 = запись
	while(!(I2C1->SR1 & I2C_SR1_RXNE));

	I2C_StopBit();
	return read_byte = I2C1 -> DR;
}


void Eeprom_24lc64_Transmit_32byte_massive (uint8_t Addr_0_Byte, uint8_t *mas, uint8_t size){
	uint8_t AddrByte_H, AddrByte_L;

	I2C_Transmit_AddrSlave(ADDR_24LC64, 0); //отправка адреса раба, бит 0 = запись

	AddrByte_H = (Addr_0_Byte >> 8); // смещение 16битного адреса для получение старшего байта с 3 пустыми битами (datasheet 24lc64, figure 3-3)
	I2C_Transmit_byte(AddrByte_H);

	AddrByte_L = Addr_0_Byte; // присваение значения младшему байту оставшейся части адреса байта
	I2C_Transmit_byte(AddrByte_L);

	for(uint8_t counter = 0; counter < size; counter++){
		uint8_t byte = mas[counter];
		I2C_Transmit_byte(byte);
	}

}

void Eeprom_24lc64_Read_massive_byte (uint8_t Addr_0_Byte, uint8_t size, uint8_t  *mas){
	uint8_t read_byte, AddrByte_H, AddrByte_L;

	I2C_Transmit_AddrSlave(ADDR_24LC64, 0); //отправка адреса раба, бит 0 = запись

	AddrByte_H = (Addr_0_Byte >> 8); // смещение 16битного адреса для получение старшего байта с 3 пустыми битами (datasheet 24lc64, figure 3-3)
	I2C_Transmit_byte(AddrByte_H);

	AddrByte_L = Addr_0_Byte; // присваение значения младшему байту оставшейся части адреса байта
	I2C_Transmit_byte(AddrByte_L);

	I2C_Transmit_AddrSlave(ADDR_24LC64, 1); //отправка адреса раба, бит 1 = запись
	while(!(I2C1->SR1 & I2C_SR1_RXNE));

	for (uint8_t counter = 0; counter < size; counter++){
		mas[counter] = &I2C1 -> DR;
	}
}
