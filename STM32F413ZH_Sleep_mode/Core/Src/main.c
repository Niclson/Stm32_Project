// STM32F413ZH_SleepMode
#include "main.h"

int main(void)
{
	GPIOA -> MODER = 0xffffffff;
	GPIOB -> MODER = 0xffffffff;
	GPIOC -> MODER = 0xffffffff;
	GPIOD -> MODER = 0xffffffff;
	GPIOE -> MODER = 0xffffffff;
	GPIOF -> MODER = 0xffffffff;
	GPIOH -> MODER = (1<<0);

	RCC->APB1ENR |= RCC_APB1ENR_PWREN;

#if 0 // Stop

	SCB->SCR |= (1 << SCB_SCR_SLEEPDEEP_Pos);
	PWR->CR |= PWR_CR_LPDS;
	PWR->CR &= ~PWR_CR_PDDS;
	while (PWR->CR & (PWR_CR_CWUF));
	__WFI(); // or __WFE
#endif

#if 0 // Sleep

#endif

#if 1 // Stendby

	PWR->CR |= PWR_CR_CWUF;
	PWR->CR |= PWR_CR_PDDS;
	SCB->SCR |= (1<<SCB_SCR_SLEEPDEEP_Pos);
	while (PWR->CR & (PWR_CR_CWUF));
	__WFI();
#endif

  while (1)
  {

  }

}
