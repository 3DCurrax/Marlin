#pragma once

/*==============================================================================
My math functions
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

//------------------------------------------------------------------------------
// Arithmentic

// Min,Max
double my_fmin (double aA,double aB);
double my_fmax (double aA,double aB);

int    my_imin (int aA,int aB);
int    my_imax (int aA,int aB);

// Close to
bool my_closeto (double aA,double aB,double aResolution);
bool my_closeto (double aA,double aB,int aResolution);

// Round to nearest integer
int    my_round (double aA);

//------------------------------------------------------------------------------
// String

// Trim CRLF from the end of a string
void my_trimCRLF(char* aString);

// Return string "true" "false" for a bool
char* my_string_from_bool(bool aValue);

char* my_stringLLU(char* aString,unsigned long long aValue);

//------------------------------------------------------------------------------
// Index arithmetic

// Index arithmetic for queues, wraps around
inline int my_index_add(int aIndex, int aDelta, int aSize)
{
   aIndex += aDelta;
   if (aIndex >= aSize) aIndex -= aSize;
   return aIndex;

}
inline int my_index_sub(int aIndex, int aDelta, int aSize)
{
   aIndex -= aDelta;
   if (aIndex < 0 ) aIndex += aSize;
   return aIndex;
}

//------------------------------------------------------------------------------
// Round up to 16 byte boundary

inline int my_round_upto16(int aValue)
{
   return ((aValue & 0xF) != 0) ? ((aValue & ~0xF) + 0x10) : aValue;

}

//******************************************************************************
//******************************************************************************
//******************************************************************************
