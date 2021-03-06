#include "main.h"

uint8_t counter = 0;
uint8_t spi_flag_transmit = 0;
uint8_t mas[32]={};

void SPI1_IRQHandler (void){
	if(!(SPI1->SR & SPI_SR_RXNE)){
		mas[counter] = SPI1->DR;
		counter++;
		//return;
	}

}

void SysClock_Init(void) {

	RCC->CR   |= RCC_CR_HSEON;  // включение тактирования вешнего осциллятора
	RCC->CFGR |= RCC_CFGR_SW_HSE; // выбор в качестве источника татовых импульсов  -- HSE
	while((RCC->CFGR & RCC_CFGR_SWS_Msk) != RCC_CFGR_SWS_HSE); // ожидание включения источника тактирования
}
void Init_MX25L3233F (void){
	//1-CH# => PD14
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIODEN; // тактирование порта

	//CH# => PD14
	GPIOD -> MODER |= GPIO_MODER_MODER14_0; //01: General purpose output mode
	GPIOD -> PUPDR |= GPIO_PUPDR_PUPD14_0; //01: Pull-up
}



int main(void)
{
	SysClock_Init(); // тактирование от внешнего кварца
	Init_SPI_1();
	Init_MX25L3233F();

	while (1)
	{

		GPIOD -> ODR &= ~GPIO_ODR_OD14;
		//SPI1_Transmit(0xC3);
		SPI1_Transmit(0x05);
		SPI1_Transmit(0x05);
		SPI1_Transmit(0x05);
		//SPI1_Transmit(0x9F);
		GPIOD -> ODR |= GPIO_ODR_OD14;

	}
}
