
#include "main.h"

int main(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	PWR->CR |= PWR_CR_CWUF;
	PWR->CR |= PWR_CR_PDDS;
	SCB->SCR |= (1<<SCB_SCR_SLEEPDEEP_Pos);
	while (PWR->CR & (PWR_CR_CWUF));
	__WFI();

  while (1)
  {

  }

}
