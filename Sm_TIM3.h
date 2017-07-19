#ifndef SM_TIM3_H
#define SM_TIM3_H

#include "Sm_TIM.h"
#include "Sm_GPIOA.h"
#include "Sm_GPIOB.h"

//定时器3有外部时钟源功能，可以对外部脉冲进行计数
#define EXTRA_CLOCK_MODE2

extern Sm::SmCallback TIM3_Callback;   
extern void *TIM3_CbArg;

typedef Sm_TIM<TIM3_BASE,
        RCC_APB1PeriphClockCmd,
        RCC_APB1Periph_TIM3,
        TIM3_IRQn,
        &TIM3_Callback,
        &TIM3_CbArg> Sm_TIM3_Base;

class Sm_TIM3 : public Sm_TIM3_Base
{
public:
    Sm_TIM3(uint16_t PSC = 1-1, uint16_t ARR = 48-1,            //时钟预分频和定时周期
            uint16_t TIM_CounterMode_x = TIM_CounterMode_Up,    //计数方向
            uint16_t TIM_CKD_DIVx = TIM_CKD_DIV1,               //时钟分割
            uint8_t TIM_RepetitionCounter = 0)               
    {
        initialize(PSC, ARR, TIM_CounterMode_x, TIM_CKD_DIVx, TIM_RepetitionCounter);
    }

    void CH1_GPIOA6_Init(void)
    {
        Sm_GPIOA::init_IO(GPIO_Pin_6, GPIO_Mode_AF);
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_1);
    }

    void CH1_GPIOB4_Init(void)
    {
        Sm_GPIOB::init_IO(GPIO_Pin_4, GPIO_Mode_AF);
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_1);
    }

    void CH2_GPIOA7_Init(void)
    {
        Sm_GPIOA::init_IO(GPIO_Pin_7, GPIO_Mode_AF);
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_1);
    }

    void CH2_GPIOB5_Init(void)
    {
        Sm_GPIOB::init_IO(GPIO_Pin_5, GPIO_Mode_AF);
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_1);
    }

    void CH3_GPIOB0_Init(void)
    {
        Sm_GPIOB::init_IO(GPIO_Pin_0, GPIO_Mode_AF);
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_1);
    }

    void CH4_GPIOB1_Init(void)
    {
        Sm_GPIOB::init_IO(GPIO_Pin_1, GPIO_Mode_AF);
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_1);
    }
};

#undef EXTRA_CLOCK_MODE2

#endif
