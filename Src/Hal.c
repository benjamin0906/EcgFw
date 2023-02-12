/*
 * Hal.c
 *
 *  Created on: 2022. márc. 15.
 *      Author: Benjamin
 */

#ifndef HAL_C_
#define HAL_C_

#include "RCC.h"
#include "SPI.h"
#include "SysTick.h"
#include "usb_device.h"
#include "config.h"
#include "TIM3.h"
#include "NVIC.h"
#include "EXTI.h"
#include "SysCfg.h"
#include "adasMngr.h"
#include "Hal_Internal.h"
#include "main.h"

void halInit(void);

void testGpio(void)
{
    static int a = 0;
    if(a == 0)
    {
        a = 1;
        GPIO_Set(PortB_7, Clear);
    }
    else
    {
        a = 0;
        GPIO_Set(PortB_7, Set);
    }
}

void halInit(void)
{
    dtRccInitConfig RCCConf = { .Clock = 160000000,
                                .AHB_Presc = AHB_Presc1,
                                .APB1_Presc = APB_Presc4,
                                .APB2_Presc = APB_Presc2,
                                .PLL_QDiv = QDiv_6,
                                .CrystalOrInternal = Crystal,
                                .CrystalClockFreq = 16000000,
                                .PLL_SAI_Conf = 0x0F000C08,
                                .PLL_SAI_EN = 1
    };

    dtGPIOConfig OutputConfig = {.Type = PushPull, .Speed  = VeryHigh, .PUPD = NoPull, .Mode = Output};
    dtGPIOConfig AdasClk = {.Type = PushPull, .Speed  = VeryHigh, .PUPD = NoPull, .Mode = Alt2};
    dtGPIOConfig SpiConfig = {.Type = PushPull, .Speed  = VeryHigh, .PUPD = NoPull, .Mode = Alt5};
    dtGPIOConfig USBConf = {.Type = PushPull, .Speed  = VeryHigh, .PUPD = NoPull, .Mode = Alt10};
    dtGPIOConfig InputConfig = {.Type = PushPull, .Speed  = Low, .PUPD = NoPull, .Mode = Input};

    RCC_ClockEnable(RCC_GPIOA, Enable);
    RCC_ClockEnable(RCC_GPIOB, Enable);
    RCC_ClockEnable(RCC_GPIOC, Enable);
    RCC_ClockEnable(RCC_PWR, Enable);
    RCC_ClockEnable(RCC_OTGFS, Enable);
    RCC_ClockEnable(RCC_TIM3, Enable);
    RCC_ClockEnable(RCC_SPI1, Enable);
    RCC_ClockEnable(RCC_SPI3, Enable);
    RCC_ClockEnable(RCC_DMA1, Enable);
    RCC_ClockEnable(RCC_DMA2, Enable);
    RCC_ClockEnable(RCC_SYSCFG, Enable);

    RCC_ClockSet(RCCConf);

    SysTick_Init(160000);

    GPIO_PinInit(PortA_15, OutputConfig);
    GPIO_PinInit(PortB_7, OutputConfig);
    GPIO_PinInit(PortA_9, InputConfig);
    GPIO_PinInit(PortA_11, USBConf);
    GPIO_PinInit(PortA_12, USBConf);
    GPIO_PinInit(ADAS_DRDY, InputConfig);
    GPIO_PinInit(ADAS_CS, OutputConfig);
    GPIO_PinInit(ADAS_RESET, OutputConfig);
    GPIO_PinInit(ADAS_PD, OutputConfig);
    GPIO_PinInit(ADAS_SCLK, SpiConfig);
    GPIO_PinInit(ADAS_SDI, SpiConfig);
    GPIO_PinInit(ADAS_SDO, SpiConfig);
    GPIO_PinInit(ADAS_CLKIO, AdasClk);

    MX_USB_DEVICE_Init();

    dtSpiConf SpiConf = SPI_1_CONFIG;
    ISPI_Init(SpiConf);
    NVIC_EnableIRQ(DMA_IRQ(SPI_1_DMA_RX_INSTANCE,SPI_1_DMA_RX_STREAM));

    NVIC_EnableIRQ(DMA_IRQ(FILTER_1_RAW_VALUE_DMA_INSTANCE, FILTER_1_RAW_VALUE_DMA_STREAM));
    NVIC_EnableIRQ(DMA_IRQ(FILTER_1_OUT_VALUE_DMA_INSTANCE, FILTER_1_OUT_VALUE_DMA_STREAM));
    IFilters_Init();

    dtGPT1Config TIM3Conf = {.ARR = 18000, .Direction = 1, .Mode = Oc4, .OCMode = OcToggle, .OcPolarity = ActiveHigh, .Presc = 0};
    TIM3Conf.ARR = (RCC_GetClock(APB1_Timer)/8000000/2)-1;
    TIM3_Init(TIM3Conf);

    dtExtiConfig ExtiCfg = {.EventMask = 0, .InterruptMask = 1, .FallingEdgeTrigger = 1, .RisingEdgeTrigger = 0};
    ISysCfg_SetExti(3,ExtiPort_A);
    IExti_Config(3, ExtiCfg, BundleAdasTriggers);
    NVIC_EnableIRQ(IRQ_EXTI3);
}

#endif /* HAL_C_ */
