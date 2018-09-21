/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <time.h>

#include "my_functions.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Min,Max

double my_fmin (double aA,double aB) {return aA > aB ? aB : aA;}
double my_fmax (double aA,double aB) {return aA > aB ? aA : aB;}

int    my_imin (int aA,int aB) {return aA > aB ? aB : aA;}
int    my_imax (int aA,int aB) {return aA > aB ? aA : aB;}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Close to

bool my_closeto (double aA,double aB,double aResolution)
{
   return (fabs(aA - aB) <= aResolution);
}

bool my_closeto (double aA,double aB,int aResolution)
{
   switch (aResolution)
   {
      case  6  : return (fabs(aA - aB) <= 1000000.0);
      case  5  : return (fabs(aA - aB) <= 100000.0);
      case  4  : return (fabs(aA - aB) <= 10000.0);
      case  3  : return (fabs(aA - aB) <= 1000.0);
      case  2  : return (fabs(aA - aB) <= 100.0);
      case  1  : return (fabs(aA - aB) <= 10.0);
      case  0  : return (fabs(aA - aB) <= 1.0);
      case -1  : return (fabs(aA - aB) <= 0.1);
      case -2  : return (fabs(aA - aB) <= 0.01);
      case -3  : return (fabs(aA - aB) <= 0.001);
      case -4  : return (fabs(aA - aB) <= 0.0001);
      case -5  : return (fabs(aA - aB) <= 0.00001);
      case -6  : return (fabs(aA - aB) <= 0.000001);
      case -7  : return (fabs(aA - aB) <= 0.0000001);
      case -8  : return (fabs(aA - aB) <= 0.00000001);
      default  : return (fabs(aA - aB) <= 0.01);
   }
   return false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Round to nearest integer

int my_round (double aA)
{
   if   (aA >= 0.0)  return int(aA + 0.5);
   else              return int(aA - 0.5);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Strings

// Trim CRLF from the end of a string
void my_trimCRLF(char* aString)
{
   // Remove cr/lf at end of line
   unsigned tStringLen = (unsigned)strlen(aString);

   if (tStringLen >= 2)
   {
      if (aString[tStringLen - 1] == 0xa)
      {
         aString[tStringLen - 1] = 0;

         if (aString[tStringLen - 2] == 0xd)
         {
            aString[tStringLen - 2] = 0;
         }
      }
   }
}

// Return string "true" "false" for a bool
char* my_string_from_bool(bool aValue)
{
   return aValue ? (char*)"true" : (char*)"false";
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Convert string to upper case

void my_strupr(char* aString)
{
   unsigned char* tPtr = (unsigned char*)aString;
   int tIndex=0;
   while (tPtr[tIndex]!=0)
   {
      int tValue = (int)tPtr[tIndex];
      if ((tValue >= 97)&&(tValue <= 122))
      {
         tValue -= 32;
         tPtr[tIndex] = (unsigned char)tValue;
      }
      tIndex++;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Copy string

void my_strncpy(char* aDestin,const char* aSource,unsigned int aSize)
{
   strncpy(aDestin,aSource,(size_t)aSize);
   aDestin[aSize-1]=0;
}

