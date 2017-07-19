#ifndef SM_GPIOB_H
#define SM_GPIOB_H

#include "Sm_GPIO.h"

typedef Sm_GPIO<GPIOB_BASE, RCC_AHBPeriph_GPIOB, EXTI_PortSourceGPIOB> Sm_GPIOB;

#endif
