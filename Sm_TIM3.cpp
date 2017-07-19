#include "Sm_TIM3.h"

template<>
TIM_TypeDef* const Sm_TIM3_Base::mTIMx = TIM3;

Sm::SmCallback TIM3_Callback = 0;   
void *TIM3_CbArg = 0;

extern "C" void TIM3_IRQHandler(void)
{
#ifdef Sm_UCOS_Support 
    OSIntEnter();
#endif

    if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        if(TIM3_Callback)
            TIM3_Callback(TIM3_CbArg);
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }

#ifdef Sm_UCOS_Support 
    OSIntExit();
#endif
}
