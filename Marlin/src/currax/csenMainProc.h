#pragma once

/*==============================================================================
Currax sensor processing unit.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "ccLCValueQueue.h"
#include "csenSensorDefs.h"
#include "fcomMsgBase.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace CSen
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This class provides for currax sensor processing. It provides for reading
// the sensors periodically from a timer interrupt service routine and 
// writing values to a queue. The queue is read during idle processing
// in the main loop and values are sent to the host via the usb serial 
// channel.

class MainProc
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants:

   const static int cMaxStringSize = 64;
   const static int cSensorQueueSize = 100;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members:

   // Timer isr counter.
   int mTimerCount;

   // Sequence number. This is incremented when the queue is written to,
   // according to the timer count and the timer modulo as above.
   int mSeqNum;

   // Incremented when queue is full.
   int mDropCount;
   
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members:

   // Transmit message pointer queue.
   CC::LCValueQueue<FCom::BaseMsg*> mTxPointerQueue;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Infrastructure:

   // Constructor.
   MainProc();

   // Initialize usb udc1.
   void initialize();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods:

   // This is called by the temperature timer isr.
   // 
   // Periodically create a sample message and write it to the transmit
   // pointer queue.
   void onTimer();

   // This is called during the main loop idle processing.
   // 
   // Read from the transmit pointer queue and send available messages to
   // the host via the comm channel.
   // 
   // Poll the comm channel for messages received from the host and process
   // them when they are available.
   void onIdle();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global singular instance.

#ifdef _CSENMAINPROC_CPP_
          MainProc gMainProc;
#else
   extern MainProc gMainProc;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

