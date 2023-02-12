/*
 * Hal_Internal.h
 *
 *  Created on: 2023. febr. 10.
 *      Author: Benjamin
 */

#ifndef HAL_INTERNAL_H_
#define HAL_INTERNAL_H_

#include "types.h"

#define CREATE_IRQ_NAME(first, sec) IRQ_##first##_##sec
#define DMA_IRQ(x, y) CREATE_IRQ_NAME(x,y)

#endif /* HAL_INTERNAL_H_ */
