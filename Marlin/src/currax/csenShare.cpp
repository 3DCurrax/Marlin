/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"
#include "csen_cdc.h"

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
   mIntQueue.initialize(cSensorQueueSize);
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
   // Guard.
   if (!mEnableFlag) return;

   // Test the counter.
   if ((mTimerCount % mTimerModulo)==0)
   {
      writeToQueue();
      // Increment the sequence counter.
      mSeqNum++;
   }

   // Increment the timer isr counter.
   mTimerCount++;
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

   // Write the timer count to the queue.
   if (!mIntQueue.tryWrite(mTimerCount))
   {
      mDropCount++;
   }
   
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

   // Try to read from the queue.
   int tTimerCount = 0;
   if (mIntQueue.tryRead(&tTimerCount))
   {
      // Print the currax sensor state.
      char tString[64];
      sprintf(tString,"csen intqueue %d %d\n",
         tTimerCount,
         mDropCount);
      csen_cdc_write(tString, strlen(tString));
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
