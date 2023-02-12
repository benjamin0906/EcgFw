/*
 * adasMngr.c
 *
 *  Created on: 2022. márc. 15.
 *      Author: Benjamin
 */

#include "adasMngr_Types.h"
#include "adasMngr.h"
#include "config.h"
#include "SysTick_red.h"
#include "GPIO.h"
#include "Utilities.h"
#include "SPI.h"

static uint32 TimeStamp;
static uint32 TxBuff[8];
static uint8 RxLen;
static uint32 RxBuff[20];
static dtAdasMngrState adasMngrReqState;
static dtAdasMngrState myState = AdasMngrState_Stopped;

dtAdasMsg MsgTxBuff[6] = {{.Word = 0xFFFFFFFF}, {.Word = 0xFFFFFFFF}, {.Word = 0xFFFFFFFF}, {.Word = 0xFFFFFFFF}, {.Word = 0xFFFFFFFF}, {.Word = 0xFFFFFFFF}};
dtAdasMsg MsgRxBuff[6];
uint8 MsgTxBuffWrPtr;
uint8 MsgTxBuffRdPtr;

dtAdasConfig AdasReqConfig = {  .Registers.FrameCtrl        = {.RW = 1, .Address = 0x0A, .Fields = {.Res = 0x7, .Res2 = 1, .PACEDIS = 1, .RESPMDIS = 1, .RESPPHDIS = 1, .LOFFDIS = 1, .GPIODIS = 1, .CRCDIS = 1}},
                                .Registers.CmRefShieldCtrl  = {.RW = 1, .Address = 0x05, .Fields = {.LACM = 1, .LLCM = 1, .RACM = 1, .RLDEN = 1, .SHLDEN = 1}},
                                .Registers.TestToneCtrl     = {.RW = 1, .Address = 0x08, .Fields = {.TONLA = 1, .TONTYPE = 0, .TONINT = 1, .TONEN = 1}},
                                .Registers.FiltCtrl         = {.RW = 1, .Address = 0x0B, .Fields = {}},
                                .Registers.EcgCtrl          = {.RW = 1, .Address = 0x01, .Fields = {. LAEN = 1, .LLEN = 1, .RAEN = 1, .V1EN = 1, .V2EN = 1, .CHCONFIG = 1, .GAIN=0, .VREFBUF = 1, .CLKEXT = 1, .MASTER = 1, .HP = 1, .CNVEN = 1, .PWREN = 1}}
};
dtAdasConfig AdasReadConfig = { .Registers.FrameCtrl        = {.RW = 0, .Address = 0x0A, },
                                .Registers.CmRefShieldCtrl  = {.RW = 0, .Address = 0x05, },
                                .Registers.TestToneCtrl     = {.RW = 0, .Address = 0x08, },
                                .Registers.FiltCtrl         = {.RW = 0, .Address = 0x0B, },
                                .Registers.EcgCtrl          = {.RW = 0, .Address = 0x01, }
};

int32 readData[4][5];
uint8 readDataWrIndex;
uint8 readDataRdIndex;
static dtReadState ReadState;

void adasMngr_Loop(void);
dtStateTransition adasMngr_SetState(dtAdasMngrState ReqState);
uint8 adasMngr_GetReadData(uint32 *buffer);
dtAdasMngrState StoppedStateHandler(void);
dtAdasMngrState StandbyStateHandler(void);
dtAdasMngrState TestingStateHandler(void);
dtAdasMngrState NormalStateHandler(void);
void adasMngr_TriggerRead(void);

void adasMngr_Loop(void)
{
    AdasReqConfig.Registers.EcgCtrl.Fields.PWREN = 1;
    switch(myState)
    {
        case AdasMngrState_Stopped:
            myState = StoppedStateHandler();
            break;
        case AdasMngrState_Standby:
            myState = StandbyStateHandler();
            break;
        case AdasMngrState_Testing:
            myState = TestingStateHandler();
            break;
        case AdasMngrState_Normal:
            myState = NormalStateHandler();
            break;
    }
    if((myState == AdasMngrState_Testing) || (myState == AdasMngrState_Normal))
    {
        switch(ReadState)
        {
            case ReadState_StartRead:
                break;
            case ReadState_WaitRead:
                if(SpiIdle == ISPI_GetData(1, RxBuff, 28))
                {
                    ReadState = ReadState_StartRead;
                    MemCpyRigth(&RxBuff[1], &readData[readDataWrIndex][0], 5*4);
                    changeEndiannessArray(&readData[readDataWrIndex][0], 5);
                    saturateI32(readData[readDataWrIndex][0], readData[readDataWrIndex][0], 24);
                    saturateI32(readData[readDataWrIndex][1], readData[readDataWrIndex][1], 24);
                    saturateI32(readData[readDataWrIndex][2], readData[readDataWrIndex][2], 24);
                    readData[readDataWrIndex][0]>>=5;
                    readData[readDataWrIndex][1]>>=5;
                    readData[readDataWrIndex][2]>>=5;
                    readDataWrIndex++;
                    readDataWrIndex &= 3;
                }
                break;
        }

    }
}

void adasMngr_TriggerRead(void)
{
    ISPI_Send(1, 0, 24);
    ReadState = ReadState_WaitRead;
}

uint8 adasMngr_GetReadData(uint32 *buffer)
{
    uint32 ret = 0;
    if(readDataWrIndex != readDataRdIndex)
    {
        MemCpyRigth(&readData[readDataRdIndex++][0], buffer, 5*sizeof(uint32));
        readDataRdIndex &= 3;
        ret = 1;
    }
    return ret;
}

dtAdasMngrState StoppedStateHandler(void)
{
    dtAdasMngrState ret = AdasMngrState_Stopped;
    static dtAdasStoppedStateStates State = StoppedState_Neutral;

    switch(State)
    {
        case StoppedState_Neutral:
            if(adasMngrReqState != AdasMngrState_Stopped)
            {
                State = StoppedState_InitPins;
            }
            break;
        case StoppedState_InitPins:
            GPIO_Set(ADAS_RESET, Set);
            GPIO_Set(ADAS_PD, Set);
            TimeStamp = SysTick_GetTicks();
            State = StoppedState_WaitInitPins;
            break;
        case StoppedState_WaitInitPins:
            if(IsPassed(TimeStamp, 50))
            {
                State = StoppedState_TriggerReset;
            }
            break;
        case StoppedState_TriggerReset:
            GPIO_Set(ADAS_RESET, Clear);
            TimeStamp = SysTick_GetTicks();
            State = StoppedState_WaitResetTrigger;
            break;
        case StoppedState_WaitResetTrigger:
            if(IsPassed(TimeStamp, 10))
            {
                GPIO_Set(ADAS_RESET, Set);
                State = StoppedState_WaitReset;
            }
            break;
        case StoppedState_WaitReset:
            if(GPIO_Get(ADAS_DRDY) == 0)
            {
                State = StoppedState_SendConf;
            }
            break;
        case StoppedState_SendConf:
            ISPI_Send(1, &AdasReqConfig.StartWord, sizeof(AdasReqConfig));
            State = StoppedState_CheckSendConf;
            break;
        case StoppedState_CheckSendConf:
            if(SpiIdle == ISPI_GetData(1, 0, 0))
            {
                State = StoppedState_ReadConf;
            }
            break;
        case StoppedState_ReadConf:
        {
            uint32 temp[sizeof(AdasReadConfig)/4+1];
            memcpy_reverse_32bit(&AdasReadConfig.StartWord, temp, sizeof(AdasReadConfig)/4);
            temp[sizeof(AdasReadConfig)] = 0;
            ISPI_Send(1, temp, sizeof(AdasReadConfig)+4);
            State = StoppedState_CheckReadConf;
        }
            break;
        case StoppedState_CheckReadConf:
        {
            uint32 t[sizeof(AdasReadConfig)/4+1];
            if(SpiIdle == ISPI_GetData(1, &t, sizeof(AdasReadConfig)+4))
            {
                memcpy_reverse_32bit(&t[1], &AdasReadConfig.StartWord, sizeof(AdasReadConfig)/4);
                uint8 i;
                for(i = 0; (i < sizeof(AdasReadConfig)/4) && (*(&AdasReadConfig.StartWord+i) == (*(&AdasReqConfig.StartWord+i)&0xFFFFFF7F)); i++);
                if(i == (sizeof(AdasReadConfig)/4))
                {
                    State = StoppedState_Done;
                }
            }
        }
            break;
        case StoppedState_Done:
            ret = AdasMngrState_Standby;
            State = StoppedState_Neutral;
            break;
    }

    return ret;
}

dtAdasMngrState StandbyStateHandler(void)
{
    dtAdasMngrState ret = AdasMngrState_Standby;
    static dtAdasStandbyStateStates State = StandbyState_Neutral;

    switch(State)
    {
        case StandbyState_Neutral:
            switch(adasMngrReqState)
            {
                case AdasMngrState_Testing:
                    State = StandbyState_StartReadFrame_Send;
                    break;
                case AdasMngrState_Normal:
                    break;
                case AdasMngrState_Stopped:
                    State = StandbyState_Stopped_TriggerReset;
                    break;
            }
            break;
        case StandbyState_Stopped_TriggerReset:
            GPIO_Set(ADAS_RESET, Clear);
            TimeStamp = SysTick_GetTicks();
            State = StandbyState_Stopped_WaitResetTrigger;
            break;
        case StandbyState_Stopped_WaitResetTrigger:
            if(IsPassed(TimeStamp, 10))
            {
                GPIO_Set(ADAS_RESET, Set);
                State = StandbyState_Stopped_WaitReset;
            }
            break;
        case StandbyState_Stopped_WaitReset:
            if(GPIO_Get(ADAS_DRDY) == 0)
            {
                State = StandbyState_Stopped_Done;
            }
            break;
        case StandbyState_StartReadFrame_Send:
            TxBuff[0] = 0x40000000;
            TxBuff[1] = 0x00000000;
            changeEndiannessArray(TxBuff, 1);
            //SPI_Send(1, TxBuff, 4, 0, 0, 0);
            ISPI_Send(1, TxBuff, 28);
            State = StandbyState_StartReadFrame_Check;
            break;
        case StandbyState_StartReadFrame_Check:
            if(SpiIdle == ISPI_GetData(1, 0, 0))
            //if(SpiIdle == SPI_Status(1))
            {
                TxBuff[1] = 0x00000000;
                State = StandbyState_Neutral;
                switch(adasMngrReqState)
                {
                    case AdasMngrState_Normal:
                        ret = AdasMngrState_Normal;
                        break;
                    case AdasMngrState_Testing:
                        ret = AdasMngrState_Testing;
                        break;
                }
            }
            break;
        case StandbyState_Stopped_Done:
            ret = AdasMngrState_Stopped;
            State = StandbyState_Neutral;
            break;
    }

    return ret;
}

dtAdasMngrState TestingStateHandler(void)
{
    dtAdasMngrState ret = AdasMngrState_Testing;
    static dtAdasTestingStateStates State = TestingState_Neutral;

    switch(State)
    {
        case TestingState_Neutral:
            break;
    }

    return ret;
}

dtAdasMngrState NormalStateHandler(void)
{
    dtAdasMngrState ret = AdasMngrState_Normal;
    static dtAdasNormalStateStates State = NormalState_Neutral;

    switch(State)
    {
        case NormalState_Neutral:
            break;
    }

    return ret;
}

dtStateTransition adasMngr_SetState(dtAdasMngrState ReqState)
{
    adasMngrReqState = ReqState;
    return (myState == adasMngrReqState) ? TransitionDone : TransitionOngoing;
}
