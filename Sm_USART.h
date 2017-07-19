#ifndef SM_USART_H
#define SM_USART_H

#include <map>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "Sm_Config.h"
#include "Sm_Debug.h"

namespace Sm{
typedef void (*OnCom)(uint16_t recByte);
typedef void (*USART_InitRCC)(void);
}

template<uint32_t USARTx, uint8_t USARTx_IRQn,
         uint32_t GPIO_Tx, uint16_t GPIO_Pin_Tx,
         uint32_t GPIO_Rx, uint16_t GPIO_Pin_Rx,
         Sm::USART_InitRCC usart_initRcc,
         std::map<void *, Sm::OnCom> *ComProcessMap>
class Sm_USART{
public:
    Sm_USART(uint32_t BaudRate = 115200,
             uint32_t WordLength = USART_WordLength_8b,
             uint32_t Parity = USART_Parity_No,
             uint32_t StopBits = USART_StopBits_1, 
             uint32_t HardWareFlowControl = USART_HardwareFlowControl_None)
    {
        Sm_assert(IS_USART_BAUDRATE(BaudRate));
        Sm_assert(IS_USART_WORD_LENGTH(WordLength));
        Sm_assert(IS_USART_PARITY(Parity));
        Sm_assert(IS_USART_STOPBITS(StopBits));
        Sm_assert(IS_USART_HARDWARE_FLOW_CONTROL(HardWareFlowControl));

        usart_initRcc();

        uint8_t GPIO_PinSourceTx = 0, GPIO_PinSourceRx = 0;
        for(uint8_t i = 0; i < 16; ++i)
        {
            if(1<<i == GPIO_Pin_Tx)
            {
                GPIO_PinSourceTx = i;
                break;
            }
        }
        for(uint8_t i = 0; i <= 16; ++i)
        {
            if(1<<i == GPIO_Pin_Rx)
            {
                GPIO_PinSourceRx = i;
                break;
            }
        }

        GPIO_InitTypeDef gpio_init;
        gpio_init.GPIO_Pin = GPIO_Pin_Tx;
        gpio_init.GPIO_Mode = GPIO_Mode_AF;
        gpio_init.GPIO_OType = GPIO_OType_PP;
        gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
        gpio_init.GPIO_PuPd = GPIO_PuPd_UP;
        GPIO_Init((GPIO_TypeDef *)GPIO_Tx, &gpio_init);

        gpio_init.GPIO_Pin = GPIO_Pin_Rx;
        GPIO_Init((GPIO_TypeDef *)GPIO_Rx, &gpio_init);

        //GPIO复用作为串口
        GPIO_PinAFConfig((GPIO_TypeDef *)GPIO_Tx, GPIO_PinSourceTx, GPIO_AF_1);
        GPIO_PinAFConfig((GPIO_TypeDef *)GPIO_Rx, GPIO_PinSourceRx, GPIO_AF_1);

        //USART初始化
        USART_InitTypeDef usart_init;
        usart_init.USART_BaudRate = BaudRate;
        usart_init.USART_WordLength = WordLength;
        usart_init.USART_Parity = Parity;
        usart_init.USART_StopBits = StopBits;
        usart_init.USART_HardwareFlowControl = HardWareFlowControl;
        usart_init.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
        USART_Init((USART_TypeDef *)USARTx, &usart_init);

        //设置UASRT默认中断优先级，并使能中断线。
        NVIC_SetPriority((IRQn_Type)USARTx_IRQn, Default_USART_NVIC_IRQChannelPriority);
        NVIC_EnableIRQ((IRQn_Type)USARTx_IRQn);

        USART_Cmd((USART_TypeDef *)USARTx, ENABLE);  //使能串口

        //初始化USART后在发送第一个字符之前要先读取USART_ISR寄存器的TC位
        //清除TC位清除，否则第一个数据没发送完成就会被第二个数据覆盖。
        while( !(((USART_TypeDef *)USARTx)->ISR & USART_FLAG_TC) )
            ;
    }

    ~Sm_USART(void)
    {
        ComProcessMap->erase(this);
    }

    void setNVICPriority(int priority)
    {
        Sm_assert(IS_NVIC_PRIORITY(priority));

        NVIC_SetPriority(USARTx_IRQn, priority);
    }

    void ITConfig(uint32_t USART_IT, FunctionalState NewState)
    {
        Sm_assert(IS_USART_CONFIG_IT(USART_IT));
        Sm_assert(IS_FUNCTIONAL_STATE(NewState));

        USART_ITConfig((USART_TypeDef *)USARTx, USART_IT, NewState);
    }

    void setITCallback(Sm::OnCom callback)
    {
        (*ComProcessMap)[this] = callback;

        //使能串口接收中断。
        USART_ITConfig((USART_TypeDef *)USARTx, USART_IT_RXNE, ENABLE);
    }

    //如果使用9位字节方式传输，则位8表示当前字节是数据还是地址。
    static inline void send(uint16_t data)
    {
        ((USART_TypeDef *)USARTx)->TDR = (data & (uint16_t)0x01FF);
        while( !(((USART_TypeDef *)USARTx)->ISR & USART_FLAG_TC) )
            ;
    }

    static void send(uint8_t *data, size_t length)
    {
        for(size_t i = 0; i < length; ++i)
        {
            ((USART_TypeDef *)USARTx)->TDR = data[i];
            while( !(((USART_TypeDef *)USARTx)->ISR & USART_FLAG_TC) )
                ;
        }
    }

    static void printf(const char *format, ...)
    {
        char buff[128];
        va_list fmt;

        va_start(fmt, format);
        vsnprintf(buff, 128, format, fmt);
        va_end(fmt);

        send((uint8_t *)buff, strlen(buff));
    }
};

#endif
