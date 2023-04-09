/*
 * Filters.h
 *
 *  Created on: 2023. febr. 12.
 *      Author: Benjamin
 */

#ifndef FILTERS_FILTERS_H_
#define FILTERS_FILTERS_H_

#include "types.h"

#define FLAG_RAW_BUFFER_READY   1
#define FLAG_OUT_BUFFER_READY   2
#define FLAG_NEW_RAW_VALUE      4

#define FLAG_CLEAR(var, flag) var &= ~(flag)
#define FLAG_SET(var, flag) (var |= flag)
#define FLAG_GET(var, flag) (var & flag)

typedef enum
{
    BufferState_Buffer0 = 0,
    BufferState_Buffer1 = 1
} dtBufferState;

typedef enum
{
    FilterState_Ready,
    FilterState_RawShiftWait,
    FilterState_Calculating,
    FilterState_OutShiftWait,
} dtFilterState;

typedef enum
{
    BufferState_1isActive,
    BufferState_ShiftingTo2,
    BufferState_2isActive,
    BufferState_ShiftingTo1,
} dtBuffState;

typedef struct
{
    dtBuffState State;
    uint8 ElementSize;
    uint16 Size;
    void *Buffer1;
    void *Buffer2;
} dtBufferInfo;

typedef struct
{
    dtDMAInstance Instance;
    dtDmaStream Stream;
} dtDmaMapEntry;

typedef union
{
    uint8 Byte;
    struct
    {
        uint8 FilterCalcActive  :1;
        uint8 NewValueReady     :1;
        uint8 NewCalcDone       :1;
        uint8 RawBufShiftDone   :1;
        uint8 OutBufShiftDone   :1;
        uint8 RawActiveBlockId  :1;
        uint8 OutActiveBlockId  :1;
    } Flags;
} dtFilterFlags;

typedef struct
{
    dtFilterFlags   Flags;
    int32           NewValue;
    int32           NewOutValue;
    int32           OutValue;
    int32           *RawBuffers[2];
    int32           *OutBuffers[2];
    uint16          Size;

    /* 0: 8 bits, 1: 16 bits, 2: 32 bits*/
    uint8           ElementSizeCode;
} dtFilterInfo;

typedef struct
{
    uint8 RawOrOutBuff  :1;
    uint8 FilterIndex   :7;
} dtFilterInfoForDma;

typedef struct
{
    dtFilterInfoForDma  InfoForDma;
    uint16              size;
    void                *SrcPtr;
    void                *DstPtr;
} dtShitftJob;

typedef enum
{
    AssignState_NothingToShift,
    AssignState_WaitingForDma,
} dtAssignState;

int32 IFilters_GetOutput(uint8 FilterId);

#endif /* FILTERS_FILTERS_H_ */
