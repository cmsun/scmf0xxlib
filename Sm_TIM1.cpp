#include "Sm_TIM1.h"

template<>
TIM_TypeDef* const Sm_TIM1_Base::mTIMx = TIM1;

Sm::SmCallback TIM1_Callback = 0;   
void *TIM1_CbArg = 0;

extern "C"
{

void TIM1_CC_IRQHandler(void)
{
#ifdef Sm_UCOS_Support 
    OSIntEnter();
#endif

    if(TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET)
    {
        TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
    }

    if(TIM_GetITStatus(TIM1, TIM_IT_CC2) != RESET)
    {
        TIM_ClearITPendingBit(TIM1, TIM_IT_CC2);
    }

    if(TIM_GetITStatus(TIM1, TIM_IT_CC3) != RESET)
    {
        TIM_ClearITPendingBit(TIM1, TIM_IT_CC3);
    }

    if(TIM_GetITStatus(TIM1, TIM_IT_CC4) != RESET)
    {
        TIM_ClearITPendingBit(TIM1, TIM_IT_CC4);
    }

#ifdef Sm_UCOS_Support 
    OSIntExit();
#endif
}

void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{
#ifdef Sm_UCOS_Support 
    OSIntEnter();
#endif

    if(TIM_GetITStatus(TIM1, TIM_IT_Break) != RESET)
    {
        TIM_ClearITPendingBit(TIM1, TIM_IT_Break);
    }

    if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
    {
        if(TIM1_Callback)
            TIM1_Callback(TIM1_CbArg);
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    }

    if(TIM_GetITStatus(TIM1, TIM_IT_Trigger) != RESET)
    {
        TIM_ClearITPendingBit(TIM1, TIM_IT_Trigger);
    }

    if(TIM_GetITStatus(TIM1, TIM_IT_COM) != RESET)
    {
        TIM_ClearITPendingBit(TIM1, TIM_IT_COM);
    }

#ifdef Sm_UCOS_Support 
    OSIntExit();
#endif
}

} //extern "C"
