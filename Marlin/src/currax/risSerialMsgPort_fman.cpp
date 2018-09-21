/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "my_functions.h"

#include "risSerialMsgPort.h"
#include "risSerialHeaderBuffer.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

SerialMsgPort::SerialMsgPort()
{
   mTxMemory = 0;
   mRxMemory = 0;
   mMemorySize = 0;
   mTxMsgCount=0;
   mRxMsgCount=0;
   mMonkey=0;
}

SerialMsgPort::~SerialMsgPort()
{
   if (mMonkey != 0)
   {
      delete mMonkey;
      mMonkey = 0;
   }

   if (mTxMemory) free(mTxMemory);
   if (mRxMemory) free(mRxMemory);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Configure the serial port.

void SerialMsgPort::configure(
   BaseMsgMonkeyCreator*  aMonkeyCreator,
   char*                  aPortDevice,
   char*                  aPortSetup,
   int                    aRxTimeout)
{
   // Initialize.
   mTxMsgCount=0;
   mRxMsgCount=0;

   // Open the serial port.
   BaseClass::doOpen(aPortDevice,aPortSetup,aRxTimeout);

   // Create a message monkey.
   mMonkey = aMonkeyCreator->createMonkey();

   // Allocate memory for byte buffers.
   mMemorySize = mMonkey->getMaxBufferSize();
   mTxMemory = (char*)malloc(mMemorySize);
   mRxMemory = (char*)malloc(mMemorySize);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This copies a message into a byte buffer and then sends the byte buffer 
// out the socket.

bool SerialMsgPort::doSendMsg(ByteContent* aMsg)
{
   // Guard.
   if (!BaseClass::mValidFlag)
   {
      mMonkey->destroyMsg(aMsg);
      return false;
   }

   // Create a byte buffer from preallocated memory.
   ByteBuffer tByteBuffer(mTxMemory,mMemorySize);

   // Configure the byte buffer.
   mMonkey->configureByteBuffer(&tByteBuffer);
   tByteBuffer.setCopyTo();

   // Copy the message to the buffer.
   mMonkey->putMsgToBuffer(&tByteBuffer,aMsg);

   // Delete the message.
   mMonkey->destroyMsg(aMsg);

   // Transmit the buffer.
   int tRet = 0;
   int tLength=tByteBuffer.getLength();
   tRet = doSendBytes(tByteBuffer.getBaseAddress(),tLength);

   mTxMsgCount++;

   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This receives data from the socket into a byte buffer and then
// extracts a message from the byte buffer

bool SerialMsgPort::doReceiveMsg (ByteContent*& aMsg)
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize.

   aMsg=0;
   int tRet=0;

   // Create a byte buffer from preallocated memory.
   ByteBuffer tByteBuffer(mRxMemory, mMemorySize);

   // Configure the byte buffer.
   mMonkey->configureByteBuffer(&tByteBuffer);
   tByteBuffer.setCopyTo();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read from the serial port into a serial header buffer.

   // Header length.
   int tHeaderLength = mMonkey->getHeaderLength();

   // Serial header buffer.
   SerialHeaderBuffer tHeaderBuffer(mMonkey->getHeaderLength());

   // Loop through the received byte stream to extract the message header.
   bool tGoing = true;
   while (tGoing)
   {
      // Read one byte.
      char tByte;
      tRet = BaseClass::doReceiveOne(&tByte);
      if (tRet != 1)
      {
         return false;
      }

      // Put it to the header buffer.
      tHeaderBuffer.put(tByte);
       
      // If the header buffer is full.
      if (tHeaderBuffer.isFull())
      {
         // Copy the header buffer to the byte buffer.
         tByteBuffer.setCopyTo();
         tByteBuffer.reset();
         tHeaderBuffer.copyTo(&tByteBuffer);

         // Copy from the byte buffer into the message monkey object,
         // extract the header parameters and validate the header.
         tByteBuffer.setCopyFrom();
         tByteBuffer.rewind();
         mMonkey->extractMessageHeaderParms(&tByteBuffer);

         // If the header is valid then exit the loop.
         if (mMonkey->mHeaderValidFlag)
         {
            tGoing = false;
         }
      }
   }
   
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read the message payload into the receive buffer.

   int   tPayloadLength = mMonkey->mPayloadLength;
   char* tPayloadBuffer = tByteBuffer.getBaseAddress() + tHeaderLength;

   tRet=doReceiveBytes(tPayloadBuffer,tPayloadLength);

   // If bad status then return false.
   if (!tRet)
   {
      return false;
   }

   // Set the buffer length.
   tByteBuffer.setLength(mMonkey->mMessageLength);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // At this point the buffer contains the complete message.
   // Extract the message from the byte buffer into a new message
   // object and return it.

   tByteBuffer.rewind();
   aMsg = mMonkey->getMsgFromBuffer(&tByteBuffer);

   // Test for errors.
   if (aMsg==0)
   {
      return false;
   }

   // Test for message footer errors.
   if (!mMonkey->validateMessageFooter(&tByteBuffer,aMsg))
   {
      return false;
   }

   // Returning true  means socket was not closed
   // Returning false means socket was closed
   mRxMsgCount++;
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

}//namespace

