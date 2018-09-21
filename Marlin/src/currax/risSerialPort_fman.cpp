//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "my_functions.h"
#include "ris_cdc.h"

#include "risSerialPort.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

SerialPort::SerialPort()
{
   mValidFlag=false;
}

SerialPort::~SerialPort(void)
{
   doClose();
}

bool SerialPort::isValid(){return mValidFlag;}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void SerialPort::doOpen(char* aPortDevice,char* aPortSetup,int aRxTimeout)
{
   mValidFlag=true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void SerialPort::doClose()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Purge the comm channel

void SerialPort::doPurge()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send fixed number of bytes

int SerialPort::doSendBytes(char* aData, int aNumBytes)
{
   ris_cdc_write(aData,aNumBytes);
   return aNumBytes;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send null terminated string

int SerialPort::doSendZString(char* aData)
{
   int tNumBytes = (int)strlen(aData);
   return doSendBytes(aData,tNumBytes);   
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send one byte

int SerialPort::doSendOne(char aData)
{
   return doSendBytes(&aData,1);   
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// receive until cr/lf termination 

int  SerialPort::doReceiveUntilCRLF(char *aData, int aMaxNumBytes)
{
   int  tStatus=0;
   int  tIndex = 0;
   int  tRxStatus=0;
   char tRxChar=0;
   char tRxCharLast=0;
   bool tGoing=true;

   aData[0]=0;

   // Loop to read single bytes, store them, and exit
   // when termination cr/lf is detected
   while ( isValid() && tGoing )
   {
      // Store last byte
      tRxCharLast = tRxChar;

      // Read one byte
      tRxStatus=doReceiveOne(&tRxChar);
      if (tRxStatus >= 0)
      { 
         // Read success
         // Store byte
         aData[tIndex]=tRxChar;
         tIndex++;

         if(tRxCharLast==13 && tRxChar==10)
         {
            // Terminator detected
            tGoing=false;
            aData[tIndex-1]=0;
            tStatus=tIndex-2;
         }
         if(tIndex==aMaxNumBytes-1)
         {
            // NumBytes limit was reached
            tGoing=false;
            aData[tIndex]=0;
            tStatus = tIndex;
         }
      }
      else
      {  
         // Read failure
         tStatus = tRxStatus;
         tGoing = false;
      }
   }
   return tStatus;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// receive until cr termination 

int  SerialPort::doReceiveUntilCR(char *aData, int aMaxNumBytes)
{
   int  tStatus = 0;
   int  tIndex = 0;
   int  tRxStatus = 0;
   char tRxChar = 0;
   bool tGoing = true;

   aData[0] = 0;

   // Loop to read single bytes, store them, and exit
   // when termination cr/lf is detected
   while (isValid() && tGoing)
   {
      // Read one byte
      tRxStatus = doReceiveOne(&tRxChar);
      if (tRxStatus >= 0)
      {
         // Read success
         // Store byte
         aData[tIndex] = tRxChar;
         tIndex++;

         // If CR
         if (tRxChar == 13)
         {
            // Terminator detected
            tGoing = false;
            aData[tIndex] = 0;
            tStatus = tIndex - 1;
         }
         if (tIndex == aMaxNumBytes - 1)
         {
            // NumBytes limit was reached
            tGoing = false;
            aData[tIndex] = 0;
            tStatus = tIndex;
         }
      }
      else
      {
         // Read failure
         tStatus = tRxStatus;
         tGoing = false;
      }
   }
   return tStatus;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// receive until lf termination 

int SerialPort::doReceiveUntilLF(char *aData, int aMaxNumBytes)
{
   int  tStatus = 0;
   int  tIndex = 0;
   int  tRxStatus = 0;
   char tRxChar = 0;
   bool tGoing = true;

   aData[0] = 0;

   // Loop to read single bytes, store them, and exit
   // when termination cr/lf is detected
   while (isValid() && tGoing)
   {
      // Read one byte
      tRxStatus = doReceiveOne(&tRxChar);
      if (tRxStatus >= 0)
      {
         // Read success
         // Store byte
         aData[tIndex] = tRxChar;

         // If CR
         if (tRxChar == 10)
         {
            // Terminator detected, strip if off
            tGoing = false;
            aData[tIndex] = 0;
            tStatus = tIndex;
         }
         if (tIndex == aMaxNumBytes - 1)
         {
            // NumBytes limit was reached
            tGoing = false;
            aData[tIndex] = 0;
            tStatus = tIndex;
         }

         // Increment
         tIndex++;
      }
      else
      {
         // Read failure
         tStatus = tRxStatus;
         tGoing = false;
      }
   }
   return tStatus;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive one byte

int  SerialPort::doReceiveOne(char *aData)
{
   return doReceiveBytes(aData, 1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive

int SerialPort::doReceiveBytes(char *aData, int aNumBytes)
{
   return 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

