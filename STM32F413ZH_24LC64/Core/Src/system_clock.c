#include "stm32f4xx.h"
#include  "system_clock.h"

#define HSE_FREQ_VALUE	(8000000)



void SysClock_Init(void) {
	RCC->CR   |= RCC_CR_HSEON;
	RCC->CFGR |= RCC_CFGR_SW_HSE;
	while((RCC->CFGR & RCC_CFGR_SWS_Msk) != RCC_CFGR_SWS_HSE);
}



void SysClock_PeriphEnable(void) {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIODEN;
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
}



int32_t SysClock_GetFreq(sysClockBus_t bus) {
	int32_t result = SYSCLOCK_ERROR;

	/* Хардкодим для простоты, но сохранения интерфейса */
	switch (bus) {
	case SYSCLK:
	case AHB   :
	case APB1  :
	case APB2  :
		result = HSE_FREQ_VALUE;
		break;

	default :
		break;
	}

	return (result);
}
