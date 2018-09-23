/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "my_functions.h"

#include "risSerialHeaderBuffer.h"

#include "hw_cdc.h"
#include "fcomMsgPort.h"

namespace FCom
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

MsgPort::MsgPort()
{
   mTxMsgCount = 0;
   mTxDropCount = 0;
   mRxMsgCount = 0;
   mRxHeaderMissCount = 0;
   mHeaderLength = 0;
   mPayloadLength = 0;
   mRxState = 0;
}

MsgPort::~MsgPort()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize.

void MsgPort::initialize()
{
   // Initialize.
   mTxMsgCount = 0;
   mTxDropCount = 0;
   mRxMsgCount = 0;
   mRxHeaderMissCount = 0;

   // Allocate memory for byte buffers.
   int tMemorySize = mMonkey.getMaxBufferSize();
   mTxByteBuffer.memAlloc(tMemorySize);
   mRxByteBuffer.memAlloc(tMemorySize);

   mMonkey.configureByteBuffer(&mTxByteBuffer);
   mMonkey.configureByteBuffer(&mRxByteBuffer);

   // Initialize receiver header variables.
   mHeaderLength = mMonkey.getHeaderLength();
   mHeaderBuffer.initialize(mHeaderLength);
   mRxState = cRxState_Start;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a message. This copies a message into a byte buffer and then
// deletes it. If there are enough bytes available in the transmit buffer
// then it sends the byte buffer out the comm channel. If there are not
// enough bytes in the transmit buffer then the message is dropped.

void MsgPort::doSendMsg(BaseMsg* aMsg)
{
   // Reset the byte buffer.
   mTxByteBuffer.reset();

   // Copy the message to the byte buffer.
   mMonkey.putMsgToBuffer(&mTxByteBuffer,aMsg);

   // Delete the message.
   mMonkey.destroyMsg(aMsg);

   // If there are enough bytes available in the transmit buffer then
   // transmit the byte buffer.
   int tLength = mTxByteBuffer.getLength();
   if (hw_cdc_get_tx_available_bytes() >= tLength)
   {
      hw_cdc_write(mTxByteBuffer.getBaseAddress(),tLength);
      mTxMsgCount++;
   }
   else
   {
      mTxDropCount++;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Poll for a received message. Return true if a message was received.
// This should be called from a polling loop. Each time that it is called
// it polls the comm channel for a byte and if one is available it puts
// it into the receive byte buffer. When a full message has been received
// it extracts it from the byte buffer into the given message pointer
// and returns true.

bool MsgPort::doPollForRxMsg (BaseMsg*& aMsg)
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // If the state is for start.

   if (mRxState == cRxState_Start)
   {
      // Reset the byte buffer.
      mRxByteBuffer.reset();

      // Reinitialize the header buffer.
      mHeaderBuffer.reinitialize();

      // Set the state for header.
      mRxState = cRxState_Header;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // If the state is for header.

   if (mRxState == cRxState_Header)
   {
      // If the number of available bytes is at least one then read
      // one byte for the header.
      if (hw_cdc_get_rx_available_bytes() > 0)
      {
         // Read one byte for the header.
         if (doReadHeaderOneByte())
         {
            // If the full header has been received then set the state
            // for payload.
            mRxState = cRxState_Payload;
         }
         else
         {
            return false;
         }
      }
      else
      {
         return false;
      }
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // If the state is for payload.

   if (mRxState == cRxState_Payload)
   {
      // If the number of available bytes is at least the payload length
      // then read the entire payload.
      if (hw_cdc_get_rx_available_bytes() >= mPayloadLength)
      {
         // Read the payload.
         if (!doReadPayload())
         {
            // If the payload has not been received then set the state
            // for error.
            mRxState = cRxState_Error;
            return false;
         }
      }
      else
      {
         return false;
      }
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // At this point the buffer contains the complete message.
   // Extract the message from the byte buffer into a new message
   // object and return it.

   mRxByteBuffer.rewind();
   aMsg = (BaseMsg*)mMonkey.getMsgFromBuffer(&mRxByteBuffer);

   // Test for errors.
   if (aMsg==0)
   {
      hw_cdc0_print("ERROR getMsgFromBuffer");
      return false;
   }

   // Success. Set the state for start again.
   mRxState = cRxState_Start;
   mRxMsgCount++;
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Read one byte for a message header and store it in the receive
// byte buffer. This must be called when the number of available
// receive bytes is at least one.
// Return true if a header has been received.

bool MsgPort::doReadHeaderOneByte()
{
   int tRet;

   // Read one byte.
   char tByte = hw_cdc_read_one_byte();

   // Put the byte to the header buffer.
   mHeaderBuffer.put(tByte);

   // If the header buffer is not full then exit.
   if (!mHeaderBuffer.isFull()) return false;

   // A full header has been received,
   // copy the header buffer to the byte buffer.
   mRxByteBuffer.setCopyTo();
   mRxByteBuffer.reset();
   mHeaderBuffer.copyTo(&mRxByteBuffer);

   // Copy from the byte buffer into the message monkey object,
   // extract the header parameters and validate the header.
   mRxByteBuffer.setCopyFrom();
   mRxByteBuffer.rewind();
   mMonkey.extractMessageHeaderParms(&mRxByteBuffer);

   // If the header is invalid then exit.
   if (!mMonkey.mHeaderValidFlag)
   {
      mRxHeaderMissCount++;
      return false;
   }

   // Header received.
   mPayloadLength = mMonkey.mPayloadLength;
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Read the message payload and store it in the byte buffer.
// This must be called when the number of available receive bytes is
// at least the payload length.
// Return true if the payload has been received.

bool MsgPort::doReadPayload()
{
   char* tPayloadBuffer = mRxByteBuffer.getBaseAddress() + mHeaderLength;

   hw_cdc_read(tPayloadBuffer,mPayloadLength);

   // Set the buffer length.
   mRxByteBuffer.setLength(mMonkey.mMessageLength);

   // Full payload was received.
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

