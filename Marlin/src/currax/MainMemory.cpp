
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
   tBlockPoolParms.mPoolIndex = FCom::cTestMsg;
   tBlockPoolParms.mBlockSize = sizeof(FCom::TestMsg);
   tBlockPoolParms.mNumBlocks = 2;
   CC::createBlockPool(&tBlockPoolParms);

   // Create block pool.
   tBlockPoolParms.reset();
   tBlockPoolParms.mPoolIndex = FCom::cSampleMsg;
   tBlockPoolParms.mBlockSize = sizeof(FCom::SampleMsg);
   tBlockPoolParms.mNumBlocks = 50;
   CC::createBlockPool(&tBlockPoolParms);

   // Create block pool.
   tBlockPoolParms.reset();
   tBlockPoolParms.mPoolIndex = FCom::cEchoRequestMsg;
   tBlockPoolParms.mBlockSize = sizeof(FCom::EchoRequestMsg);
   tBlockPoolParms.mNumBlocks = 2;
   CC::createBlockPool(&tBlockPoolParms);

   // Create block pool.
   tBlockPoolParms.reset();
   tBlockPoolParms.mPoolIndex = FCom::cEchoResponseMsg;
   tBlockPoolParms.mBlockSize = sizeof(FCom::EchoResponseMsg);
   tBlockPoolParms.mNumBlocks = 2;
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
