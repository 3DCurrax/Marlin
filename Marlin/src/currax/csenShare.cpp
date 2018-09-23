/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"
#include "hw_cdc.h"
#include "fcomMsg.h"
#include "fcomMsgBase.h"
#include "fcomMsgPort.h"
#include "fcomRxMsgProc.h"

#define  _CSENSHARE_CPP_
#include "csenShare.h"

namespace CSen
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

Share::Share()
{
   mEnableFlag = false;
   mTimerCount = 0;
   mTimerModulo = 1000;
   mSeqNum = 0;
   mDropCount = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize usb udc1.

void Share::initialize()
{
   mTxPointerQueue.initialize(cSensorQueueSize);
   FCom::gMsgPort.initialize();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Configure. This sets the timer modulo. If it is zero then it
//  is disabled.

void Share::configure(int aTimerModulo)
{
   // First do this to disable processing during the timer interrupt.
   mEnableFlag = false;

   // Set the timer modulo. If it is not zero then enable.
   mTimerModulo = aTimerModulo;
   if (mTimerModulo) mEnableFlag = true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return the state as a string. This is not thread safe.

static char rStateString[Share::cMaxStringSize];

char* Share::getStateString()
{
   sprintf(rStateString,"CSenShare %d",mTimerCount);
   return rStateString;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is called by the temperature timer isr.
// 
// Periodically create a sample message and write it to the transmit
// pointer queue.

void Share::onTimer()
{
   // Guard.
   if (!mEnableFlag) return;
   if (mTimerModulo == 0) return;

   // Increment the counter.
   mTimerCount++;

   // Test the counter against the modulo.
   if ((mTimerCount % mTimerModulo) != 0) return;

   // Increment the sequence number.
   mSeqNum++;

   // Try to create a message.
   FCom::SampleMsg* tMsg = (FCom::SampleMsg*)FCom::createMsg(FCom::cSampleMsg);
   tMsg->mTimerCount = mTimerCount;

   // Test if the message was created.
   if (tMsg)
   {
      // Try to write the message to the pointer queue.
      if (!mTxPointerQueue.tryWrite(tMsg))
      {
         // The pointer queue was full.
         FCom::destroyMsg(tMsg);
         mDropCount++;
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is called during the main loop idle processing.
// 
// Read from the transmit pointer queue and send available messages to
// the host via the comm channel.
// 
// Poll the comm channel for messages received from the host and process
// them when they are available.

void Share::onIdle()
{
   FCom::BaseMsg* tMsg = 0;

   // Try to read a message from the transmit pointer queue.
   tMsg = 0;
   if (mTxPointerQueue.tryRead(&tMsg))
   {
      // If there was a message then send it to the host.
      FCom::gMsgPort.doSendMsg(tMsg);
   }

   // Poll the message port for a received message.
   tMsg = 0;
   if (FCom::gMsgPort.doPollForRxMsg(tMsg))
   {
      // If there was a message then process it.
      FCom::gRxMsgProc.processRxMsg(tMsg);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
