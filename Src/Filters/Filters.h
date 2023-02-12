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

#endif /* FILTERS_FILTERS_H_ */
