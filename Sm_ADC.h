#ifndef SM_ADC_H
#define SM_ADC_H

#include "Sm_Config.h"
#include "Sm_GPIOA.h"
#include "Sm_GPIOB.h"
#include "Sm_GPIOC.h"

template<uint32_t ADCx, uint32_t RCC_APB2Periph_ADCx>
class Sm_ADC
{
public:
    Sm_ADC(FunctionalState continuousConvMode = DISABLE,
            uint32_t ADC_Resolution_x = ADC_Resolution_12b,
            uint32_t ADC_ExternalTrigConvEdge_x = ADC_ExternalTrigConvEdge_None,
            uint32_t ADC_ExternalTrigConv_x = ADC_ExternalTrigConv_T1_TRGO,
            uint32_t ADC_DataAlign_x = ADC_DataAlign_Right,
            uint32_t ADC_ScanDirection_x = ADC_ScanDirection_Backward)
    {
        initialize(continuousConvMode,
                ADC_Resolution_x,
                ADC_ExternalTrigConvEdge_x,
                ADC_ExternalTrigConv_x,
                ADC_DataAlign_x,
                ADC_ScanDirection_x);
    }

    static void initialize(FunctionalState continuousConvMode = DISABLE,
            uint32_t ADC_Resolution_x = ADC_Resolution_12b,
            uint32_t ADC_ExternalTrigConvEdge_x = ADC_ExternalTrigConvEdge_None,
            uint32_t ADC_ExternalTrigConv_x = ADC_ExternalTrigConv_T1_TRGO,
            uint32_t ADC_DataAlign_x = ADC_DataAlign_Right,
            uint32_t ADC_ScanDirection_x = ADC_ScanDirection_Upward)
    {
        //使能ADC时钟。
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADCx, ENABLE);
        //设置ADC时钟分频因子。ADC时钟ADCCLK大于14M会导致转换不准，
        //所以只能选择6分频和8分频：72M/6=12M, 72/8=9M
        // RCC_ADCCLKConfig(RCC_PCLK2_Div6);

        ADC_DeInit((ADC_TypeDef *)ADCx);

        ADC_InitTypeDef adc_init;
        adc_init.ADC_Resolution = ADC_Resolution_x;
        adc_init.ADC_ContinuousConvMode = continuousConvMode;
        adc_init.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_x;
        adc_init.ADC_ExternalTrigConv = ADC_ExternalTrigConv_x; //外部触发源。
        adc_init.ADC_DataAlign = ADC_DataAlign_x;               //数据对齐。
        adc_init.ADC_ScanDirection = ADC_ScanDirection_x;
        ADC_Init((ADC_TypeDef *)ADCx, &adc_init);

        ADC_GetCalibrationFactor((ADC_TypeDef *)ADCx);

        ADC_Cmd((ADC_TypeDef *)ADCx, ENABLE);

        while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN))
            ;
   }

    static void regularChannelConfig(uint32_t ADC_Channel_x,
            uint8_t ADC_SampleTime_x = ADC_SampleTime_239_5Cycles)
    {
            if(ADC_Channel_x <= ADC_Channel_7)
            {
                Sm_GPIOA::init_IO((uint16_t)ADC_Channel_x, GPIO_Mode_AN, GPIO_PuPd_NOPULL);
            }
            else if(ADC_Channel_x == ADC_Channel_8)
            {
                Sm_GPIOB::init_IO(GPIO_Pin_8, GPIO_Mode_AN, GPIO_PuPd_NOPULL);
            }
            else if(ADC_Channel_x == ADC_Channel_9)
            {
                Sm_GPIOB::init_IO(GPIO_Pin_9, GPIO_Mode_AN, GPIO_PuPd_NOPULL);
            }
            else if(ADC_Channel_x == ADC_Channel_14)
            {
                Sm_GPIOC::init_IO(GPIO_Pin_4, GPIO_Mode_AN, GPIO_PuPd_NOPULL);
            }
            else if(ADC_Channel_x == ADC_Channel_15)
            {
                Sm_GPIOC::init_IO(GPIO_Pin_5, GPIO_Mode_AN, GPIO_PuPd_NOPULL);
            }

        ADC_ChannelConfig((ADC_TypeDef *)ADCx, ADC_Channel_x, ADC_SampleTime_x);
    }

    static void softwareConvCmd(FunctionalState NewState)
    {
        Sm_assert(IS_FUNCTIONAL_STATE(NewState));

        if(NewState != DISABLE)
            ADC_StartOfConversion((ADC_TypeDef *)ADCx);
        else
            ADC_StopOfConversion((ADC_TypeDef*)ADCx);
    }

    //当只用一个ADC通道时，使用这个函数进行软件触发ADC转换，并读取转换值。
    static uint16_t softwareGetValue(void)
    {
        ADC_StartOfConversion((ADC_TypeDef *)ADCx);
        while(!ADC_GetFlagStatus((ADC_TypeDef *)ADCx, ADC_FLAG_EOC));
        return ADC_GetConversionValue((ADC_TypeDef *)ADCx);
    }

    static void regularDMAConfig(uint16_t *buff, uint16_t size)
    {
        Sm_assert(size >= 1 && size <= 16);

        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
        DMA_DeInit(DMA1_Channel1);

        DMA_InitTypeDef dma_init;
        dma_init.DMA_PeripheralBaseAddr = (uint32_t)&((ADC_TypeDef *)ADCx)->DR;
        dma_init.DMA_MemoryBaseAddr = (uint32_t)buff;
        dma_init.DMA_DIR = DMA_DIR_PeripheralSRC;
        dma_init.DMA_BufferSize = size;
        dma_init.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
        dma_init.DMA_MemoryInc = DMA_MemoryInc_Enable;
        dma_init.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
        dma_init.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
        dma_init.DMA_Mode = DMA_Mode_Circular;
        dma_init.DMA_Priority = DMA_Priority_High;
        dma_init.DMA_M2M = DMA_M2M_Disable;

        DMA_Init(DMA1_Channel1, &dma_init);
        DMA_Cmd(DMA1_Channel1, ENABLE);

        ADC_DMARequestModeConfig((ADC_TypeDef *)ADCx, ADC_DMAMode_Circular);
        ADC_DMACmd((ADC_TypeDef *)ADCx, ENABLE);
    }
};

#endif /* end of include guard: SM_ADC_H */
