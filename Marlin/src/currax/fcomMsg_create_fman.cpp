/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"
#include <new>

#include "ccBlockPool.h"

#include "fcomMsg.h"

namespace FCom
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Create a new message.
// Allocate the message from the block pool, based on a message type.

void* createMsg(int aMessageType)
{
   // Block pointer.
   void* tBlockPointer = 0;

   // Try to allocate a block for the message from the block pool.
   CC::allocateBlockPoolBlock(aMessageType, (void**)&tBlockPointer, 0);

   // Exit if the block pool is empty.
   if (tBlockPointer == 0) return 0;

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
// Deallocate it back to the block pool, based on the message type.

void destroyMsg(void* aMsg)
{
   // Deallocate the block back to the block pool
   CC::deallocateBlockPoolBlock(aMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace


