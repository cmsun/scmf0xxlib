#ifndef SM_TIM15_H
#define SM_TIM15_H

#include "Sm_TIM.h"
#include "Sm_GPIOA.h"
#include "Sm_GPIOB.h"

extern Sm::SmCallback TIM15_Callback;   
extern void *TIM15_CbArg;

typedef Sm_TIM<TIM15_BASE,
        RCC_APB2PeriphClockCmd,
        RCC_APB2Periph_TIM15,
        TIM15_IRQn,
        &TIM15_Callback,
        &TIM15_CbArg> Sm_TIM15_Base;

class Sm_TIM15 : public Sm_TIM15_Base
{
public:
    Sm_TIM15(uint16_t PSC = 1-1, uint16_t ARR = 48-1,           //时钟预分频和定时周期
            uint16_t TIM_CounterMode_x = TIM_CounterMode_Up,    //计数方向
            uint16_t TIM_CKD_DIVx = TIM_CKD_DIV1,               //时钟分割
            uint8_t TIM_RepetitionCounter = 0)               
    {
        initialize(PSC, ARR, TIM_CounterMode_x, TIM_CKD_DIVx, TIM_RepetitionCounter);
    }

    void CH1_GPIOA2_Init(void)
    {
        Sm_GPIOA::init_IO(GPIO_Pin_2, GPIO_Mode_AF);
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_0);
    }

    void CH1_GPIOB14_Init(void)
    {
        Sm_GPIOB::init_IO(GPIO_Pin_14, GPIO_Mode_AF);
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_1);
    }

    void CH1N_GPIOB15_Init(void)
    {
        Sm_GPIOB::init_IO(GPIO_Pin_15, GPIO_Mode_AF);
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_3);
    }

    void CH2_GPIOA3_Init(void)
    {
        Sm_GPIOA::init_IO(GPIO_Pin_3, GPIO_Mode_AF);
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_0);
    }

    void CH2_GPIOB15_Init(void)
    {
        Sm_GPIOB::init_IO(GPIO_Pin_15, GPIO_Mode_AF);
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_1);
    }
};

#endif
