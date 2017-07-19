#ifndef SM_GPIOA_H
#define SM_GPIOA_H

#include "Sm_GPIO.h"

typedef Sm_GPIO<GPIOA_BASE, RCC_AHBPeriph_GPIOA, EXTI_PortSourceGPIOA> Sm_GPIOA;

#endif
