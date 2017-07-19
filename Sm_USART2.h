#ifndef SM_USART2_H
#define SM_USART2_H

#include "Sm_USART.h"

extern std::map<void *, Sm::OnCom> usart2_ComProcessMap;
extern void USART2_Init_DefPin(void);

typedef Sm_USART<USART2_BASE, USART2_IRQn,
                GPIOA_BASE, GPIO_Pin_2,
                GPIOA_BASE, GPIO_Pin_3,
                USART2_Init_DefPin,
                &usart2_ComProcessMap> Sm_USART2;

#endif
