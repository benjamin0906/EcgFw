/*
 * Filters_Proc.h
 *
 *  Created on: 2023. febr. 12.
 *      Author: Benjamin
 */

#ifndef FILTERS_FILTERS_PROC_H_
#define FILTERS_FILTERS_PROC_H_

#include "types.h"

extern void IFilters_TriggerNewProcess(void);
extern void IFilters_NewValue(uint8 FilterId, int32 Value);

#endif /* FILTERS_FILTERS_PROC_H_ */
