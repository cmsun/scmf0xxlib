#include "Sm_TIM2.h"

template<>
TIM_TypeDef* const Sm_TIM2_Base::mTIMx = TIM2;

Sm::SmCallback TIM2_Callback = 0;
void *TIM2_CbArg = 0;

extern "C" void TIM2_IRQHandler(void)
{
#ifdef Sm_UCOS_Support 
    OSIntEnter();
#endif

    if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        if(TIM2_Callback)
            TIM2_Callback(TIM2_CbArg);
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }

#ifdef Sm_UCOS_Support 
    OSIntExit();
#endif
}
