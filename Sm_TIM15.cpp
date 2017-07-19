#include "Sm_TIM15.h"

template<>
TIM_TypeDef* const Sm_TIM15_Base::mTIMx = TIM15;

Sm::SmCallback TIM15_Callback = 0;   
void *TIM15_CbArg = 0;

extern "C" void TIM15_IRQHandler(void)
{
#ifdef Sm_UCOS_Support 
    OSIntEnter();
#endif

    if(TIM_GetITStatus(TIM15, TIM_IT_Update) != RESET)
    {
        if(TIM15_Callback)
            TIM15_Callback(TIM15_CbArg);
        TIM_ClearITPendingBit(TIM15, TIM_IT_Update);
    }

#ifdef Sm_UCOS_Support 
    OSIntExit();
#endif
}
