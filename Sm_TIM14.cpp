#include "Sm_TIM14.h"

template<>
TIM_TypeDef* const Sm_TIM14_Base::mTIMx = TIM14;

Sm::SmCallback TIM14_Callback = 0;   
void *TIM14_CbArg = 0;

extern "C" void TIM14_IRQHandler(void)
{
#ifdef Sm_UCOS_Support 
    OSIntEnter();
#endif

    if(TIM_GetITStatus(TIM14, TIM_IT_Update) != RESET)
    {
        if(TIM14_Callback)
            TIM14_Callback(TIM14_CbArg);
        TIM_ClearITPendingBit(TIM14, TIM_IT_Update);
    }

#ifdef Sm_UCOS_Support 
    OSIntExit();
#endif
}
