#pragma once

/*==============================================================================
Tcp message serial port class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risByteBuffer.h"
#include "risByteContent.h"
#include "risByteMsgMonkey.h"
#include "risSerialHeaderBuffer.h"

#include "fcomMsgBase.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace FCom
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Message serial port for the udc1 usb port and fcom byte content messages.
// It exchanges byte content messages (send and recv) via the udc1 usb port.
// Messages are based on the ByteContent message encapsulation scheme.

class MsgPort
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   static const int cRxState_Start    = 0;
   static const int cRxState_Header   = 1;
   static const int cRxState_Payload  = 2;
   static const int cRxState_Error    = 3;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // These are transmit and receive byte buffers. They are allocated when
   // the message port is opened.
   Ris::ByteBuffer mTxByteBuffer;
   Ris::ByteBuffer mRxByteBuffer;

   // Header length.
   int mHeaderLength;
   int mPayloadLength;

   // Serial header buffer.
   Ris::SerialHeaderBuffer mHeaderBuffer;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // This a  message monkey that is used to get details about  a message from
   // a message header that is contained in a byte buffer. For receive, the 
   // message monkey allows the doRecvMsg method to receive and extract a
   // message from a byte buffer without the having the message code visible
   // to it. For transmit, message monkey allows the doSendMsg method to set
   // header data before the message is sent.
   MsgMonkey mMonkey;

   // Receiver state variable.
   int mRxState;

   // Metrics.
   int mTxMsgCount;
   int mTxDropCount;
   int mRxMsgCount;
   int mRxHeaderMissCount;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   MsgPort(); 
  ~MsgPort();

   // Initialize.
   void initialize();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Send a message. This copies a message into a byte buffer and then
   // deletes it. If there are enough bytes available in the transmit buffer
   // then it sends the byte buffer out the comm channel. If there are not
   // enough bytes in the transmit buffer then the message is dropped.
   void doSendMsg(BaseMsg* aMsg);

   // Poll for a received message. Return true if a message was received.
   // This should be called from a polling loop. Each time that it is called
   // it polls the comm channel for a byte and if one is available it puts
   // it into the receive byte buffer. When a full message has been received
   // it extracts it from the byte buffer into the given message pointer
   // and returns true.
   bool doPollForRxMsg (BaseMsg*& aMsg);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Read one byte for a message header and store it in the receive
   // byte buffer. This must be called when the number of available
   // receive bytes is at least one.
   // Return true if a header has been received.
   bool doReadHeaderOneByte();

   // Read the message payload and store it in the byte buffer.
   // This must be called when the number of available receive bytes is
   // at least the payload length.
   // Return true if the payload has been received.
   bool doReadPayload();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global singular instance.

#ifdef    _FCOMSGPORT_CPP_
          MsgPort gMsgPort;
#else
   extern MsgPort gMsgPort;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

