#pragma once

/*==============================================================================
Tcp message serial port class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risByteContent.h"
#include "risByteMsgMonkey.h"
#include "risSerialPort.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Message serial port, blocking.
//
// It exchanges byte content messages (send and recv) via a serial port.
//
// It inherits from serialPort for serial functionality and
// provides methods that can be used to transport messages.
//
// Messages are based on the ByteContent message encapsulation scheme.

class SerialMsgPort : public SerialPort
{
public:
   typedef SerialPort BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // These are transmit and receive memory. They are allocated when the 
   // message port is opened.
   char* mTxMemory;
   char* mRxMemory;

   // Size of allocated memory.
   int mMemorySize;

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
   BaseMsgMonkey* mMonkey;

   // Metrics.
   int mTxMsgCount;
   int mRxMsgCount;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Infastrcture.

   // Constructor.
   SerialMsgPort(); 
  ~SerialMsgPort(); 

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Do socket and bind calls.
  void configure(
     BaseMsgMonkeyCreator*  aMonkeyCreator,
     char*                  aPortDevice,
     char*                  aPortSetup,
     int                    aRxTimeout);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Receive a message from the socket via blocking recv calls.
   // It returns true if successful.
   bool doReceiveMsg (ByteContent*& aRxMsg);

   // Send a message over the socket via a blocking send call.
   // It returns true if successful.
   // It is protected by the transmit mutex.
   bool doSendMsg (ByteContent*  aTxMsg);

};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

