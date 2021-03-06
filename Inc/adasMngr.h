/*
 * adasMngr.h
 *
 *  Created on: 2022. márc. 15.
 *      Author: Benjamin
 */

#ifndef ADASDRIVER_ADASMNGR_H_
#define ADASDRIVER_ADASMNGR_H_

typedef enum
{
    AdasMngrState_Stopped,
    AdasMngrState_Standby,
    AdasMngrState_Testing,
    AdasMngrState_Normal,
} dtAdasMngrState;

typedef enum
{
    TransitionOngoing,
    TransitionDone,
} dtStateTransition;

void adasMngr_Loop(void);
dtStateTransition adasMngr_SetState(dtAdasMngrState ReqState);
extern uint8 adasMngr_GetReadData(uint32 *buffer);

#endif /* ADASDRIVER_ADASMNGR_H_ */
