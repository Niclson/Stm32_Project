#ifndef INC_SYSTEM_CLOCK_H_
#define INC_SYSTEM_CLOCK_H_

#include <stdint.h>

#define SYSCLOCK_ERROR 	(-1)
#define SYSCLOCK_OK 	( 0)

void SysClock_Init(void);
void SysClock_PeriphEnable(void);


typedef enum {
	SYSCLK, AHB, APB1, APB2
}sysClockBus_t;


int32_t SysClock_GetFreq(sysClockBus_t bus);

#endif /* INC_SYSTEM_CLOCK_H_ */
