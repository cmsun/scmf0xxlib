#include <map>
#include "Sm_USART2.h"

std::map<void *, Sm::OnCom> usart2_ComProcessMap;

void USART2_Init_DefPin(void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
}

extern "C" void USART2_IRQHandler(void)
{
#ifdef Sm_UCOS_Support 
    OSIntEnter();
#endif

    uint16_t recByte = 0; 

    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
        recByte = USART2->RDR; 

    for(std::map<void *, Sm::OnCom>::iterator itr = usart2_ComProcessMap.begin();
        itr != usart2_ComProcessMap.end(); ++itr)
    {
        itr->second(recByte);
    }

#ifdef Sm_UCOS_Support 
    OSIntExit();
#endif
}
