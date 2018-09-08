#ifndef _LFINTQUEUE_H_
#define _LFINTQUEUE_H_

//******************************************************************************
//******************************************************************************
//******************************************************************************
namespace LFIntQueue
{
   void initialize (int aAllocate);
   void finalize();
   int  size();

   bool tryWrite  (int  aValue);
   bool tryRead   (int* aValue);

   void show();

   unsigned long writeRetry();
   unsigned long readRetry();
   unsigned long popRetry();
   unsigned long pushRetry();
}
#endif

