
#include "main.h"

void EXTI15_10_IRQHandler(void){
	// обработчик прерывания.
	// так как он работает на 5 пинов. нужно проверять откуда пришло нажание -- регистр PR
	if((EXTI -> PR & (EXTI_PR_PR13))){
		EXTI -> PR |= (EXTI_PR_PR13); // сброс, чтоб не завис
	}

		return;
}

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
	// А что это за раздел SYSCFG?
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC; // ST -- !ПИДАРАСЫ!!! Как знать? Где знать??? Где читать?

	// Вывод МК должен быть настрое как вход.
	// На плате вывод резитором притянут на земле. Замыкание происходит на питание

	//EXTI -> RTSR |= EXTI_RTSR_TR13;	// Прерывание по нарастанию импульса
	EXTI -> FTSR |= EXTI_FTSR_TR13; // Прерывание по спаду импульса

	EXTI -> IMR |= EXTI_IMR_IM13; // Прерывание 13 включено

	//А что с это строкой???
	//ошибка(((

	NVIC_EnableIRQ(EXTI15_10_IRQn); // А где брать вот эти начальные названия???да, если для usart
	// а если для Timer?


 //  может в еррор лист что-то будет?


}

int main(void)
{
	SysClock_Init();
	Init_EXTI();
	while (1)
	{
		// что-то он туда постоянно попадает, хотя я ничего не нажимаю...
	}

}
