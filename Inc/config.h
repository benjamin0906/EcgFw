/*
 * config.h
 *
 *  Created on: Oct 18, 2021
 *      Author: Benjamin
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#define ADAS_DRDY   PortA_3
#define ADAS_CS     PortA_4
#define ADAS_SCLK   PortA_5
#define ADAS_SDO    PortA_6
#define ADAS_SDI    PortA_7
#define ADAS_PD     PortC_4
#define ADAS_RESET  PortC_5
#define ADAS_CLKIO  PortB_1

#define MODULE_OFF  0
#define MODULE_ON   1

#define IRQ_MODE         0
#define BLOCKING_MODE    1
#define DMA_MODE         2

#define DATA_WIDTH_32_BIT   32
#define DATA_WIDTH_16_BIT   16
#define DATA_WIDTH_8_BIT    8

/* SPI config */

#define SPI_1                       MODULE_ON
#define SPI_2                       MODULE_OFF
#define SPI_3                       MODULE_ON
#define SPI_4                       MODULE_OFF

#if (SPI_1 == MODULE_ON) || (SPI_2 == MODULE_ON) || (SPI_3 == MODULE_ON) || (SPI_4 == MODULE_ON)
#define SPI_USAGE_MODE              DMA_MODE

/* SPI 1 config */
#if SPI_1 == MODULE_ON
#define SPI_1_CONFIG    {           \
        .Instance       = 1,        \
        .ChipSelectPin  = ADAS_CS,  \
        .DataSize       = 0,        \
        .LsbOrMsb       = 0,        \
        .CHPA           = 1,        \
        .CPOL           = 1,        \
        .ClockDiv       = 3}
#if SPI_USAGE_MODE == DMA_MODE
#define SPI_1_DMA_TX_BUFFER_WIDTH   DATA_WIDTH_32_BIT
#define SPI_1_DMA_TX_BUFFER_SIZE    8
#define SPI_1_DMA_RX_BUFFER_WIDTH   DATA_WIDTH_32_BIT
#define SPI_1_DMA_RX_BUFFER_SIZE    8
#define SPI_1_DMA_TX_INSTANCE          DMA2
#define SPI_1_DMA_TX_STREAM            Stream3
#define SPI_1_DMA_TX_REQ_CH            3
#define SPI_1_DMA_RX_INSTANCE          DMA2
#define SPI_1_DMA_RX_STREAM            Stream0
#define SPI_1_DMA_RX_REQ_CH            3
#else
#error "No DMA config is defined for SPI 1"
#endif
#endif

/* SPI 2 config */
#if SPI_2 == MODULE_ON
#define SPI_3_CONFIG    {           \
        .Instance       = 1,        \
        .ChipSelectPin  = ADAS_CS,  \
        .DataSize       = 0,        \
        .LsbOrMsb       = 0,        \
        .CHPA           = 1,        \
        .CPOL           = 1,        \
        .ClockDiv       = 3}
#endif

/* SPI 3 config */
#if SPI_3 == MODULE_ON
#define SPI_3_CONFIG    {           \
        .Instance       = 1,        \
        .ChipSelectPin  = ADAS_CS,  \
        .DataSize       = 0,        \
        .LsbOrMsb       = 0,        \
        .CHPA           = 1,        \
        .CPOL           = 1,        \
        .ClockDiv       = 3}
#if SPI_USAGE_MODE == DMA_MODE
#define SPI_3_DMA_TX_BUFFER_WIDTH   DATA_WIDTH_32_BIT
#define SPI_3_DMA_TX_BUFFER_SIZE    8
#define SPI_3_DMA_RX_BUFFER_WIDTH   DATA_WIDTH_32_BIT
#define SPI_3_DMA_RX_BUFFER_SIZE    8
#define SPI_3_DMA_TX_INSTANCE          DMA1
#define SPI_3_DMA_TX_STREAM            DmaStream_5
#define SPI_3_DMA_TX_REQ_CH            0
#define SPI_3_DMA_RX_INSTANCE          DMA1
#define SPI_3_DMA_RX_STREAM            DmaStream_0
#define SPI_3_DMA_RX_REQ_CH            0
#else
#error "No DMA config is defined for SPI 3"
#endif
#endif

/* SPI 4 config */
#if SPI_4 == MODULE_ON
#endif

#endif

#define FILTER_1_RAW_VALUE_DMA_INSTANCE   DMA1
#define FILTER_1_RAW_VALUE_DMA_STREAM     Stream4
#define FILTER_1_OUT_VALUE_DMA_INSTANCE   DMA1
#define FILTER_1_OUT_VALUE_DMA_STREAM     Stream5

#define SUPPLY_VOLTAGE      33
#define USB_RX_BUFFER_SIZE  127 //This needs to be less by 1 than a power of 2

#endif /* CONFIG_H_ */
