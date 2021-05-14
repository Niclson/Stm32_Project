#ifndef INC_GPIO_CONTROL_H_
#define INC_GPIO_CONTROL_H_

#include <stdint.h>
#include "stm32f4xx.h"

#define GPIO_CONFIG_ERROR	(-1)
#define GPIO_CONFIG_OK		( 0)

typedef enum {
	INPUT,
	OUTPUT_PUSH_PULL, OUTPUT_OPEN_DRAIN,
	ALTFUNC_PUSH_PULL,ALTFUNC_OPEN_DRAIN,
	ANALOG
} gpioMode_t;


typedef enum {
	NONE, PULL_UP, PULL_DOWN
}gpioPuPd_t;


int GpioCtrl_Config(GPIO_TypeDef *port, uint8_t pinNumber,
					gpioMode_t mode, gpioPuPd_t pupdMode, uint8_t funcNumber);


#endif /* INC_GPIO_CONTROL_H_ */
