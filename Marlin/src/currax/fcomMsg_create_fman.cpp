/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"
#include <new>

#include "fcomMsg.h"

namespace FCom
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Create a new message.
// Allocate the message from the global one message storage. The lifetime of
// the message is short, there can only be one created message. 

static char gSingleRxMsgBlock[MsgDefT::cMsgBufferSize];

void* createMsg(int aMessageType)
{
   // Block pointer.
   void* tBlockPointer = &gSingleRxMsgBlock;

   // Call the constructor with placement new on the block pointer.
   BaseMsg* tMsg = 0;

   switch (aMessageType)
   {
   case cTestMsg:
      tMsg = new (tBlockPointer) TestMsg;
      break;
   case cSampleMsg:
      tMsg = new (tBlockPointer) SampleMsg;
      break;
   case cStatusMsg:
      tMsg = new (tBlockPointer) StatusMsg;
      break;
   case cSettingsMsg:
      tMsg = new (tBlockPointer) SettingsMsg;
      break;
   case cEchoRequestMsg:
      tMsg = new (tBlockPointer) EchoRequestMsg;
      break;
   case cEchoResponseMsg:
      tMsg = new (tBlockPointer) EchoResponseMsg;
      break;
   default:
      return 0;
      break;
   }

   // Return the new message.
   return tMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Destroy a message.

void destroyMsg(void* aMsg)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace


