
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef NON_BLOCK_QUEUE_NDX_H
#define NON_BLOCK_QUEUE_NDX_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "general_purpose_defs.h"


// Notes
// Single Produce, Single Consumer queue that can be used between threads or interrupts.
// This is a bare minimum implementation that works on any type of array of items without modifications.
// Procedure to use this.
// 
// Copying data is left up to the user.  Only pointers to memory items are supported
// 
// 
// 
//
// Queue size must be 3 or larger.


/* NBQ = Non Blocking Queue */
typedef struct
{
	volatile void * pFront;
	volatile void * pBack;
	volatile void * pBuffStart;
	volatile void * pBuffEnd;
	volatile void * pNextFront;
	volatile void * pNextBack;
	uint32_t maxItems;
	uint32_t itemSizeBytes;
} NBQ_QueueHdr_t;


void NBQ_Init(volatile NBQ_QueueHdr_t * pQueueHdr, void * pQueueBuffer, uint32_t itemSizeBytes, uint32_t maxItems);
GP_StatusTypeDef NBQ_AcquireFront(volatile NBQ_QueueHdr_t * pQueueHdr, volatile void ** ppNextFront);
void NBQ_ReleaseFront(volatile NBQ_QueueHdr_t * pQueueHdr);
GP_StatusTypeDef NBQ_AcquireBack(volatile NBQ_QueueHdr_t * pQueueHdr, volatile void ** ppBack);
void NBQ_ReleaseBack(volatile NBQ_QueueHdr_t * pQueueHdr);
void NBQ_UnlockBack(volatile NBQ_QueueHdr_t * pQueueHdr);
GP_StatusTypeDef NBQ_Empty(volatile NBQ_QueueHdr_t * pQueueHdr);
GP_StatusTypeDef NBQ_IsFull(volatile NBQ_QueueHdr_t * pQueueHdr);

#ifdef __cplusplus
}
#endif

#endif
