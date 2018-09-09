#pragma once

/*==============================================================================
Currax sensor processing unit central interface.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
// These are a set of calls that provide an interface to the currax
// sensor processing package . The intention of this interface is to 
// isolate the sensor code from the feynman code.

   // Initialize the package. This is called from the setup routine.
   void csen_initialize();

   // Configure. This sets the timer modulo. If it is zero then it
   // is disabled.
   void csen_configure(int aTimerModulo);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods:

   // Return the state as a string.
   char* csen_get_state_string();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods:

   // This is called by the temperature timer isr. 
   void csen_on_timer();

   // This is called during the main loop idle processing. 
   void csen_on_idle();

//******************************************************************************
//******************************************************************************
//******************************************************************************
