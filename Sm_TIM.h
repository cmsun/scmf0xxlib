#ifndef SM_TIM_H
#define SM_TIM_H

#include "Sm_Config.h"
#include "Sm_Debug.h"

namespace Sm{

typedef enum {
    PWM_Channel1 = 0,
    PWM_Channel2 = 1,
    PWM_Channel3 = 2,
    PWM_Channel4 = 3
} PWM_Chl;

}

template<uint32_t TIMX,
    Sm::RCC_ClockCmd RCC_APBxPeriphClockCmd,
    uint32_t RCC_APBxPeriph_TIMx,
    IRQn TIMx_IRQn,
    Sm::SmCallback *Time_Callback,
    void **Time_CbArg>
class Sm_TIM{
public:
    static TIM_TypeDef* const mTIMx;

public:
    static void initialize(uint16_t PSC = 1-1, uint16_t ARR = 48-1, //时钟预分频和定时周期
            uint16_t TIM_CounterMode_x = TIM_CounterMode_Up,        //计数方向
            uint16_t TIM_CKD_DIVx = TIM_CKD_DIV1,                   //时钟分割
            uint8_t TIM_RepetitionCounter = 0)               
    {
        Sm_assert(IS_TIM_COUNTER_MODE(TIM_CounterMode_x));
        Sm_assert(IS_TIM_CKD_DIV(TIM_CKD_DIVx));

        RCC_APBxPeriphClockCmd(RCC_APBxPeriph_TIMx, ENABLE);

        TIM_TimeBaseInitTypeDef tim_init;
        tim_init.TIM_Prescaler = PSC;
        tim_init.TIM_Period = ARR;
        tim_init.TIM_ClockDivision = TIM_CKD_DIVx;
        tim_init.TIM_CounterMode = TIM_CounterMode_x;
        //设置RCR寄存器，只对高级定时器1和8有用。作用是每(1+TIM_RepetitionCounter)次更arr溢出才调用一次中断函数
        tim_init.TIM_RepetitionCounter = TIM_RepetitionCounter;
        TIM_TimeBaseInit(mTIMx, &tim_init);

        //设置定时器默认中断优先级，并使能中断线。
        NVIC_SetPriority(TIMx_IRQn, Default_TIM_NVIC_IRQChannelPriority);
        NVIC_EnableIRQ(TIMx_IRQn);
        if(TIM1 == mTIMx)
        {
            NVIC_SetPriority(TIM1_CC_IRQn, Default_TIM_NVIC_IRQChannelPriority);
            NVIC_EnableIRQ(TIM1_CC_IRQn);
        }

        //把TIMx_CR1的URS位置1，使UG=1产生更新事件时不产生更新中断。而溢出更新事件可以正常产生更新中断。
        mTIMx->CR1 |= TIM_CR1_URS;

        //对TIMx_EGR的UG位软件置1会产生一次更新事件。
        //产生更新事件时会把新写入的ARR和PSC的值写入影子寄存器里。
        //所有的寄存器会更新：计数器会被清0，同时预分频器的计数也被请0(但预分频系数不变)。
        //如果设置了TIMx_CR1的URS位为1，则UG=1时只产生更新事件，不产生更新中断。
        //UG=1产生更新事件后硬件会自动把UG位清0。
        mTIMx->EGR |= TIM_EGR_UG;
    }

    static void setNVICPriority(IRQn_Type timx_irqn,
                         uint8_t priority = Default_TIM_NVIC_IRQChannelPriority)
    {
        Sm_assert(IS_NVIC_PRIORITY(priority));

        NVIC_SetPriority(timx_irqn, priority);
    }

    static void ITConfig(uint16_t TIM_IT, Sm::FunctionalState NewState)
    {
        Sm_assert(IS_TIM_IT(TIM_IT));
        Sm_assert(IS_FUNCTIONAL_STATE(NewState));

        TIM_ITConfig(mTIMx, TIM_IT, NewState);
    }

    //设置定时器时钟分频
    static inline void setPSC(uint16_t PSC, bool Immediately = true)
    {
        mTIMx->PSC = PSC;
        if(Immediately)
            mTIMx->EGR |= TIM_EGR_UG;
    }

    //设置ARR值被修改后是否立即更新影子寄存器。
    static void setARRPreload(Sm::FunctionalState NewState)
    {
        // TIM_ARRPreloadConfig((TIM_TypeDef *)mTIMx, NewState);

        if (NewState != DISABLE)
            mTIMx->CR1 |= TIM_CR1_ARPE;
        else
            mTIMx->CR1 &= (uint16_t)~((uint16_t)TIM_CR1_ARPE);
    }

    //设置定时器溢出值
    //是否产即更新影子寄存器由CR1的ARPE寄存器决定
    static inline void setARR(uint16_t ARR)
    {
        mTIMx->ARR = ARR;
    }

    static void setCallback(Sm::SmCallback callback, void *arg = 0)
    {
        //设置回调函数
        *Time_Callback = callback;
        *Time_CbArg = arg;

        //必须先清除中断标志位再使能定时器更新中断，否则不管有没有使能定时器都会产即进入中断。
        TIM_ClearITPendingBit(mTIMx, TIM_IT_Update);

        //使能定时器更新中断
        ITConfig(TIM_IT_Update, ENABLE);
    }

    //最大定时为65535/2=32767ms
    static void init_ms(uint16_t nms)
    {
        Sm_assert(nms > 0 && nms <= 32767);

        mTIMx->PSC = 24000 - 1;  //定时器每秒计数：48000000(次/sec) / 24000 = 2000(次/sec)
        mTIMx->ARR = nms*2 - 1;  //2000(次/sec) / 1000(ms/sec) = 2(次/ms); 所以定时器计数2次为1ms
        mTIMx->EGR = 0x0001;     //UG位置1，产生更新事件，计数清0并把ARR和PSC的值写入影子寄存器
    }

    //最大定时为65535/2=32767us
    static void init_us(uint16_t nus)
    {
        Sm_assert(nus > 0 && nus <= 32767);

        mTIMx->PSC = 24 - 1;     //定时器每秒计数：48000000(次/sec) / 24 = 2000000(次/sec)
        mTIMx->ARR = nus*2 - 1;  //1000000(次/sec) / 1000000(us/sec) = 1(次/us); 所以定时器计数2次为1us
        mTIMx->EGR = 0x0001;     //UG位置1，产生更新事件，计数清0并把ARR和PSC的值写入影子寄存器
    }

    static inline void enable(void)
    {
        // TIM_Cmd(mTIMx, ENABLE);
        mTIMx->CR1 |= TIM_CR1_CEN;

#ifdef ADVANCED_TIMER  //如果是高级定时器
        // TIM_CtrlPWMOutputs(mTIMx, ENABLE);
        mTIMx->BDTR |= TIM_BDTR_MOE;
#endif
    }

    //如果compare=true，则停止后清空计数值，并强制产生一次比较
    static inline void disable(bool compare = true)
    {
#ifdef ADVANCED_TIMER  //如果是高级定时器
        // TIM_CtrlPWMOutputs(mTIMx, DISABLE);
        mTIMx->BDTR &= (uint16_t)(~((uint16_t)TIM_BDTR_MOE));
#endif

        // TIM_Cmd(mTIMx, DISABLE);
        mTIMx->CR1 &= (uint16_t)(~((uint16_t)TIM_CR1_CEN));

        if(compare)
            mTIMx->EGR |= TIM_EGR_UG;
    }

    //TIM_OCMode_PWM1:计数值小于比较值时为有效值。
    //TIM_OCMode_PWM2:计数值大于比较值时为有效值。
    //TIM_OCPolarity_High:计数值为有效值时输出高电平，非有效值时输出低电平
    //TIM_OCPolarity_Low: 计数值为有效值时输出低电平，非有效值时输出高电平
    static void PWMConfig(Sm::PWM_Chl channel,
            uint16_t TIM_OCMode = TIM_OCMode_PWM2,
            uint16_t TIM_OutputState = TIM_OutputState_Enable,
            uint16_t TIM_OCPolarity = TIM_OCPolarity_High)
    {
        TIM_OCInitTypeDef tim_ocinit;
        tim_ocinit.TIM_OCMode = TIM_OCMode;                     //选择PWM模式
        tim_ocinit.TIM_OutputState = TIM_OutputState;           //比较输出使能
        tim_ocinit.TIM_OCPolarity = TIM_OCPolarity;             //输出极性高
#ifdef ADVANCED_TIMER  //如果是高级定时器
        tim_ocinit.TIM_OutputNState = TIM_OutputNState_Disable;
        tim_ocinit.TIM_OCNPolarity =  TIM_OCNPolarity_High;
        tim_ocinit.TIM_OCIdleState = TIM_OCIdleState_Reset;
        tim_ocinit.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
#endif

        if(channel == Sm::PWM_Channel1)
        {
            tim_ocinit.TIM_Pulse = mTIMx->CCR1;
            TIM_OC1Init(mTIMx, &tim_ocinit);
            TIM_OC1PreloadConfig(mTIMx, TIM_OCPreload_Enable); //使能预装载寄存器
        }
        else if(channel == Sm::PWM_Channel2)
        {
            tim_ocinit.TIM_Pulse = mTIMx->CCR2;
            TIM_OC2Init(mTIMx, &tim_ocinit);
            TIM_OC2PreloadConfig(mTIMx, TIM_OCPreload_Enable); //使能预装载寄存器
        }
        else if(channel == Sm::PWM_Channel3)
        {
            tim_ocinit.TIM_Pulse = mTIMx->CCR3;
            TIM_OC3Init(mTIMx, &tim_ocinit);    
            TIM_OC3PreloadConfig(mTIMx, TIM_OCPreload_Enable); //使能预装载寄存器
        }
        else if(channel == Sm::PWM_Channel4)
        {
            tim_ocinit.TIM_Pulse = mTIMx->CCR4;
            TIM_OC4Init(mTIMx, &tim_ocinit);
            TIM_OC4PreloadConfig(mTIMx, TIM_OCPreload_Enable); //使能预装载寄存器
        }
    }

    static void PWM_NChannel_Config(Sm::PWM_Chl channel,
            uint16_t TIM_OutputNState_x = TIM_OutputNState_Enable,
            uint16_t TIM_OCNPolarity_x =  TIM_OCNPolarity_Low,
            uint16_t TIM_OCIdleState_x = TIM_OCIdleState_Set,
            uint16_t TIM_OCNIdleState_x = TIM_OCNIdleState_Set)
    {
        uint16_t tmpccer = 0, tmpcr2 = 0;

        tmpccer = mTIMx->CCER;
        tmpcr2 =  mTIMx->CR2;

        /* Reset the Output N State */
        tmpccer &= (uint16_t)(~(TIM_CCER_CC1NE<<channel*4));    
        /* Set the Output N State */
        tmpccer |= TIM_OutputNState_x << (channel*4);

        /* Reset the Output N Polarity level */
        tmpccer &= (uint16_t)(~(TIM_CCER_CC1NP<<channel*4));
        /* Set the Output N Polarity */
        tmpccer |= TIM_OCNPolarity_x<<channel*4;

        /* Reset the Ouput Compare and Output Compare N IDLE State */
        tmpcr2 &= (uint16_t)(~((uint16_t)TIM_CR2_OIS1<<channel*2));
        tmpcr2 &= (uint16_t)(~((uint16_t)TIM_CR2_OIS1N<<channel*2));

        /* Set the Output Idle state */
        tmpcr2 |= TIM_OCIdleState_x<<channel*2;
        /* Set the Output N Idle state */
        tmpcr2 |= TIM_OCNIdleState_x<<channel*2;
        Sm_Debug("tmpcr2:%#010x\n", tmpcr2);
 
        mTIMx->CCER = tmpccer;
        mTIMx->CR2 = tmpcr2;
    }

    //初使化PWM后，PWM通道的初始电平是不确定的。所以初始化PWM并设置比较值后应立
    //即设置UG位产生一次更新事件，由此发生一次比效，使PWM初始电平处于正确的电平。
    //所以使用PWMConfig()后第一次使用setCompare()时应使Immediately=true。
    //在PWM输出过程中重新设置比较值时，如果设置UG位则立即更新比较值，否则在下个周期更新比较值
    static void setCompare(Sm::PWM_Chl channel, uint16_t compareVal, bool Immediately = true)
    {
        static volatile uint32_t *compareReg[] = {&mTIMx->CCR1, &mTIMx->CCR2,
                                                  &mTIMx->CCR3, &mTIMx->CCR4};
        *compareReg[channel] = compareVal;
        if(Immediately)
            mTIMx->EGR |= TIM_EGR_UG;//设置UG位，产生更新比较值，并产生一次比较
    }

#ifdef ADVANCED_TIMER  //如果是高级定时器
    static inline void setRCR(uint8_t RCR, bool Immediately = true)
    {
        mTIMx->RCR = RCR;
        if(Immediately)
            mTIMx->EGR |= TIM_EGR_UG;
    }
#endif  //ADVANCED_TIMER

#ifdef EXTRA_CLOCK_MODE2 //如果定时器有外部时钟源模式2功能
    //设置外部时钟模式2，从ETR管脚接入外部脉冲时行计数
    static void initETRClockMode2(uint16_t PSC = 1-1, uint16_t ARR = 0xffff-1)
    {
        //参数1：mTIMx为定时器TIM1、TIM2、TIM3、TIM4、TIM8
        //参数2：TIM_ExtTRGPrescaler外部触发预分频
        //参数3：TIM_ExtTRGPolarity外部时钟极性
        //参数4：外部触发滤波，取值为0x0到0xf之间
        TIM_ETRClockMode2Config(mTIMx, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0);

        mTIMx->PSC = PSC;
        mTIMx->ARR = ARR;
        mTIMx->EGR |= TIM_EGR_UG;
    }
#endif //EXTRA_CLOCK_MODE2

    static void selectOutputTrigger(uint16_t TIM_TRGOSource_x)
    {
        Sm_assert(IS_TIM_TRGO_SOURCE(TIM_TRGOSource_x));

        TIM_SelectOutputTrigger(mTIMx, TIM_TRGOSource_x);
    }

    static void encoderConfig()
    {
/*
 *         init_Endcoder_GPIO();
 * 
 *         TIM_EncoderInterfaceConfig(mTIMx,
 *                                    TIM_EncoderMode_TI12,
 *                                    TIM_ICPolarity_Rising,
 *                                    TIM_ICPolarity_Rising);
 *         TIM_ICInitTypeDef tim_icinit;
 *         tim_icinit.TIM_ICFilter = 6;
 *         TIM_ICInitStruct(&tim_icinit);
 */
    }
};

#endif
