#ifndef SM_TIM16_H
#define SM_TIM16_H

#include "Sm_TIM.h"
#include "Sm_GPIOA.h"
#include "Sm_GPIOB.h"

extern Sm::SmCallback TIM16_Callback;   
extern void *TIM16_CbArg;

typedef Sm_TIM<TIM16_BASE,
        RCC_APB2PeriphClockCmd,
        RCC_APB2Periph_TIM16,
        TIM16_IRQn,
        &TIM16_Callback,
        &TIM16_CbArg> Sm_TIM16_Base;

class Sm_TIM16 : public Sm_TIM16_Base
{
public:
    Sm_TIM16(uint16_t PSC = 1-1, uint16_t ARR = 48-1,           //时钟预分频和定时周期
            uint16_t TIM_CounterMode_x = TIM_CounterMode_Up,    //计数方向
            uint16_t TIM_CKD_DIVx = TIM_CKD_DIV1,               //时钟分割
            uint8_t TIM_RepetitionCounter = 0)               
    {
        initialize(PSC, ARR, TIM_CounterMode_x, TIM_CKD_DIVx, TIM_RepetitionCounter);
    }

    void CH1_GPIOA6_Init(void)
    {
        Sm_GPIOA::init_IO(GPIO_Pin_6, GPIO_Mode_AF);
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_5);
    }

    void CH1_GPIOB8_Init(void)
    {
        Sm_GPIOB::init_IO(GPIO_Pin_8, GPIO_Mode_AF);
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_2);
    }

    void CH1N_GPIOB6_Init(void)
    {
        Sm_GPIOB::init_IO(GPIO_Pin_6, GPIO_Mode_AF);
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_2);
    }
};


#endif /* end of include guard: SM_TIM16_H */
