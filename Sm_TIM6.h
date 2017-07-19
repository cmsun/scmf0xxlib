#ifndef SM_TIM6_H
#define SM_TIM6_H

#include "Sm_TIM.h"

extern Sm::SmCallback TIM6_Callback;   
extern void *TIM6_CbArg;

typedef Sm_TIM<TIM6_BASE,
        RCC_APB1PeriphClockCmd,
        RCC_APB1Periph_TIM6,
        TIM6_DAC_IRQn,          /* TIM6 and DAC Interrupts */
        &TIM6_Callback,
        &TIM6_CbArg> Sm_TIM6_Base;

class Sm_TIM6 : public Sm_TIM6_Base
{
public:
    Sm_TIM6(uint16_t PSC = 1-1, uint16_t ARR = 48-1,            //时钟预分频和定时周期
            uint16_t TIM_CounterMode_x = TIM_CounterMode_Up,    //计数方向
            uint16_t TIM_CKD_DIVx = TIM_CKD_DIV1,               //时钟分割
            uint8_t TIM_RepetitionCounter = 0)               
    {
        initialize(PSC, ARR, TIM_CounterMode_x, TIM_CKD_DIVx, TIM_RepetitionCounter);
    }
};

#endif
