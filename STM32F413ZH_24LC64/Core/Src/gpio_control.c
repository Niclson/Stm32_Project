#include <stddef.h>
#include "gpio_control.h"

#define MODER_REG_MASK	(0x03)

#define REG_CLEAR_BITS(reg, mask)	( (reg) &= ~(mask) )
#define REG_SET_BITS(reg, mask)		( (reg) |=  (mask) )

#define xGPIO_MODE_INPUT		(0x00)
#define xGPIO_MODE_OUTPUT		(0x01)
#define xGPIO_MODE_ALT			(0x02)
#define xGPIO_MODE_ANALOG		(0x03)
#define xGPIO_MODE_SET(reg, pin, mode)	( REG_SET_BITS((reg), ((mode) << ((pin) * 2))) )

#define xGPIO_OUT_TYPE_PUSH_PULL	(0x00)
#define xGPIO_OUT_TYPE_OPEN_DRAIN	(0x01)
#define xGPIO_OUT_TYPE_SET(reg, pin, otype)	( REG_SET_BITS((reg), ((otype) << (pin))) )

#define xGPIO_SPEED_LOW			(0x00)
#define xGPIO_SPEED_MEDIUM		(0x01)
#define xGPIO_SPEED_HIGH		(0x02)
#define xGPIO_SPEED_VERY_HIGH	(0x03)
#define xGPIO_SPEED_SET(reg, pin, speed)	( REG_SET_BITS((reg), ((speed) << ((pin) * 2))) )

#define xGPIO_PUPD_NONE			(0x00)
#define xGPIO_PUPD_PULL_UP		(0x01)
#define xGPIO_PUPD_PULL_DOWN	(0x02)
#define xGPIO_PUPD_SET(reg, pin, pupdType)	( REG_SET_BITS((reg), ((pupdType) << ((pin) * 2))) )




int GpioCtrl_Config(GPIO_TypeDef *port, uint8_t pinNumber,
					gpioMode_t mode, gpioPuPd_t pupdMode, uint8_t funcNumber) {

	int 	 status = GPIO_CONFIG_ERROR;
	uint32_t afrIndex;

	if ((port == NULL) || (pinNumber >= 16) || (funcNumber >= 16)) {
		/* Не допустимые значения аргументов */
		goto error;
	}

	afrIndex = (pinNumber < 8) ? (0) : (1);

	REG_CLEAR_BITS(port->MODER, (MODER_REG_MASK << (pinNumber * 2)));
	REG_CLEAR_BITS(port->OTYPER, (GPIO_OTYPER_OT0_Msk << pinNumber));
	REG_CLEAR_BITS(port->OSPEEDR, (GPIO_OSPEEDR_OSPEED0_Msk << (pinNumber * 2)));
	REG_CLEAR_BITS(port->PUPDR, (GPIO_PUPDR_PUPD0_Msk << (pinNumber * 2)));
	REG_CLEAR_BITS(port->AFR[afrIndex], (0xF << (pinNumber % 8) * 4));

	switch (mode) {
	case INPUT  		  : xGPIO_MODE_SET(port->MODER, pinNumber, xGPIO_MODE_INPUT);  break;
	case ANALOG 	      : xGPIO_MODE_SET(port->MODER, pinNumber, xGPIO_MODE_ANALOG); break;

	case OUTPUT_PUSH_PULL :
		xGPIO_MODE_SET(port->MODER, pinNumber, xGPIO_MODE_OUTPUT);
		xGPIO_OUT_TYPE_SET(port->OTYPER, pinNumber, xGPIO_OUT_TYPE_PUSH_PULL);
		break;

	case OUTPUT_OPEN_DRAIN :
		xGPIO_MODE_SET(port->MODER, pinNumber, xGPIO_MODE_OUTPUT);
		xGPIO_OUT_TYPE_SET(port->OTYPER, pinNumber, xGPIO_OUT_TYPE_OPEN_DRAIN);
		break;

	case ALTFUNC_PUSH_PULL :
		xGPIO_MODE_SET(port->MODER, pinNumber, xGPIO_MODE_ALT);
		xGPIO_OUT_TYPE_SET(port->OTYPER, pinNumber, xGPIO_OUT_TYPE_PUSH_PULL);
		REG_SET_BITS(port->AFR[afrIndex], (funcNumber << (pinNumber % 8) * 4));
		break;

	case ALTFUNC_OPEN_DRAIN :
		xGPIO_MODE_SET(port->MODER, pinNumber, xGPIO_MODE_ALT);
		xGPIO_OUT_TYPE_SET(port->OTYPER, pinNumber, xGPIO_OUT_TYPE_OPEN_DRAIN);
		REG_SET_BITS(port->AFR[afrIndex], (funcNumber << (pinNumber % 8) * 4));
		break;

	default :
		/* Не допустимое значение аргумента */
		goto error;
		break;
	}

	switch (pupdMode) {
	case NONE 	   : xGPIO_PUPD_SET(port->PUPDR, pinNumber, xGPIO_PUPD_NONE);      break;
	case PULL_UP   : xGPIO_PUPD_SET(port->PUPDR, pinNumber, xGPIO_PUPD_PULL_UP);   break;
	case PULL_DOWN : xGPIO_PUPD_SET(port->PUPDR, pinNumber, xGPIO_PUPD_PULL_DOWN); break;
	default :
		/* Не допустимое значение аргумента */
		goto error;
		break;
	}

	status = GPIO_CONFIG_OK;

error:
	return (status);
}
