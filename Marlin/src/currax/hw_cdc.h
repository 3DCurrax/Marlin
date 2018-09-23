#pragma once

/*==============================================================================
Currax hardware wrappers cdc interface.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
// These are a set of calls that provide a wrapper for feynman hardware
// code. They wrap the code that interfaces to udc1, the second usb endpoint.

// Return the number of transmit buffer available bytes for udc1.
int hw_cdc_get_tx_available_bytes();

// Write bytes to udc1.
void hw_cdc_write(const void* aBuffer,int aSize);

// Return the number of receive buffer available bytes for udc1.
int hw_cdc_get_rx_available_bytes();

// Read from udc1. Return the number of bytes remaining in the receive buffer.
int hw_cdc_read(void* aBuffer,int aSize);

// Read one byte from udc1.
char hw_cdc_read_one_byte();

// Wait and read one byte from udc1.
int hw_cdc_wait_and_read_one_byte();

//******************************************************************************
//******************************************************************************
//******************************************************************************
// These are a set of calls that provide a wrapper for feynman hardware
// code. They wrap the code that interfaces to udc0, the first usb endpoint.

// Write a string to udc0.
void hw_cdc0_print(char* aString);

// Write a string to udc0.
void hw_cdc0_print(char* aString,int aX1);

// Write a string to udc0.
void hw_cdc0_print(char* aString,int aX1,int aX2);

//******************************************************************************
//******************************************************************************
//******************************************************************************

