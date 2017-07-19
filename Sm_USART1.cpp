#include <map>
#include "Sm_USART1.h"
#include "Sm_GPIOC.h"

std::map<void *, Sm::OnCom> usart1_ComProcessMap;

void USART1_Init_DefPin(void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
}

extern "C" void USART1_IRQHandler(void)
{
#ifdef Sm_UCOS_Support 
    OSIntEnter();
#endif

    uint16_t recByte = 0; 

    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
        recByte = USART1->RDR; 

    for(std::map<void *, Sm::OnCom>::iterator itr = usart1_ComProcessMap.begin();
        itr != usart1_ComProcessMap.end(); ++itr)
    {
        itr->second(recByte);
    }

#ifdef Sm_UCOS_Support 
    OSIntExit();
#endif
}
