#ifndef SM_TIM1_H
#define SM_TIM1_H

//声明TIM1是高级定时器,对模板类Sm_TIM进行条件编译
#define ADVANCED_TIMER 1

//定时器1有外部时钟源功能，可以对外部脉冲进行计数
#define EXTRA_CLOCK_MODE2

#include "Sm_TIM.h"
#include "Sm_GPIOA.h"
#include "Sm_GPIOB.h"

extern Sm::SmCallback TIM1_Callback;   
extern void *TIM1_CbArg;

typedef Sm_TIM<TIM1_BASE,
        RCC_APB2PeriphClockCmd,
        RCC_APB2Periph_TIM1,
        TIM1_BRK_UP_TRG_COM_IRQn,
        &TIM1_Callback,
        &TIM1_CbArg> Sm_TIM1_Base;

class Sm_TIM1 : public Sm_TIM1_Base
{
public:
    Sm_TIM1(uint16_t PSC = 1-1, uint16_t ARR = 48-1,            //时钟预分频和定时周期
            uint16_t TIM_CounterMode_x = TIM_CounterMode_Up,    //计数方向
            uint16_t TIM_CKD_DIVx = TIM_CKD_DIV1,               //时钟分割
            uint8_t TIM_RepetitionCounter = 0)               
    {
        initialize(PSC, ARR, TIM_CounterMode_x, TIM_CKD_DIVx, TIM_RepetitionCounter);
    }

    void CH1_GPIOA8_Init(void)
    {
        //必须先调用init_IO，再调用GPIO_PinAFConfig，否则复用功能会配置失败。
        Sm_GPIOA::init_IO(GPIO_Pin_8, GPIO_Mode_AF);
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_2);
    }

    void CH1N_GPIOA7_Init(void)
    {
        //必须先调用init_IO，再调用GPIO_PinAFConfig，否则复用功能会配置失败。
        Sm_GPIOA::init_IO(GPIO_Pin_7, GPIO_Mode_AF);
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_2);
    }

    void CH1N_GPIOB13_Init(void)
    {
        //必须先调用init_IO，再调用GPIO_PinAFConfig，否则复用功能会配置失败。
        Sm_GPIOB::init_IO(GPIO_Pin_13, GPIO_Mode_AF);
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_2);
    }

    void CH2_GPIOA9_Init(void)
    {
        //必须先调用init_IO，再调用GPIO_PinAFConfig，否则复用功能会配置失败。
        Sm_GPIOA::init_IO(GPIO_Pin_9, GPIO_Mode_AF);
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_2);
    }

    void CH2N_GPIOB0_Init(void)
    {
        //必须先调用init_IO，再调用GPIO_PinAFConfig，否则复用功能会配置失败。
        Sm_GPIOB::init_IO(GPIO_Pin_0, GPIO_Mode_AF);
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_2);
    }

    void CH2N_GPIOB14_Init(void)
    {
        //必须先调用init_IO，再调用GPIO_PinAFConfig，否则复用功能会配置失败。
        Sm_GPIOB::init_IO(GPIO_Pin_14, GPIO_Mode_AF);
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_2);
    }

    void CH3_GPIOA10_Init(void)
    {
        //必须先调用init_IO，再调用GPIO_PinAFConfig，否则复用功能会配置失败。
        Sm_GPIOA::init_IO(GPIO_Pin_10, GPIO_Mode_AF);
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_2);
    }

    void CH3N_GPIOB1_Init(void)
    {
        //必须先调用init_IO，再调用GPIO_PinAFConfig，否则复用功能会配置失败。
        Sm_GPIOB::init_IO(GPIO_Pin_1, GPIO_Mode_AF);
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_2);
    }

    void CH3N_GPIOB15_Init(void)
    {
        //必须先调用init_IO，再调用GPIO_PinAFConfig，否则复用功能会配置失败。
        Sm_GPIOB::init_IO(GPIO_Pin_15, GPIO_Mode_AF);
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_2);
    }

    void CH4_GPIOA11_Init(void)
    {
        //必须先调用init_IO，再调用GPIO_PinAFConfig，否则复用功能会配置失败。
        Sm_GPIOA::init_IO(GPIO_Pin_11, GPIO_Mode_AF);
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_2);
    }

    void ETR_GPIOA12_Init(void)
    {
        //必须先调用init_IO，再调用GPIO_PinAFConfig，否则复用功能会配置失败。
        // Sm_GPIOA::init_IO(GPIO_Pin_12, GPIO_Mode_AF, );
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_2);
    }
};

#undef EXTRA_CLOCK_MODE2

#undef ADVANCED_TIMER

#endif
