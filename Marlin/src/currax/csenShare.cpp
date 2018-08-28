/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdio.h"
#include "../core/serial.h"

#define  _CSENSHARE_CPP_
#include "csenShare.h"

namespace CSen
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor

Share::Share()
{
   mTimerCount = 0;
   mTimerModulo = 1000;
   mSeqNum = 0;
   mEnableFlag = false;
   mDropCount = 0;

   mSensorQueue.initialize(cSensorQueueSize);
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
// Read the sensors and store their values in the queue.
//
// This is called by the temperature timer isr. 

void Share::onTimer()
{
   // Increment the timer isr counter.
   mTimerCount++;

   // Test the counter.
   if ((mTimerCount % mTimerModulo)==0)
   {
      // If enabled then write to the queue.
      if (mEnableFlag)
      {
         writeToQueue();
      }
      // Increment the sequence counter.
      mSeqNum++;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Write to the queue. This is called by the ontimer routine at a rate
// that is determined by the time modulo.

void Share::writeToQueue()
{
   // Guard.
   if (!mEnableFlag) return;

   // Guard. If the queue is full then exit.
   if (!mSensorQueue.isPut())
   {
      mDropCount++;
      return;
   }

   // Fill a sample record with the current values.
   SampleRecord tRecord;
   tRecord.mSeqNum = mSeqNum;
   tRecord.mTimerCount = mTimerCount;

   // Write the sample record to the queue.
   mSensorQueue.put(tRecord);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// If the queue is not empty then read the records from it that were
// written by the isr, format them, and send them to the host via
// the usb serial cahnnel.
//
// This is called during the main loop idle processing. 

void Share::onIdle()
{
   // Guard.
   if (!mEnableFlag) return;

   // Loop while the queue is not empty.
   while(mSensorQueue.isGet())
   {
      // Get a sensor sample record from the queue.
      SampleRecord tRecord;
      mSensorQueue.get(tRecord);

      // Print the currax sensor state.
      char tString[64];
      sprintf(tString,"csen sample %d %d %d\n",
         tRecord.mSeqNum,
         tRecord.mTimerCount,
         tRecord.mDropCount);
      SERIAL_PROTOCOL(tString);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
