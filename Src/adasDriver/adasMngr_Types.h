/*
 * adasMngr_Types.h
 *
 *  Created on: 2022. márc. 15.
 *      Author: Benjamin
 */

#ifndef ADASDRIVER_ADASMNGR_TYPES_H_
#define ADASDRIVER_ADASMNGR_TYPES_H_

#include "types.h"

typedef enum
{
    StoppedState_Neutral,
    StoppedState_InitPins,
    StoppedState_WaitInitPins,
    StoppedState_TriggerReset,
    StoppedState_WaitResetTrigger,
    StoppedState_WaitReset,
    StoppedState_SendConf,
    StoppedState_CheckSendConf,
    StoppedState_ReadConf,
    StoppedState_CheckReadConf,
    StoppedState_Done,
} dtAdasStoppedStateStates;

typedef enum
{
    StandbyState_Neutral,
    StandbyState_Stopped_TriggerReset,
    StandbyState_Stopped_WaitResetTrigger,
    StandbyState_Stopped_WaitReset,
    StandbyState_Stopped_Done,

    StandbyState_StartReadFrame_Send,
    StandbyState_StartReadFrame_Check,
} dtAdasStandbyStateStates;

typedef enum
{
    TestingState_Neutral,
} dtAdasTestingStateStates;

typedef enum
{
    NormalState_Neutral,
} dtAdasNormalStateStates;

typedef enum
{
    adasRead,
    adasWrite
} dtadasReadWrite;

typedef enum
{
    ReadState_StartRead,
    ReadState_WaitRead,

} dtReadState;

enum eAdasInitStates
{
    InitStates_Init,
    InitStates_WaitForInit,
    Initstates_TriggerReset,
    Initstates_WaitForPinTrigger,
    Initstates_WaitForReset,
    InitStates_SendConfig,
    InitStates_WaitForSendConfig,
    InitStates_ReadConfig,
    InitStates_WaitForReadConfig,
    InitStates_TestRead,
    InitStates_WaitTestRead,

    InitStates_ChangeState,

};

typedef union
{
    struct
    {
        uint8 Addr;
        uint8 Data[3];
    } Fields;
    uint32 Word;
} dtAdasMsg;

typedef enum
{
    AdasMsg_Nop                 = 0x00,
    AdasMsg_EcgControl          = 0x01,
    AdasMsg_LeadOffControl      = 0x02,
    AdasMsg_RespControl         = 0x03,
    AdasMsg_PaceDetControl      = 0x04,
    AdasMsg_CommonControl       = 0x05,
    AdasMsg_GPIOControl         = 0x06,
    AdasMsg_PaceAmpThres        = 0x07,
    AdasMsg_TestTone            = 0x08,
    AdasMsg_CalibDAC            = 0x09,
    AdasMsg_FrameControl        = 0x0A,
    AdasMsg_FilterControl       = 0x0B,
    AdasMsg_ACLeadoffUpTh       = 0x0C,
    AdasMsg_ACLeadoffLowTh      = 0x0D,
    AdasMsg_PaceEdgeTh          = 0x0E,
    AdasMsg_PaceLevelTh         = 0x0F,
    AdasMsg_LeadI               = 0x11,
    AdasMsg_LeadII              = 0x12,
    AdasMsg_LeadIII             = 0x13,
    AdasMsg_V1                  = 0x14,
    AdasMsg_V2                  = 0x15,
    AdasMsg_PaceStatus          = 0x1A,
    AdasMsg_RespMagnitude       = 0x1B,
    AdasMsg_RespPhase           = 0x1C,
    AdasMsg_LeadOffStatus,
    AdasMsg_DCLeadOff,
    AdasMsg_OpState,
    AdasMsg_ExtResp,
    AdasMsg_UserGainCalLA,
    AdasMsg_UserGainCalLL,
    AdasMsg_UserGainCalRA,
    AdasMsg_UserGainCalV1,
    AdasMsg_UserGainCalV2,
    AdasMsg_LeadOffAmpLA        = 0x31,
    AdasMsg_LeadOffAmpLL,
    AdasMsg_LeadOffAmpRA,
    AdasMsg_LeadOffAmpV1,
    AdasMsg_LeadOffAmpV2,
    AdasMsg_Pace1WidthAmp       = 0x3A,
    AdasMsg_Pace2WidthAmp       = 0x3B,
    AdasMsg_Pace3WidthAmp       = 0x3C,
    AdasMsg_FrameHeader         = 0x40,
    AdasMsg_FrameCrc            = 0x41,
} dtAdasMsgTypes;

typedef struct
{
    uint8 Res       :3;
    uint8 V2EN      :1;
    uint8 V1EN      :1;
    uint8 RAEN      :1;
    uint8 LLEN      :1;
    uint8 LAEN      :1;

    uint8 GAIN      :2;
    uint8 CHCONFIG  :1;
    uint8 Res2      :5;

    uint8 SWRST     :1;
    uint8 PWREN     :1;
    uint8 CNVEN     :1;
    uint8 HP        :1;
    uint8 Gang      :1;
    uint8 MASTER    :1;
    uint8 CLKEXT    :1;
    uint8 VREFBUF   :1;
} dtAdasEcgCtrl;

typedef struct
 {
    uint8           Res3;           //Set to 0
    uint8           Res4;           //Set to 0

    uint8           Res         :2; //Set to 0
    uint8           LPF         :2;
    uint8           N2KBP       :1;
    uint8           MN2K        :1;
    uint8           Res2        :2; //Set to 0

} dtAdasFiltCtrl;

typedef struct
{
    uint8           Res     :3; //Set to 0
    uint8           TONV2   :1;
    uint8           TONV1   :1;
    uint8           TONRA   :1;
    uint8           TONLL   :1;
    uint8           TONLA   :1;

    uint8           Res2;       //Set to 0

    uint8           TONEN   :1;
    uint8           TONOUT  :1;
    uint8           TONINT  :1;
    uint8           TONTYPE :2;
    uint8           Res3    :3; //Set to 0
} dtAdasTestTone;

typedef struct
{
    uint8           Res     :3;//Set to 0
    uint8           V2CM    :1;
    uint8           V1CM    :1;
    uint8           RACM    :1;
    uint8           LLCM    :1;
    uint8           LACM    :1;

    uint8           CEREFEN :1;
    uint8           CERLD   :1;
    uint8           V2RLD   :1;
    uint8           V1RLD   :1;
    uint8           RARLD   :1;
    uint8           LLRLD   :1;
    uint8           LARLD   :1;
    uint8           Res2    :1;//Set to 0

    uint8           SHLDEN  :1;
    uint8           RLDEN   :1;
    uint8           EXTCM   :1;
    uint8           DRVCM   :1;
    uint8           RLDSEL  :4;
} dtAdasCmRefShieldCtrl;

typedef struct
{
    uint8           Res         :3;//Set to 1
    uint8           LEADV2      :1;
    uint8           LEADV1      :1;
    uint8           LEADIII     :1;
    uint8           LEADII      :1;
    uint8           LEADI       :1;

    uint8           Res3        :1;//Set to 0
    uint8           CRCDIS      :1;
    uint8           GPIODIS     :1;
    uint8           LOFFDIS     :1;
    uint8           RESPPHDIS   :1;
    uint8           RESPMDIS    :1;
    uint8           PACEDIS     :1;
    uint8           Res2        :1;//Set to 1

    uint8           FRMRATE     :2;
    uint8           SKIP        :2;
    uint8           DATAFMT     :1;
    uint8           Res4        :1;//Set to 0
    uint8           RDYRPT      :1;
    uint8           ADIS        :1;
} dtAdasFrameCtrl;

/*
 * typedef struct
{
        uint8          FRMRATE     :2;
        uint8          SKIP        :2;
        uint8          DATAFMT     :1;
        const uint8    Res3        :1;//Set to 0
        uint8          RDYRPT      :1;
        uint8          ADIS        :1;
        const uint8    Res2        :1;//Set to 0
        uint8          CRCDIS      :1;
        uint8          GPIODIS     :1;
        uint8          LOFFDIS     :1;
        uint8          RESPPHDIS   :1;
        uint8          RESPMDIS    :1;
        uint8          PACEDIS     :1;
        const uint8    Res         :4;//Set to 1
        uint8          LEADV2      :1;
        uint8          LEADV1      :1;
        uint8          LEADIII     :1;
        uint8          LEADII      :1;
        uint8          LEADI       :1;
} dtAdasFrameCtrl;
 * */

typedef union
{
    uint32 StartWord;
    struct
    {
        struct
        {
            uint8                   Address   :7;//0x0A
            uint8                   RW        :1;
            dtAdasFrameCtrl         Fields;
        } FrameCtrl;
        struct
        {
            uint8                   Address     :7;//0x05
            uint8                   RW          :1;
            dtAdasCmRefShieldCtrl   Fields;
        } CmRefShieldCtrl;
        struct
        {
            uint8                   Address     :7;//0x08
            uint8                   RW          :1;
            dtAdasTestTone          Fields;
        } TestToneCtrl;
        struct
        {
            uint8                   Address     :7;//0x0B
            uint8                   RW          :1;
            dtAdasFiltCtrl          Fields;
        } FiltCtrl;
        struct
        {
            uint8                   Address     :7;//0x01
            uint8                   RW          :1;
            dtAdasEcgCtrl           Fields;
        } EcgCtrl;
    } Registers;
} dtAdasConfig;

#endif /* ADASDRIVER_ADASMNGR_TYPES_H_ */
