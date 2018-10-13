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
#include "csenSettings.h"

#define  _CSENMAINPROC_CPP_
#include "csenMainProc.h"

namespace CSen
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

MainProc::MainProc()
{
   mTimerCount = 0;
   mSeqNum = 0;
   mDropCount = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize usb udc1.

void MainProc::initialize()
{
   mTxPointerQueue.initialize(cSensorQueueSize);
   FCom::gMsgPort.initialize();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is called by the temperature timer isr.
// 
// Periodically create a sample message and write it to the transmit
// pointer queue.

void MainProc::onTimer()
{
   // Guard.
   if (!gSettings.mEnableFlag) return;
   if (gSettings.mTimerModulo == 0) return;

   // Increment the counter.
   mTimerCount++;

   // Test the counter against the modulo.
   if ((mTimerCount % gSettings.mTimerModulo) != 0) return;

   // Increment the sequence number.
   mSeqNum++;

   // Try to create a message.
   FCom::SampleMsg* tMsg = (FCom::SampleMsg*)FCom::createMsgFromBlockPool(FCom::cSampleMsg);
   tMsg->mTimerCount = mTimerCount;

   // Test if the message was created.
   if (tMsg)
   {
      // Try to write the message to the pointer queue.
      if (!mTxPointerQueue.tryWrite(tMsg))
      {
         // The pointer queue was full.
         FCom::destroyMsgFromBlockPool(tMsg);
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

void MainProc::onIdle()
{
   FCom::BaseMsg* tMsg = 0;

   // Try to read a message from the transmit pointer queue.
   tMsg = 0;
   if (mTxPointerQueue.tryRead(&tMsg))
   {
      // If there was a message then send it to the host.
      FCom::gMsgPort.doSendMsg(tMsg);
      // Delete the message.
      FCom::destroyMsgFromBlockPool(tMsg);
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
