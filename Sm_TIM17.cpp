#include "Sm_TIM17.h"

template<>
TIM_TypeDef* const Sm_TIM17_Base::mTIMx = TIM17;

Sm::SmCallback TIM17_Callback = 0;   
void *TIM17_CbArg = 0;

extern "C" void TIM17_IRQHandler(void)
{
#ifdef Sm_UCOS_Support 
    OSIntEnter();
#endif

    if(TIM_GetITStatus(TIM17, TIM_IT_Update) != RESET)
    {
        if(TIM17_Callback)
            TIM17_Callback(TIM17_CbArg);
        TIM_ClearITPendingBit(TIM17, TIM_IT_Update);
    }

#ifdef Sm_UCOS_Support 
    OSIntExit();
#endif
}
