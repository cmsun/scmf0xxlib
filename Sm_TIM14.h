#ifndef SM_TIM14_H
#define SM_TIM14_H

#include "Sm_TIM.h"
#include "Sm_GPIOA.h"
#include "Sm_GPIOB.h"

extern Sm::SmCallback TIM14_Callback;   
extern void *TIM14_CbArg;

typedef Sm_TIM<TIM14_BASE,
        RCC_APB1PeriphClockCmd,
        RCC_APB1Periph_TIM14,
        TIM14_IRQn,
        &TIM14_Callback,
        &TIM14_CbArg> Sm_TIM14_Base;

class Sm_TIM14 : public Sm_TIM14_Base
{
public:
    Sm_TIM14(uint16_t PSC = 1-1, uint16_t ARR = 48-1,           //时钟预分频和定时周期
            uint16_t TIM_CounterMode_x = TIM_CounterMode_Up,    //计数方向
            uint16_t TIM_CKD_DIVx = TIM_CKD_DIV1,               //时钟分割
            uint8_t TIM_RepetitionCounter = 0)               
    {
        initialize(PSC, ARR, TIM_CounterMode_x, TIM_CKD_DIVx, TIM_RepetitionCounter);
    }

    void CH1_GPIOA4_Init(void)
    {
        Sm_GPIOA::init_IO(GPIO_Pin_4, GPIO_Mode_AF);
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_4);
    } 

    void CH1_GPIOA7_Init(void)
    {
        Sm_GPIOA::init_IO(GPIO_Pin_7, GPIO_Mode_AF); 
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_4);
    }

    void CH1_GPIOB1_Init(void)
    {
        Sm_GPIOB::init_IO(GPIO_Pin_1, GPIO_Mode_AF); 
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_0);
    }
};

#endif
