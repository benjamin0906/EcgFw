/*
 * Filters.h
 *
 *  Created on: 2023. febr. 10.
 *      Author: Benjamin
 */

#ifndef FILTERS_H_
#define FILTERS_H_

#include "types.h"

void IFilters_Init(void);
void IFilters_TriggerNewProcess(void);
void IFilters_CalcNew(int32 newValue);

#endif /* FILTERS_H_ */
