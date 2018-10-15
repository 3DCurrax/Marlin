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
   mTxSampleMsgQueue.reset();
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
   if (!gSettings.mSampleEnable) return;
   if (gSettings.mTimerModulo == 0) return;

   // Increment the counter.
   mTimerCount++;

   // Test the counter against the modulo.
   if ((mTimerCount % gSettings.mTimerModulo) != 0) return;

   // Increment the sequence number.
   mSeqNum++;

   // Try to write a message to the transmit message queue.
   if (FCom::SampleMsg* tMsg = mTxSampleMsgQueue.startWrite())
   {
      // Call the message constructor.
      new(tMsg) FCom::SampleMsg;
      // Set message variables.
      tMsg->mTimerCount = mTimerCount;
      // Finish the write to the message queue.
      mTxSampleMsgQueue.finishWrite();
   }
   // The message queue was full.
   else
   {
      mDropCount++;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is called during the main loop idle processing.
// 
// Read from the transmit message queue and send available messages to
// the host via the comm channel.
// 
// Poll the comm channel for messages received from the host and process
// them when they are available.

void MainProc::onIdle()
{
   // Poll the transmit message queue for an available message.
   if (FCom::SampleMsg* tTxMsg = mTxSampleMsgQueue.startRead())
   {
      // If there was a message then send it to the host.
      FCom::gMsgPort.doSendMsg(tTxMsg);
      // Finish the read from the message queue.
      mTxSampleMsgQueue.finishRead();
   }

   // Poll the message port for a received message.
   FCom::BaseMsg* tRxMsg = 0;
   if (FCom::gMsgPort.doPollForRxMsg(tRxMsg))
   {
      // If there was a message then process it.
      FCom::gRxMsgProc.processRxMsg(tRxMsg);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
