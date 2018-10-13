/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
// git test
#include "stdafx.h"

#include "fcomMsg.h"

namespace FCom
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

TestMsg::TestMsg ()
{
   mMessageType = cTestMsg;
   mCode1 = 901;
} 

void TestMsg::copyToFrom (Ris::ByteBuffer* aBuffer)
{
   mHeader.headerCopyToFrom(aBuffer,this);

   aBuffer->copy( &mCode1 );

   mHeader.headerReCopyToFrom(aBuffer,this);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

SampleMsg::SampleMsg()
{
   mMessageType = cSampleMsg;
   mTimerCount = 0;
}

void SampleMsg::copyToFrom(Ris::ByteBuffer* aBuffer)
{
   mHeader.headerCopyToFrom(aBuffer, this);

   aBuffer->copy(&mTimerCount);

   mHeader.headerReCopyToFrom(aBuffer, this);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

StatusMsg::StatusMsg()
{
   mMessageType = cStatusMsg;
   mCode1 = 0;
}

void StatusMsg::copyToFrom(Ris::ByteBuffer* aBuffer)
{
   mHeader.headerCopyToFrom(aBuffer, this);

   aBuffer->copy(&mCode1);

   mHeader.headerReCopyToFrom(aBuffer, this);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

SettingsMsg::SettingsMsg()
{
   mMessageType = cSettingsMsg;
   mEnable = false;
   mTimerModulo = 0;
}

void SettingsMsg::copyToFrom(Ris::ByteBuffer* aBuffer)
{
   mHeader.headerCopyToFrom(aBuffer, this);

   aBuffer->copy(&mEnable);
   aBuffer->copy(&mTimerModulo);

   mHeader.headerReCopyToFrom(aBuffer, this);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

EchoRequestMsg::EchoRequestMsg ()
{
   mMessageType = cEchoRequestMsg;

   mCode1 = 101;
} 

void EchoRequestMsg::copyToFrom (Ris::ByteBuffer* aBuffer)
{
   mHeader.headerCopyToFrom(aBuffer,this);

   aBuffer->copy        (& mCode1  );

   mHeader.headerReCopyToFrom(aBuffer,this);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

EchoResponseMsg::EchoResponseMsg()
{
   mMessageType = cEchoResponseMsg;

   mCode1 = 201;
}

void EchoResponseMsg::copyToFrom(Ris::ByteBuffer* aBuffer)
{
   mHeader.headerCopyToFrom(aBuffer, this);

   aBuffer->copy(&mCode1);

   mHeader.headerReCopyToFrom(aBuffer, this);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace


