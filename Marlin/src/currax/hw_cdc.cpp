/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "USB/udi_cdc.h"
#include "hw_cdc.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// These are a set of calls that provide a wrapper for feynman hardware
// code. They wrap the code that interfaces to udc1, the seconds usb endpoint.

// Return the number of transmit buffer available bytes for udc1.
int hw_cdc_get_tx_available_bytes()
{
   return (int)udi_cdc_multi_get_free_tx_buffer(1);
}

// Write bytes to udc1.
void hw_cdc_write(const void* aBuffer,int aSize)
{
   udi_cdc_multi_write_buf(1, aBuffer, (iram_size_t)aSize);
}

// Return the number of receive buffer available bytes for udc1.
int hw_cdc_get_rx_available_bytes()
{
   return (int)udi_cdc_multi_get_nb_received_data(1);
}

// Read from udc1. Return the number of bytes remaining in the receive buffer.
int hw_cdc_read(void* aBuffer,int aSize)
{
   return udi_cdc_multi_read_buf(1, aBuffer, (iram_size_t)aSize);
}

// Read one byte from udc1.
char hw_cdc_read_one_byte()
{
   char tByte;
   udi_cdc_multi_read_buf(1, (void*)&tByte, (iram_size_t)1);
   return tByte;
}

// Wait and read one byte from udc1.
int hw_cdc_wait_and_read_one_byte()
{
   return udi_cdc_multi_getc(1);
}


//******************************************************************************
//******************************************************************************
//******************************************************************************
// These are a set of calls that provide a wrapper for feynman hardware
// code. They wrap the code that interfaces to udc0, the first usb endpoint.

// Write a string to udc0.
void hw_cdc0_print(char* aString)
{
   char tBuffer[100];
   int tSize = sprintf(tBuffer,"%s\n",aString);
   udi_cdc_multi_write_buf(0, tBuffer, (iram_size_t)tSize);
}

// Write a string to udc0.
void hw_cdc0_print(char* aString,int aX1)
{
   char tBuffer[100];
   int tSize = sprintf(tBuffer,"%s %d\n",aString,aX1);
   udi_cdc_multi_write_buf(0, tBuffer, (iram_size_t)tSize);
}

// Write a string to udc0.
void hw_cdc0_print(char* aString,int aX1,int aX2)
{
   char tBuffer[100];
   int tSize = sprintf(tBuffer,"%s %d %d\n",aString,aX1,aX2);
   udi_cdc_multi_write_buf(0, tBuffer, (iram_size_t)tSize);
}






//******************************************************************************
//******************************************************************************
//******************************************************************************
