/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "hw_cdc.h"
#include "fcomMsgPort.h"

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
      case FCom::cTestMsg :
         processRxMsg((FCom::TestMsg*)aMsg);
         break;
      case FCom::cEchoRequestMsg :
         processRxMsg((FCom::EchoRequestMsg*)aMsg);
         break;
      default :
         hw_cdc0_print("Unrecognized RxMsgProc::processRxMsg %d",aMsg->mMessageType);
         destroyMsg(aMsg);
         break;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - TestMsg

void RxMsgProc::processRxMsg(FCom::TestMsg* aMsg)
{
   hw_cdc0_print("RxMsgProc::processRxMsg_TestMsg %d",aMsg->mCode1);
   destroyMsg(aMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - EchoRequestMsg

void RxMsgProc::processRxMsg(FCom::EchoRequestMsg* aRxMsg)
{
   // Try to create a response message.
   EchoResponseMsg* tTxMsg = (EchoResponseMsg*)createMsg(FCom::cEchoResponseMsg);

   // Test if the response message was created.
   if (tTxMsg)
   {
      // Send the response message.
      tTxMsg->mCode1 = aRxMsg->mCode1;
      gMsgPort.doSendMsg(tTxMsg);
   }

   // Destroy the request message.
   destroyMsg(aRxMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace