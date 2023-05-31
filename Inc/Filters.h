/*
 * Filters.h
 *
 *  Created on: 2023. febr. 10.
 *      Author: Benjamin
 */

#ifndef FILTERS_H_
#define FILTERS_H_

#include "types.h"

extern void IFilters_Init(void);
extern void IFilters_TriggerNewProcess(void);
extern void IFilters_NewValue(uint8 FilterId, int32 Value);
extern int32 IFilters_GetOutput(uint8 FilterId);

#endif /* FILTERS_H_ */
