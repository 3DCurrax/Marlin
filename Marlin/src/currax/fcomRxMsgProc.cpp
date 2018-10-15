/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "hw_cdc.h"
#include "fcomMsgPort.h"
#include "csenSettings.h"

#define _FCOMRXMSGPROC_CPP_
#include "fcomRxMsgProc.h"

namespace FCom
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

RxMsgProc::RxMsgProc()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive message handler. It calls one of the following specific 
// message handlers, according to the message type.    

void RxMsgProc::processRxMsg(BaseMsg* aMsg)
{
   // Guard.
   if(!aMsg) return;

   // Message jump table based on message type.
   // Call a corresponding specfic message handler method.
   switch (aMsg->mMessageType)
   {
      case FCom::cSettingsMsg :
         processRxMsg((FCom::SettingsMsg*)aMsg);
         break;
      case FCom::cEchoRequestMsg :
         processRxMsg((FCom::EchoRequestMsg*)aMsg);
         break;
      default :
         hw_cdc0_print("Unrecognized RxMsgProc::processRxMsg %d",aMsg->mMessageType);
         break;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - SettingsMsg

void RxMsgProc::processRxMsg(FCom::SettingsMsg* aMsg)
{
   // First do this to disable processing during the timer interrupt.
   CSen::gSettings.mSampleEnable = false;

   // Copy the settings variables from the received message.
   CSen::gSettings.mTimerModulo = aMsg->mTimerModulo;
   CSen::gSettings.mSampleEnable  = aMsg->mSampleEnable;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - EchoRequestMsg

void RxMsgProc::processRxMsg(FCom::EchoRequestMsg* aRxMsg)
{
   // Send an echo response message.
   EchoResponseMsg tTxMsg;
   tTxMsg.mCode1 = aRxMsg->mCode1;
   gMsgPort.doSendMsg(&tTxMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace