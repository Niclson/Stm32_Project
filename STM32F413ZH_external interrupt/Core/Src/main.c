// STM32F413ZH_ExternalInterrupt
#include "main.h"


void SysClock_Init(void) {
	RCC->CR   |= RCC_CR_HSEON;
	RCC->CFGR |= RCC_CFGR_SW_HSE;
	while((RCC->CFGR & RCC_CFGR_SWS_Msk) != RCC_CFGR_SWS_HSE);
}

void Init_EXTI(void){
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOCEN; // тактирование порта кнопки
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	/*
	 * Button = PC13
	 */
	// Вывод МК должен быть настрое как вход.
	// На плате вывод резитором притянут на земле. Замыкание происходит на питание

	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC; // ST -- !ПИДАРАСЫ!!!

	//EXTI -> RTSR |= EXTI_RTSR_TR13;	// Прерывание по нарастанию импульса
	EXTI -> FTSR |= EXTI_FTSR_TR13; // Прерывание по спаду импульса

	EXTI -> IMR |= EXTI_IMR_IM13; // Прерывание 13 включено

	NVIC_EnableIRQ(EXTI15_10_IRQn); // Разрешение прерывания

}

int main(void)
{
	SysClock_Init();
	Init_EXTI();
	while (1)
	{

	}

}
