#include "Sm_TIM16.h"

template<>
TIM_TypeDef* const Sm_TIM16_Base::mTIMx = TIM16;

Sm::SmCallback TIM16_Callback = 0;   
void *TIM16_CbArg = 0;

extern "C" void TIM16_IRQHandler(void)
{
#ifdef Sm_UCOS_Support 
    OSIntEnter();
#endif

    if(TIM_GetITStatus(TIM16, TIM_IT_Update) != RESET)
    {
        if(TIM16_Callback)
            TIM16_Callback(TIM16_CbArg);
        TIM_ClearITPendingBit(TIM16, TIM_IT_Update);
    }

#ifdef Sm_UCOS_Support 
    OSIntExit();
#endif
}
