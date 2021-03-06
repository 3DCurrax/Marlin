#pragma once

/*==============================================================================
Currax sensor processing unit.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

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

class Settings
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members:

   // If true then prints are enabled. 
   bool mPrintEnable;

   // If true then sensor processing is enabled. Sensors will be read
   // and sensor values will be sent to the host.
   bool mSampleEnable;

   // Timer isr counter modulo.
   // If timer count % timer modulo == 0 then the queue is written to.
   // The timer isr currently executes at 1000hz, so if this is 100 then the
   // queue will be written to at 10hz.
   int mTimerModulo;

   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   Settings();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global singular instance.

#ifdef _CSENSETTINGS_CPP_
          Settings gSettings;
#else
   extern Settings gSettings;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

