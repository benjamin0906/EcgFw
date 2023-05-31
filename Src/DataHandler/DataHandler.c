/*
 * DataHandler.c
 *
 *  Created on: 2023. ápr. 9.
 *      Author: Benjamin
 */

#include "adasMngr.h"
#include "Filters.h"
#include "DataHandler_Types.h"
#include "Com.h"

static dtEcgData adasRawData;
static dtEcgData FilteredData;
static dtDataHandlerFlags DataHandlerFlags;

void DataHandler_Runnable(void);
void DataHandler_BundleTriggers(void);

void DataHandler_BundleTriggers(void)
{
    static uint8 BaselineDivider = 0;
    if(adasMngr_GetReadData(&adasRawData) != 0)
    {
        DataHandlerFlags.unprocessedRawData = 1;
    }
    adasMngr_TriggerRead();
    BaselineDivider++;
    if(BaselineDivider == 20)
    {
        IFilters_TriggerNewProcess();
        IFilters_NewValue(0, adasRawData.Lead_I);
        IFilters_NewValue(1, adasRawData.Lead_II);
        IFilters_NewValue(2, adasRawData.Lead_III);
        IFilters_NewValue(3, adasRawData.Lead_V1);
        IFilters_NewValue(4, adasRawData.Lead_V2);
        BaselineDivider = 0;
    }
}

void DataHandler_Runnable(void)
{
    if(DataHandlerFlags.unprocessedRawData != 0)
    {
        FilteredData.Lead_I     = adasRawData.Lead_I - IFilters_GetOutput(0);
        FilteredData.Lead_II    = adasRawData.Lead_II - IFilters_GetOutput(1);
        FilteredData.Lead_III   = adasRawData.Lead_III - IFilters_GetOutput(2);
        FilteredData.Lead_V1    = adasRawData.Lead_V1 - IFilters_GetOutput(3);
        FilteredData.Lead_V2    = adasRawData.Lead_V2 - IFilters_GetOutput(4);
        DataHandlerFlags.unprocessedRawData = 0;
        ICom_NewEcgData(&FilteredData);
        DataHandlerFlags.unreadData = 1;
    }
}
