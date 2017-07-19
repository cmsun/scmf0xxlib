#ifndef SM_USART1_H
#define SM_USART1_H

#include "Sm_USART.h"

extern std::map<void *, Sm::OnCom> usart1_ComProcessMap;
extern void USART1_Init_DefPin(void);

typedef Sm_USART<USART1_BASE, USART1_IRQn,
                GPIOA_BASE, GPIO_Pin_9,
                GPIOA_BASE, GPIO_Pin_10,
                USART1_Init_DefPin,
                &usart1_ComProcessMap> Sm_USART1;

#endif
