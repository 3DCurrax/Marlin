
#include "stdafx.h"

#include "ccBlockPool.h"
#include "fcomMsg.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize program memory.

void main_memory_initialize()
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize block pools.

   // Initialize block pool central facility.
   CC::initializeBlockPoolCentral();

   // Block pool parameters.
   CC::BlockPoolParms tBlockPoolParms;

   // Create block pool.
   tBlockPoolParms.reset();
   tBlockPoolParms.mPoolIndex = FCom::cSampleMsg;
   tBlockPoolParms.mBlockSize = sizeof(FCom::SampleMsg);
   tBlockPoolParms.mNumBlocks = 100;
   CC::createBlockPool(&tBlockPoolParms);

}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Finalize program memory.

void main_memory_finalize()
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Finalize block pools.

   // Finalize block pool central facility.
   // This destroys all created block pools.
   CC::finalizeBlockPoolCentral();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
