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

#define DMA_DATA_WIDTH 32

/* Q15, 80dB, 100 Hz, 0.05..4 Hz */
const int32 FirWeightQ15_80dB_100Hz_0_05_4[129] = {
        0, 1, 1, 1, 2, 2, 3, 3, 4, 4,
        4, 4, 3, 2, 1, -1, -3, -7, -10, -15,
        -21, -27, -34, -41, -49, -57, -66, -74, -81, -88,
        -93, -97, -98, -97, -93, -85, -73, -57, -35, -9,
        22, 59, 102, 150, 203, 261, 324, 391, 462, 535,
        610, 686, 762, 838, 911, 981, 1047, 1109, 1164, 1212,
        1252, 1285, 1308, 1322, 1327, 1322, 1308, 1285, 1252, 1212,
        1164, 1109, 1047, 981, 911, 838, 762, 686, 610, 535,
        462, 391, 324, 261, 203, 150, 102, 59, 22, -9,
        -35, -57, -73, -85, -93, -97, -98, -97, -93, -88,
        -81, -74, -66, -57, -49, -41, -34, -27, -21, -15,
        -10, -7, -3, -1, 1, 2, 3, 4, 4, 4,
        4, 3, 3, 2, 2, 1, 1, 1, 0};

int32 RawSamples[2][sizeof(FirWeightQ15_80dB_100Hz_0_05_4)/sizeof(FirWeightQ15_80dB_100Hz_0_05_4[0])];
int32 FilterOutputs[2][sizeof(FirWeightQ15_80dB_100Hz_0_05_4)/sizeof(FirWeightQ15_80dB_100Hz_0_05_4[0])];
int32 RawSamples_2[2][sizeof(FirWeightQ15_80dB_100Hz_0_05_4)/sizeof(FirWeightQ15_80dB_100Hz_0_05_4[0])];
int32 FilterOutputs_2[2][sizeof(FirWeightQ15_80dB_100Hz_0_05_4)/sizeof(FirWeightQ15_80dB_100Hz_0_05_4[0])];
int32 RawSamples_3[2][sizeof(FirWeightQ15_80dB_100Hz_0_05_4)/sizeof(FirWeightQ15_80dB_100Hz_0_05_4[0])];
int32 FilterOutputs_3[2][sizeof(FirWeightQ15_80dB_100Hz_0_05_4)/sizeof(FirWeightQ15_80dB_100Hz_0_05_4[0])];
int32 RawSamples_4[2][sizeof(FirWeightQ15_80dB_100Hz_0_05_4)/sizeof(FirWeightQ15_80dB_100Hz_0_05_4[0])];
int32 FilterOutputs_4[2][sizeof(FirWeightQ15_80dB_100Hz_0_05_4)/sizeof(FirWeightQ15_80dB_100Hz_0_05_4[0])];
int32 RawSamples_5[2][sizeof(FirWeightQ15_80dB_100Hz_0_05_4)/sizeof(FirWeightQ15_80dB_100Hz_0_05_4[0])];
int32 FilterOutputs_5[2][sizeof(FirWeightQ15_80dB_100Hz_0_05_4)/sizeof(FirWeightQ15_80dB_100Hz_0_05_4[0])];
const dtDmaMapEntry DmaMap[] = {{FILTER_1_ASSIGNED_DMA_INSTANCE, FILTER_1_ASSIGNED_DMA_STREAM},
                            {FILTER_2_ASSIGNED_DMA_INSTANCE, FILTER_2_ASSIGNED_DMA_STREAM},
                            {FILTER_3_ASSIGNED_DMA_INSTANCE, FILTER_3_ASSIGNED_DMA_STREAM},
                            {FILTER_4_ASSIGNED_DMA_INSTANCE, FILTER_4_ASSIGNED_DMA_STREAM},
                            {FILTER_5_ASSIGNED_DMA_INSTANCE, FILTER_5_ASSIGNED_DMA_STREAM},
                            {FILTER_6_ASSIGNED_DMA_INSTANCE, FILTER_6_ASSIGNED_DMA_STREAM}};
dtFilterInfoForDma BufferInfoIdOnThisDma[6];
dtShitftJob ShiftJobs[15];

dtBufferInfo BufferInfo[] = {   {.State = BufferState_1isActive, .Buffer1 = &RawSamples[0][0],      .Buffer2 = &RawSamples[1][0],       .ElementSize = 4, .Size = sizeof(RawSamples[0])},
                                {.State = BufferState_1isActive, .Buffer1 = &FilterOutputs[0][0],   .Buffer2 = &FilterOutputs[1][0],    .ElementSize = 4, .Size = sizeof(FilterOutputs[0])},
                                {.State = BufferState_1isActive, .Buffer1 = &RawSamples_2[0][0],    .Buffer2 = &RawSamples_2[1][0],     .ElementSize = 4, .Size = sizeof(RawSamples_2[0])},
                                {.State = BufferState_1isActive, .Buffer1 = &FilterOutputs_2[0][0], .Buffer2 = &FilterOutputs_2[1][0],  .ElementSize = 4, .Size = sizeof(FilterOutputs_2[0])},
                                {.State = BufferState_1isActive, .Buffer1 = &RawSamples_3[0][0],    .Buffer2 = &RawSamples_3[1][0],     .ElementSize = 4, .Size = sizeof(RawSamples_3[0])},
                                {.State = BufferState_1isActive, .Buffer1 = &FilterOutputs_3[0][0], .Buffer2 = &FilterOutputs_3[1][0],  .ElementSize = 4, .Size = sizeof(FilterOutputs_3[0])},
                                {.State = BufferState_1isActive, .Buffer1 = &RawSamples_4[0][0],    .Buffer2 = &RawSamples_4[1][0],     .ElementSize = 4, .Size = sizeof(RawSamples_4[0])},
                                {.State = BufferState_1isActive, .Buffer1 = &FilterOutputs_4[0][0], .Buffer2 = &FilterOutputs_4[1][0],  .ElementSize = 4, .Size = sizeof(FilterOutputs_4[0])},
                                {.State = BufferState_1isActive, .Buffer1 = &RawSamples_5[0][0],    .Buffer2 = &RawSamples_5[1][0],     .ElementSize = 4, .Size = sizeof(RawSamples_5[0])},
                                {.State = BufferState_1isActive, .Buffer1 = &FilterOutputs_5[0][0], .Buffer2 = &FilterOutputs_5[1][0],  .ElementSize = 4, .Size = sizeof(FilterOutputs_5[0])}
};
dtFilterInfo FilterInfo[5];

#define FILTER_NUMBER sizeof(FilterInfo)/sizeof(dtFilterInfo)

void IFilters_Init(void);
void IFilters_TriggerNewProcess(void);
void IFilters_NewValue(uint8 FilterId, int32 Value);
void Filters_Runnable(void);
int32 IFilters_GetOutput(uint8 FilterId);
static void SetBufferToBeShifted(uint8 Id);
static dtAssignState AssignShiftingToDma(void);
void Dma1CallBack(uint8 Flags, uint32 Num);
void Dma2CallBack(uint8 Flags, uint32 Num);
void Dma3CallBack(uint8 Flags, uint32 Num);
void Dma4CallBack(uint8 Flags, uint32 Num);
void Dma5CallBack(uint8 Flags, uint32 Num);
void Dma6CallBack(uint8 Flags, uint32 Num);

static inline void TriggerNewProcessOnFilter(uint8 Id);

void IFilters_Init()
{
    void (*DmaCallBacks[6])(uint8 Flags, uint32 Num) = {Dma1CallBack, Dma2CallBack, Dma3CallBack, Dma4CallBack, Dma5CallBack, Dma6CallBack};
    dtDmaConfig DmaCfg;
    DmaCfg.Mem0Ptr              = 0;
    DmaCfg.Mem1Ptr              = 0;
    DmaCfg.PerPtr               = 0;
    DmaCfg.Priority             = 0;//very high
#if DMA_DATA_WIDTH == 32
    DmaCfg.MemoryDataSize       = 2;//byte
    DmaCfg.PeripheralDataSize   = 2;//byte
#endif
    DmaCfg.MemAddrInc           = 1;
    DmaCfg.PerAddrInc           = 1;
    DmaCfg.CircularMode         = 0;
    DmaCfg.TransferDirection    = 2;
    DmaCfg.RequestChannel       = 0;
    uint16 i;
    for(i = 0; i < (sizeof(DmaMap)/sizeof(DmaMap[0])); i++)
    {
        DmaCfg.Instance             = DmaMap[i].Instance;
        DmaCfg.Stream               = DmaMap[i].Stream;
        IDMA_Config(&DmaCfg, DmaCallBacks[i]);
    }
    for(i = 0; i < (sizeof(RawSamples[0])/sizeof(RawSamples[0][0])); i++)
    {
        RawSamples[0][i] = i;
        RawSamples[1][i] = i;
        FilterOutputs[0][i] = i;
        FilterOutputs[1][i] = i;
    }

    FilterInfo[0].RawBuffers[0] = RawSamples[0];
    FilterInfo[0].RawBuffers[1] = RawSamples[1];
    FilterInfo[0].OutBuffers[0] = FilterOutputs[0];
    FilterInfo[0].OutBuffers[1] = FilterOutputs[1];
    FilterInfo[0].Size          = sizeof(RawSamples[0])/sizeof(RawSamples[0][0]);
    FilterInfo[0].ElementSizeCode   = 2;

    FilterInfo[1].RawBuffers[0] = RawSamples_2[0];
    FilterInfo[1].RawBuffers[1] = RawSamples_2[1];
    FilterInfo[1].OutBuffers[0] = FilterOutputs_2[0];
    FilterInfo[1].OutBuffers[1] = FilterOutputs_2[1];
    FilterInfo[1].Size          = sizeof(RawSamples_2[0])/sizeof(RawSamples_2[0][0]);
    FilterInfo[1].ElementSizeCode   = 2;

    FilterInfo[2].RawBuffers[0] = RawSamples_3[0];
    FilterInfo[2].RawBuffers[1] = RawSamples_3[1];
    FilterInfo[2].OutBuffers[0] = FilterOutputs_3[0];
    FilterInfo[2].OutBuffers[1] = FilterOutputs_3[1];
    FilterInfo[2].Size          = sizeof(RawSamples_3[0])/sizeof(RawSamples_3[0][0]);
    FilterInfo[2].ElementSizeCode   = 2;

    FilterInfo[3].RawBuffers[0] = RawSamples_4[0];
    FilterInfo[3].RawBuffers[1] = RawSamples_4[1];
    FilterInfo[3].OutBuffers[0] = FilterOutputs_4[0];
    FilterInfo[3].OutBuffers[1] = FilterOutputs_4[1];
    FilterInfo[3].Size          = sizeof(RawSamples_4[0])/sizeof(RawSamples_4[0][0]);
    FilterInfo[3].ElementSizeCode   = 2;

    FilterInfo[4].RawBuffers[0] = RawSamples_5[0];
    FilterInfo[4].RawBuffers[1] = RawSamples_5[1];
    FilterInfo[4].OutBuffers[0] = FilterOutputs_5[0];
    FilterInfo[4].OutBuffers[1] = FilterOutputs_5[1];
    FilterInfo[4].Size          = sizeof(RawSamples_5[0])/sizeof(RawSamples_5[0][0]);
    FilterInfo[4].ElementSizeCode   = 2;
}

static inline void TriggerNewProcessOnFilter(uint8 Id)
{
    FilterInfo[Id].Flags.Flags.FilterCalcActive = 1;
    FilterInfo[Id].Flags.Flags.OutBufShiftDone = 0;
    FilterInfo[Id].Flags.Flags.RawBufShiftDone = 0;
    FilterInfo[Id].Flags.Flags.NewCalcDone = 0;
    FilterInfo[Id].Flags.Flags.NewValueReady = 0;
#if DMA_DATA_WIDTH == 32
    FilterInfo[Id].OutValue = FilterInfo[Id].OutBuffers[FilterInfo[Id].Flags.Flags.OutActiveBlockId][FilterInfo[Id].Size - 1];
#elif DMA_DATA_WIDTH == 8
    FilterInfo[Id].OutValue = FilterInfo[Id].OutBuffers[FilterInfo[Id].Flags.Flags.OutActiveBlockId][(FilterInfo[Id].Size>>FilterInfo[Id].ElementSizeCode) - 1];
#endif
    SetBufferToBeShifted(Id);
}

void Filters_Runnable(void)
{
    AssignShiftingToDma();
    uint8 FilterInfoLooper;
    for(FilterInfoLooper = 0; FilterInfoLooper < (sizeof(FilterInfo)/sizeof(FilterInfo[0])); FilterInfoLooper++)
    {
        if(FilterInfo[FilterInfoLooper].Flags.Flags.FilterCalcActive != 0)
        {
            /* Check if Raw shifting is done */
            if(FilterInfo[FilterInfoLooper].Flags.Flags.RawBufShiftDone != 0)
            {
                if(FilterInfo[FilterInfoLooper].Flags.Flags.NewCalcDone == 0)
                {
                    /* Not yet calculated new output value */
                    if(FilterInfo[FilterInfoLooper].Flags.Flags.NewValueReady != 0)
                    {
                        FilterInfo[FilterInfoLooper].RawBuffers[FilterInfo[FilterInfoLooper].Flags.Flags.RawActiveBlockId][0] = FilterInfo[FilterInfoLooper].NewValue;
                        int64 t = multiplyArrays(&FilterInfo[FilterInfoLooper].RawBuffers[FilterInfo[FilterInfoLooper].Flags.Flags.RawActiveBlockId][0], FirWeightQ15_80dB_100Hz_0_05_4, sizeof(FirWeightQ15_80dB_100Hz_0_05_4)/sizeof(FirWeightQ15_80dB_100Hz_0_05_4[0]));

                        //RawSamples[FilterInfo[FilterInfoLooper].Flags.Flags.RawActiveBlockId][0] = FilterInfo[FilterInfoLooper].NewValue;
                        //int64 t = multiplyArrays(&RawSamples[FilterInfo[FilterInfoLooper].Flags.Flags.RawActiveBlockId][0], FirWeightQ15_80dB_100Hz_0_05_4, sizeof(FirWeightQ15_80dB_100Hz_0_05_4)/sizeof(FirWeightQ15_80dB_100Hz_0_05_4[0]));
                        FilterInfo[FilterInfoLooper].NewOutValue = (int32)(t >> 15);
                        //FilterInfo[FilterInfoLooper].NewOutValue >>= 15;
                        FilterInfo[FilterInfoLooper].Flags.Flags.NewCalcDone = 1;
                    }
                }

                if(FilterInfo[FilterInfoLooper].Flags.Flags.OutBufShiftDone != 0)
                {
                    if(FilterInfo[FilterInfoLooper].Flags.Flags.NewCalcDone != 0)
                    {
                        /* Output buffer shifting is done the new output value van be added to it */
                        FilterInfo[FilterInfoLooper].OutBuffers[FilterInfo[FilterInfoLooper].Flags.Flags.OutActiveBlockId][0] = FilterInfo[FilterInfoLooper].NewOutValue;
                        //FilterOutputs[FilterInfo[FilterInfoLooper].Flags.Flags.OutActiveBlockId][0] = FilterInfo[FilterInfoLooper].NewOutValue;

                        /* Deactivate calculation process */
                        FilterInfo[FilterInfoLooper].Flags.Flags.FilterCalcActive = 0;

                        ClearTestGpio(FilterInfoLooper);
                    }
                }
            }
        }
    }
}

static void SetBufferToBeShifted(uint8 Id)
{
    uint8 looper = 0;
    while((looper < 15) && (ShiftJobs[looper].size != 0))
    {
        looper ++;
    }
    if(looper < 15)
    {
        ShiftJobs[looper].SrcPtr = FilterInfo[Id].RawBuffers[FilterInfo[Id].Flags.Flags.RawActiveBlockId];
#if DMA_DATA_WIDTH == 32
        ShiftJobs[looper].size = FilterInfo[Id].Size - 1;
#elif DMA_DATA_WIDTH == 8
        ShiftJobs[looper].size = FilterInfo[Id].Size - (1 << FilterInfo[Id].ElementSizeCode);
#endif
        if(FilterInfo[Id].Flags.Flags.RawActiveBlockId != 0)
        {
            ShiftJobs[looper].DstPtr = FilterInfo[Id].RawBuffers[0];
            FilterInfo[Id].Flags.Flags.RawActiveBlockId = 0;
        }
        else
        {
            ShiftJobs[looper].DstPtr = FilterInfo[Id].RawBuffers[1];
            FilterInfo[Id].Flags.Flags.RawActiveBlockId = 1;
        }
        ShiftJobs[looper].DstPtr += (1 << FilterInfo[Id].ElementSizeCode);
        ShiftJobs[looper].InfoForDma.FilterIndex = Id;
        ShiftJobs[looper].InfoForDma.RawOrOutBuff = 1;

        looper ++;

        ShiftJobs[looper].SrcPtr = FilterInfo[Id].OutBuffers[FilterInfo[Id].Flags.Flags.OutActiveBlockId];
#if DMA_DATA_WIDTH == 32
        ShiftJobs[looper].size = FilterInfo[Id].Size - 1;
#elif DMA_DATA_WIDTH == 8
        ShiftJobs[looper].size = FilterInfo[Id].Size - (1 << FilterInfo[Id].ElementSizeCode);
#endif
        if(FilterInfo[Id].Flags.Flags.OutActiveBlockId != 0)
        {
            ShiftJobs[looper].DstPtr = FilterInfo[Id].OutBuffers[0];
            FilterInfo[Id].Flags.Flags.OutActiveBlockId = 0;
        }
        else
        {
            ShiftJobs[looper].DstPtr = FilterInfo[Id].OutBuffers[1];
            FilterInfo[Id].Flags.Flags.OutActiveBlockId = 1;
        }
        ShiftJobs[looper].DstPtr += (1 << FilterInfo[Id].ElementSizeCode);
        ShiftJobs[looper].InfoForDma.FilterIndex = Id;
        ShiftJobs[looper].InfoForDma.RawOrOutBuff = 0;
    }
}


static dtAssignState AssignShiftingToDma(void)
{
    dtAssignState ret = AssignState_NothingToShift;
    uint8 DmaLooper = 0;
    uint8 JobLooper = 0;
    while((DmaLooper < 6) && (JobLooper < 15))
    {
        if(IDMA_IsFree(DmaMap[DmaLooper].Instance, DmaMap[DmaLooper].Stream) != 0)
        {
            while((JobLooper < 15) && (ShiftJobs[JobLooper].size == 0))
            {
               JobLooper ++;
            }
            if(JobLooper < 15)
            {
                BufferInfoIdOnThisDma[DmaLooper] = ShiftJobs[JobLooper].InfoForDma;
                DMA_StartWithNew(DmaMap[DmaLooper].Instance, DmaMap[DmaLooper].Stream, ShiftJobs[JobLooper].size, ShiftJobs[JobLooper].SrcPtr, ShiftJobs[JobLooper].DstPtr);

                /* Job is started it can be cleared */
                ShiftJobs[JobLooper].size = 0;
            }
        }
        DmaLooper++;
    }
    if(DmaLooper == 6) ret = AssignState_WaitingForDma;


    return ret;
}

void IFilters_TriggerNewProcess(void)
{
    SetAllTestGpio();
    TriggerNewProcessOnFilter(0);
    TriggerNewProcessOnFilter(1);
    TriggerNewProcessOnFilter(2);
    TriggerNewProcessOnFilter(3);
    TriggerNewProcessOnFilter(4);
    AssignShiftingToDma();
}

void IFilters_NewValue(uint8 FilterId, int32 Value)
{
    FilterInfo[FilterId].NewValue = Value;
    FilterInfo[FilterId].Flags.Flags.NewValueReady = 1;
}

int32 IFilters_GetOutput(uint8 FilterId)
{
    if(FilterId < FILTER_NUMBER)
    {
        return FilterInfo[FilterId].OutValue;
    }
    else
    {
        return 0;
    }
}

void Dma1CallBack(uint8 Flags, uint32 Num)
{
    if(BufferInfoIdOnThisDma[0].RawOrOutBuff != 0)
    {
        FilterInfo[BufferInfoIdOnThisDma[0].FilterIndex].Flags.Flags.RawBufShiftDone = 1;
    }
    else
    {
        FilterInfo[BufferInfoIdOnThisDma[0].FilterIndex].Flags.Flags.OutBufShiftDone = 1;
    }
}

void Dma2CallBack(uint8 Flags, uint32 Num)
{
    if(BufferInfoIdOnThisDma[1].RawOrOutBuff != 0)
    {
        FilterInfo[BufferInfoIdOnThisDma[1].FilterIndex].Flags.Flags.RawBufShiftDone = 1;
    }
    else
    {
        FilterInfo[BufferInfoIdOnThisDma[1].FilterIndex].Flags.Flags.OutBufShiftDone = 1;
    }
}

void Dma3CallBack(uint8 Flags, uint32 Num)
{
    if(BufferInfoIdOnThisDma[2].RawOrOutBuff != 0)
    {
        FilterInfo[BufferInfoIdOnThisDma[2].FilterIndex].Flags.Flags.RawBufShiftDone = 1;
    }
    else
    {
        FilterInfo[BufferInfoIdOnThisDma[2].FilterIndex].Flags.Flags.OutBufShiftDone = 1;
    }
}

void Dma4CallBack(uint8 Flags, uint32 Num)
{
    if(BufferInfoIdOnThisDma[3].RawOrOutBuff != 0)
    {
        FilterInfo[BufferInfoIdOnThisDma[3].FilterIndex].Flags.Flags.RawBufShiftDone = 1;
    }
    else
    {
        FilterInfo[BufferInfoIdOnThisDma[3].FilterIndex].Flags.Flags.OutBufShiftDone = 1;
    }
}

void Dma5CallBack(uint8 Flags, uint32 Num)
{
    if(BufferInfoIdOnThisDma[4].RawOrOutBuff != 0)
    {
        FilterInfo[BufferInfoIdOnThisDma[4].FilterIndex].Flags.Flags.RawBufShiftDone = 1;
    }
    else
    {
        FilterInfo[BufferInfoIdOnThisDma[4].FilterIndex].Flags.Flags.OutBufShiftDone = 1;
    }
}

void Dma6CallBack(uint8 Flags, uint32 Num)
{
    if(BufferInfoIdOnThisDma[5].RawOrOutBuff != 0)
    {
        FilterInfo[BufferInfoIdOnThisDma[5].FilterIndex].Flags.Flags.RawBufShiftDone = 1;
    }
    else
    {
        FilterInfo[BufferInfoIdOnThisDma[5].FilterIndex].Flags.Flags.OutBufShiftDone = 1;
    }
}
