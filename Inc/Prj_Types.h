/*
 * Prj_Types.h
 *
 *  Created on: 2023. ápr. 28.
 *      Author: Benjamin
 */

#ifndef PRJ_TYPES_H_
#define PRJ_TYPES_H_

#include "types.h"

typedef struct
{
    int32 Lead_I;
    int32 Lead_II;
    int32 Lead_III;
    int32 Lead_V1;
    int32 Lead_V2;
} dtEcgData;

#define ECG_DATA_TYPESIZE_BYTE  sizeof(dtEcgData)
#define ECG_DATA_TYPESIZE_WORD  ECG_DATA_TYPESIZE_BYTE/4

#endif /* PRJ_TYPES_H_ */
