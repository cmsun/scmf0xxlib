#ifndef SM_GPIOC_H
#define SM_GPIOC_H

#include "Sm_GPIO.h"

typedef Sm_GPIO<GPIOC_BASE, RCC_AHBPeriph_GPIOC, EXTI_PortSourceGPIOC> Sm_GPIOC;

#endif
