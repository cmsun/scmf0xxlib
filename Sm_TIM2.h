#ifndef SM_TIM2_H
#define SM_TIM2_H

#include "Sm_TIM.h"
#include "Sm_GPIOA.h"
#include "Sm_GPIOB.h"

//定时器2有外部时钟源功能，可以对外部脉冲进行计数
#define EXTRA_CLOCK_MODE2

extern Sm::SmCallback TIM2_Callback;
extern void *TIM2_CbArg;

typedef Sm_TIM<TIM2_BASE,
        RCC_APB1PeriphClockCmd,
        RCC_APB1Periph_TIM2,
        TIM2_IRQn,
        &TIM2_Callback,
        &TIM2_CbArg> Sm_TIM2_Base;

class Sm_TIM2 : public Sm_TIM2_Base
{
public:
    Sm_TIM2(uint16_t PSC = 1-1, uint16_t ARR = 48-1,            //时钟预分频和定时周期
            uint16_t TIM_CounterMode_x = TIM_CounterMode_Up,    //计数方向
            uint16_t TIM_CKD_DIVx = TIM_CKD_DIV1,               //时钟分割
            uint8_t TIM_RepetitionCounter = 0)               
    {
        initialize(PSC, ARR, TIM_CounterMode_x, TIM_CKD_DIVx, TIM_RepetitionCounter);
    }

    void CH1_ERT_GPIOA0_Init(void)
    {
        Sm_GPIOA::init_IO(GPIO_Pin_0, GPIO_Mode_AF);
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_2);
    }

    void CH1_ERT_GPIOA5_Init(void)
    {
        Sm_GPIOA::init_IO(GPIO_Pin_5, GPIO_Mode_AF);
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_2);
    }

    void CH1_ERT_GPIOA15_Init(void)
    {
        Sm_GPIOA::init_IO(GPIO_Pin_15, GPIO_Mode_AF);
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_2);
    }

    void CH2_GPIOA1_Init(void)
    {
        Sm_GPIOA::init_IO(GPIO_Pin_1, GPIO_Mode_AF);
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_2);
    }

    void CH2_GPIOB3_Init(void)
    {
        Sm_GPIOB::init_IO(GPIO_Pin_3, GPIO_Mode_AF);
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_2);
    }

    void CH3_GPIOA2_Init(void)
    {
        Sm_GPIOA::init_IO(GPIO_Pin_2, GPIO_Mode_AF);
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_2);
    }

    void CH3_GPIOB10_Init(void)
    {
        Sm_GPIOB::init_IO(GPIO_Pin_10, GPIO_Mode_AF);
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_2);
    }

    void CH4_GPIOA3_Init(void)
    {
        Sm_GPIOA::init_IO(GPIO_Pin_3, GPIO_Mode_AF);
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_2);
    }

    void CH4_GPIOB11_Init(void)
    {
        Sm_GPIOB::init_IO(GPIO_Pin_11, GPIO_Mode_AF);
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_2);
    }
};

#undef EXTRA_CLOCK_MODE2

#endif
