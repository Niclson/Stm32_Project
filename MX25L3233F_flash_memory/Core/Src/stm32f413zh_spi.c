#include "stm32f413_spi.h"

void Init_SPI_1 (void){

	//NSS1: 		PA4, CN7_17(D24)
	//MOSI1:		PA7, CN7_14(D11)
	//MISO1:		PA6, CN7_12(D12)
	//SCK1:			PA5, CN7_10(D13)

	RCC -> APB2ENR |= RCC_APB2ENR_SPI1EN; // тактирование SPI
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // тактирование порта А



	//MOSI1:		PA7, CN7_14(D11)
	GPIOA -> MODER |= GPIO_MODER_MODER7_1; // 10: Alternate function mode
	GPIOA -> AFR[0] |= GPIO_AFRL_AFRL7_0 | GPIO_AFRL_AFRL7_2; // (AF5)

	//MISO1:		PA6, CN7_12(D12)
	GPIOA -> MODER |= GPIO_MODER_MODER6_1; // 10: Alternate function mode
	GPIOA -> AFR[0] |= GPIO_AFRL_AFRL6_0 | GPIO_AFRL_AFRL6_2; // (AF5)

	//SCK1:			PA5, CN7_10(D13)
	GPIOA -> MODER |= GPIO_MODER_MODER5_1; // 10: Alternate function mode
	GPIOA -> AFR[0] |= GPIO_AFRL_AFRL5_0 | GPIO_AFRL_AFRL5_2; // (AF5)

	//NSS1: 		PA4, CN7_17(D24)
	GPIOA -> MODER |= GPIO_MODER_MODER4_1; // 10: Alternate function mode
	GPIOA -> AFR[0] |= GPIO_AFRL_AFRL4_0 | GPIO_AFRL_AFRL4_2; // (AF5)



	// Настройка регистров SPI:
	SPI1 -> CR1 |= SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_BR_0; //111: fPCLK/256
	SPI1 -> CR1 |= SPI_CR1_CPHA | SPI_CR1_CPOL; // фронты
	SPI1 -> CR1 |= SPI_CR1_MSTR; // конфигурация мастер

	SPI1 -> CR2 |= SPI_CR2_RXNEIE; // разрешение прерывания на приём и на передачу

	SPI1 -> CR1 |= SPI_CR1_SPE; //вкл
	NVIC_EnableIRQ(SPI1_IRQn);  // разрешение глобального прерывания spi

}

void SPI1_Transmit (uint8_t data_byte){

	while(!(SPI1->SR & SPI_SR_TXE));
	SPI1 -> DR = data_byte;
}


void Init_SPI_3 (void){
	//MOSI3		PB5(135)(AF6) | PC12(113) 	   | PD6 (122)
	//MISO3		PB4(134)(AF6) | PC11(112)      |
	//SCK3		PB3(133)(AF6) | PB12(73)       | PC10(111)
	//NSS3		PA4(40) (AF6) | PA15(110)(AF6) |

	RCC -> APB1ENR |= RCC_APB1ENR_SPI3EN; // тактирование SPI3
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOAEN; // тактирование порта

	//MOSI3		PB5(135)(AF6)
	GPIOB -> MODER |= GPIO_MODER_MODER5_1; // 10: Alternate function mode
	GPIOB -> AFR[0] |= GPIO_AFRL_AFRL5_1 | GPIO_AFRL_AFRL5_2; // (AF6)

	//MISO3		PB4(134)
	GPIOB -> MODER |= GPIO_MODER_MODER4_1; // 10: Alternate function mode
	GPIOB -> AFR[0] |= GPIO_AFRL_AFRL4_1 | GPIO_AFRL_AFRL4_2; // (AF6)

	//SCK3		PB3(133)(AF6)
	GPIOB -> MODER |= GPIO_MODER_MODER3_1; // 10: Alternate function mode
	//GPIOB -> PUPDR |= GPIO_PUPDR_PUPD3_0; // подтяжка вверх
	GPIOB -> AFR[0] |= GPIO_AFRL_AFRL3_1 | GPIO_AFRL_AFRL3_2; // (AF6)

	//NSS3		PA4(40) (AF6)
	GPIOA -> MODER |= GPIO_MODER_MODER4_1; // 10: Alternate function mode
	GPIOA -> AFR[0] |= GPIO_AFRL_AFRL4_1 | GPIO_AFRL_AFRL4_2; // (AF6)


	// Настройка регистров SPI:
	SPI3 -> CR1 |= SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_BR_0; //111: fPCLK/256
	SPI3 -> CR1 |= SPI_CR1_CPHA | SPI_CR1_CPOL; // фронты
	SPI3 -> CR1 |= SPI_CR1_MSTR; // конфигурация мастер
	SPI3 -> CR1 |= SPI_CR1_SPE; //вкл

	while(!(SPI3->SR & SPI_SR_TXE));
	SPI3 -> DR = 0xf5;


}
