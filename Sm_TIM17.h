#ifndef SM_TIM17_H
#define SM_TIM17_H

#include "Sm_TIM.h"
#include "Sm_GPIOA.h"
#include "Sm_GPIOB.h"

extern Sm::SmCallback TIM17_Callback;   
extern void *TIM17_CbArg;

typedef Sm_TIM<TIM17_BASE,
        RCC_APB2PeriphClockCmd,
        RCC_APB2Periph_TIM17,
        TIM17_IRQn,
        &TIM17_Callback,
        &TIM17_CbArg> Sm_TIM17_Base;

class Sm_TIM17 : public Sm_TIM17_Base
{
public:
    Sm_TIM17(uint16_t PSC = 1-1, uint16_t ARR = 48-1,           //时钟预分频和定时周期
            uint16_t TIM_CounterMode_x = TIM_CounterMode_Up,    //计数方向
            uint16_t TIM_CKD_DIVx = TIM_CKD_DIV1,               //时钟分割
            uint8_t TIM_RepetitionCounter = 0)               
    {
        initialize(PSC, ARR, TIM_CounterMode_x, TIM_CKD_DIVx, TIM_RepetitionCounter);
    }

    void CH1_GPIOA7_Init(void)
    {
        Sm_GPIOA::init_IO(GPIO_Pin_7, GPIO_Mode_AF);
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_5);
    }

    void CH1_GPIOB9_Init(void)
    {
        Sm_GPIOB::init_IO(GPIO_Pin_9, GPIO_Mode_AF);
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_2);
    }

    void CH1N_GPIOB7_Init(void)
    {
        Sm_GPIOB::init_IO(GPIO_Pin_7, GPIO_Mode_AF);
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_2);
    }
};

#endif
