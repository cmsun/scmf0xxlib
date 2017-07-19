#include "Sm_GPIO.h"

Sm::EXTI_IRQ EXTI_IRQ_Table[16];

static inline void processEXTI(uint32_t line, int index)
{
    if(EXTI->PR & line)
    {
        Sm::EXTI_IRQ &irq = EXTI_IRQ_Table[index]; 
        if(irq.pf_IRQ_Process != NULL)
            irq.pf_IRQ_Process(irq.p_IRQ_Argument);
        EXTI->PR = line;
    }
}

extern "C" {

void EXTI0_1_IRQHandler(void)
{
#ifdef Sm_UCOS_Support 
    OSIntEnter();
#endif

    processEXTI(EXTI_Line0, 0);
    processEXTI(EXTI_Line1, 1);

#ifdef Sm_UCOS_Support 
    OSIntExit();
#endif
}

void EXTI2_3_IRQHandler(void)
{
#ifdef Sm_UCOS_Support 
    OSIntEnter();
#endif

    processEXTI(EXTI_Line2, 2);
    processEXTI(EXTI_Line3, 3);

#ifdef Sm_UCOS_Support 
    OSIntExit();
#endif
}

void EXTI4_15_IRQHandler(void)
{
#ifdef Sm_UCOS_Support 
    OSIntEnter();
#endif

    processEXTI(EXTI_Line4,   4);
    processEXTI(EXTI_Line5,   5);
    processEXTI(EXTI_Line6,   6);
    processEXTI(EXTI_Line7,   7);
    processEXTI(EXTI_Line8,   8);
    processEXTI(EXTI_Line9,   9);
    processEXTI(EXTI_Line10, 10);
    processEXTI(EXTI_Line11, 11);
    processEXTI(EXTI_Line12, 12);
    processEXTI(EXTI_Line13, 13);
    processEXTI(EXTI_Line14, 14);
    processEXTI(EXTI_Line15, 15);

#ifdef Sm_UCOS_Support 
    OSIntExit();
#endif
}

}
