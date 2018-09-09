/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "csenShare.h"
#include "csen_central.h"

using namespace CSen;

//******************************************************************************
//******************************************************************************
//******************************************************************************

// Initialize the package. This is called from the setup routine.
void csen_initialize()
{
   gShare.initialize();
}

// Configure. This sets the timer modulo. If it is zero then it
// is disabled.
void csen_configure(int aTimerModulo)
{
   gShare.configure(aTimerModulo);
}

// Return the state as a string.
char* csen_get_state_string()
{
   return gShare.getStateString();
}

// This is called by the temperature timer isr. 
void csen_on_timer()
{
   gShare.onTimer();
}

// This is called during the main loop idle processing. 
void csen_on_idle()
{
   gShare.onIdle();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
