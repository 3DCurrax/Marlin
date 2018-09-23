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

class Share
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

   // Timer isr counter modulo.
   // If timer count % timer modulo == 0 then the queue is written to.
   // The timer isr currently executes at 1000hz, so if this is 100 then the
   // queue will be written to at 10hz.
   int mTimerModulo;

   // Sequence number. This is incremented when the queue is written to,
   // according to the timer count and the timer modulo as above.
   int mSeqNum;

   // If true then sensor processing is enabled. Sensors will be read
   // and sensor values will be sent to the host.
   bool mEnableFlag;

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
   Share();

   // Initialize usb udc1.
   void initialize();

   // Configure. This sets the timer modulo. If it is zero then it
   //  is disabled.
   void configure(int aTimerModulo);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods:

   // Return the state as a string. This is not thread safe.
   char* getStateString();

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

#ifdef _CSENSHARE_CPP_
          Share gShare;
#else
   extern Share gShare;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

