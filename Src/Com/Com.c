/*
 * Com.c
 *
 *  Created on: 2023. ápr. 25.
 *      Author: Benjamin
 */

#include "Com_Types.h"
#include "Utilities.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"

#define ECG_DATA_BUFFE_SIZE 8

static dtEcgData OutData[ECG_DATA_BUFFE_SIZE];
static uint8 WrIndex;
static uint8 RdIndex;
static uint8 Size;
static uint32 BuildBuffer[16];
static uint8 BuildBufferSize;
static uint32 OutBuffer[16];

void Com_Runnable(void);
void ICom_NewEcgData(dtEcgData *NewData);
static inline Std_ReturnType GetFromFifo(dtEcgData *data);

void Com_Runnable(void)
{
    dtEcgData currentData;
    if(GetFromFifo(&currentData) == E_OK)
    {
        BuildBuffer[BuildBufferSize + 0] = (OutData[RdIndex].Lead_I & 0xFFFFFF)   + 0x11000000;
        BuildBuffer[BuildBufferSize + 1] = (OutData[RdIndex].Lead_II & 0xFFFFFF)  + 0x12000000;
        BuildBuffer[BuildBufferSize + 2] = (OutData[RdIndex].Lead_III & 0xFFFFFF) + 0x13000000;
        BuildBuffer[BuildBufferSize + 3] = (OutData[RdIndex].Lead_V1 & 0xFFFFFF)  + 0x14000000;
        BuildBuffer[BuildBufferSize + 4] = (OutData[RdIndex].Lead_V2 & 0xFFFFFF)  + 0x15000000;
        BuildBufferSize += 5;
        if(BuildBufferSize >= 11) BuildBufferSize = 0;
    }
    if(BuildBufferSize >= 10)
    {
        if(CDC_IsTxBusy() != USBD_BUSY)
        {
            memcpy_reverse_32bit(BuildBuffer, OutBuffer, BuildBufferSize);
            if(USB_Transmit(OutBuffer, BuildBufferSize*4) == USBD_OK)
            {
                BuildBufferSize = 0;
            }
        }
    }
}

static inline Std_ReturnType GetFromFifo(dtEcgData *data)
{
    if(Size > 0)
    {
        if(data != 0)
        {
            memcpy_reverse_32bit(&OutData[RdIndex], data, ECG_DATA_TYPESIZE_WORD);
            RdIndex ++;
            RdIndex &= 7;
            Size --;
            return E_OK;
        }
        else
        {
            return E_NULL_PTR;
        }
    }
    else
    {
        return E_NO_DATA;
    }
}

void ICom_NewEcgData(dtEcgData *NewData)
{
    if(Size < ECG_DATA_BUFFE_SIZE)
    {
        memcpy_reverse_32bit(NewData, &OutData[WrIndex], ECG_DATA_TYPESIZE_WORD);
        WrIndex++;
        WrIndex &= 7;
        Size++;
    }
}
