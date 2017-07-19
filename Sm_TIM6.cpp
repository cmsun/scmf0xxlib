#include "Sm_TIM6.h"

template<>
TIM_TypeDef* const Sm_TIM6_Base::mTIMx = TIM6;

Sm::SmCallback TIM6_Callback = 0;   
void *TIM6_CbArg = 0;

extern "C" void TIM6_DAC_IRQHandler(void)
{
#ifdef Sm_UCOS_Support 
    OSIntEnter();
#endif

    if(TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
    {
        if(TIM6_Callback)
            TIM6_Callback(TIM6_CbArg);
        TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
    }

#ifdef Sm_UCOS_Support 
    OSIntExit();
#endif
}
