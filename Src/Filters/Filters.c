/*
 * Filters.c
 *
 *  Created on: 2023. febr. 4.
 *      Author: Benjamin
 */

#include "types.h"
#include "DMA.h"
#include "Utilities.h"
#include "Filters.h"
#include "Config.h"

extern void TestGpioSet(void);
extern void TestGpioClear(void);

/* Q15, 80dB, 100 Hz, 0.05..4 Hz */
const int32 FirWeightQ15_80dB_100Hz_0_05_4[129] = {
            2, 2, 3, 4, 5, 6, 7, 7, 8, 8,
            8, 7, 6, 4, 2, -1, -6, -11, -17, -24,
            -32, -40, -49, -59, -69, -79, -89, -98, -106, -113,
            -118, -121, -121, -118, -111, -100, -85, -65, -40, -10,
            25, 66, 112, 163, 220, 280, 345, 414, 485, 558,
            633, 709, 784, 857, 929, 997, 1060, 1119, 1172, 1218,
            1256, 1287, 1309, 1323, 1327, 1323, 1309, 1287, 1256, 1218,
            1172, 1119, 1060, 997, 929, 857, 784, 709, 633, 558,
            485, 414, 345, 280, 220, 163, 112, 66, 25, -10,
            -40, -65, -85, -100, -111, -118, -121, -121, -118, -113,
            -106, -98, -89, -79, -69, -59, -49, -40, -32, -24,
            -17, -11, -6, -1, 2, 4, 6, 7, 8, 8,
            8, 7, 7, 6, 5, 4, 3, 2, 2
        };

int32 RawSamples[2][sizeof(FirWeightQ15_80dB_100Hz_0_05_4)/sizeof(FirWeightQ15_80dB_100Hz_0_05_4[0])];
int32 FilterOutputs[2][sizeof(FirWeightQ15_80dB_100Hz_0_05_4)/sizeof(FirWeightQ15_80dB_100Hz_0_05_4[0])];
int32 OutputValue;
int32 NewRawValue;
uint8 BufferFlags;
dtBufferState BufferState;
dtFilterState FilterState;

void IFilters_Init(void);
void IFilters_TriggerNewProcess(void);
void IFilters_NewValue(int32 Value);
void Filters_Runnable(void);
void Filters_RawShiftDoneCallback(uint8 Flags, uint32 Num);
void Filters_OutShiftDoneCallback(uint8 Flags, uint32 Num);

void IFilters_Init()
{
    dtDmaConfig DmaCfg;
    DmaCfg.Mem0Ptr              = &RawSamples[1][0];
    DmaCfg.Mem1Ptr              = 0;
    DmaCfg.PerPtr               = &RawSamples[0][0];
    DmaCfg.Priority             = 0;//very high
    DmaCfg.MemoryDataSize       = 2;//byte
    DmaCfg.PeripheralDataSize   = 2;//byte
    DmaCfg.MemAddrInc           = 1;
    DmaCfg.PerAddrInc           = 1;
    DmaCfg.CircularMode         = 0;
    DmaCfg.TransferDirection    = 2;
    DmaCfg.Instance             = FILTER_1_RAW_VALUE_DMA_INSTANCE;
    DmaCfg.Stream               = FILTER_1_RAW_VALUE_DMA_STREAM;
    DmaCfg.RequestChannel       = 0;
    IDMA_Config(&DmaCfg, Filters_RawShiftDoneCallback);


    DmaCfg.Mem0Ptr              = &FilterOutputs[1][0];
    DmaCfg.PerPtr               = &FilterOutputs[0][0];
    DmaCfg.Instance             = FILTER_1_OUT_VALUE_DMA_INSTANCE;
    DmaCfg.Stream               = FILTER_1_OUT_VALUE_DMA_STREAM;
    IDMA_Config(&DmaCfg, Filters_OutShiftDoneCallback);

    memset_32bit(RawSamples, 0, sizeof(FirWeightQ15_80dB_100Hz_0_05_4)/sizeof(FirWeightQ15_80dB_100Hz_0_05_4[0]));
    memset_32bit(FilterOutputs, 0, sizeof(FirWeightQ15_80dB_100Hz_0_05_4)/sizeof(FirWeightQ15_80dB_100Hz_0_05_4[0]));

    uint8 i;
    for(i = 0; i < 20; i++)
    {
        IFilters_TriggerNewProcess();
        Filters_Runnable();
        Filters_Runnable();
        IFilters_NewValue(1);
        Filters_Runnable();
        Filters_Runnable();
    }
}

void Filters_Runnable(void)
{
    static int32 tempOutValue = 0;
    switch(FilterState)
    {
        case FilterState_Ready:
            break;
        case FilterState_RawShiftWait:
            if(FLAG_GET(BufferFlags, FLAG_RAW_BUFFER_READY) == 0)
            {
                break;
            }
        case FilterState_Calculating:
            if(FLAG_GET(BufferFlags, FLAG_NEW_RAW_VALUE) != 0)
            {
                RawSamples[BufferState][0] = NewRawValue;
                tempOutValue = multiplyArrays(&RawSamples[BufferState][0], FirWeightQ15_80dB_100Hz_0_05_4, sizeof(FirWeightQ15_80dB_100Hz_0_05_4)/sizeof(FirWeightQ15_80dB_100Hz_0_05_4[0]));
                tempOutValue >>= 15;
                FilterState = FilterState_OutShiftWait;
            }
            else
            {
                break;
            }
        case FilterState_OutShiftWait:
            if(FLAG_GET(BufferFlags, FLAG_OUT_BUFFER_READY) != 0)
            {
                FilterOutputs[BufferState][0] = tempOutValue;
                FilterState = FilterState_Ready;
                TestGpioClear();
            }
    }
}

void Filters_RawShiftDoneCallback(uint8 Flags, uint32 Num)
{
    FLAG_SET(BufferFlags, FLAG_RAW_BUFFER_READY);
}

void Filters_OutShiftDoneCallback(uint8 Flags, uint32 Num)
{
    FLAG_SET(BufferFlags, FLAG_OUT_BUFFER_READY);
}

void IFilters_TriggerNewProcess(void)
{
    TestGpioSet();
    FilterState = FilterState_RawShiftWait;
    FLAG_CLEAR(BufferFlags, FLAG_RAW_BUFFER_READY);
    FLAG_CLEAR(BufferFlags, FLAG_OUT_BUFFER_READY);
    FLAG_CLEAR(BufferFlags, FLAG_NEW_RAW_VALUE);
    if(BufferState == BufferState_Buffer0)
    {
        OutputValue = FilterOutputs[0][sizeof(FilterOutputs[0])/sizeof(FilterOutputs[0][0])-1];
        BufferState = BufferState_Buffer1;
        DMA_StartWithNew(FILTER_1_RAW_VALUE_DMA_INSTANCE, FILTER_1_RAW_VALUE_DMA_STREAM, (sizeof(RawSamples[0])/sizeof(RawSamples[0][0]))-1, &RawSamples[0][0], &RawSamples[1][1]);
        DMA_StartWithNew(FILTER_1_OUT_VALUE_DMA_INSTANCE, FILTER_1_OUT_VALUE_DMA_STREAM, (sizeof(FilterOutputs[0])/sizeof(FilterOutputs[0][0]))-1, &FilterOutputs[0][0], &FilterOutputs[1][1]);
    }
    else
    {
        OutputValue = FilterOutputs[1][sizeof(FilterOutputs[0])/sizeof(FilterOutputs[0][0])-1];
        BufferState = BufferState_Buffer0;
        DMA_StartWithNew(FILTER_1_RAW_VALUE_DMA_INSTANCE, FILTER_1_RAW_VALUE_DMA_STREAM, (sizeof(RawSamples[1])/sizeof(RawSamples[1][0]))-1, &RawSamples[1][0], &RawSamples[0][1]);
        DMA_StartWithNew(FILTER_1_OUT_VALUE_DMA_INSTANCE, FILTER_1_OUT_VALUE_DMA_STREAM, (sizeof(FilterOutputs[1])/sizeof(FilterOutputs[1][0]))-1, &FilterOutputs[1][0], &FilterOutputs[0][1]);
    }
}

void IFilters_NewValue(int32 Value)
{
    NewRawValue = Value;
    FLAG_SET(BufferFlags, FLAG_NEW_RAW_VALUE);
}
