#include "non_block_queue_ndx.h"
#include <string.h>

// Limitations:
// Minimum queue size of 2
// Ex

void NBQ_Init(volatile NBQ_QueueHdr_t * pQueueHdr, void * pQueueBuffer, uint32_t itemSizeBytes, uint32_t maxItems)
{
	pQueueHdr->pBuffStart = pQueueBuffer;
	pQueueHdr->pBuffEnd = (int8_t *)pQueueBuffer + (itemSizeBytes * maxItems);  // Note: cast to (int8_t *) is to insure pointer math is in bytes
	pQueueHdr->maxItems = maxItems;
	pQueueHdr->itemSizeBytes = itemSizeBytes;
	pQueueHdr->pFront = (int8_t *)pQueueHdr->pBuffEnd - itemSizeBytes; // Init so that first/next item is stored at top of buffer
	pQueueHdr->pBack = pQueueHdr->pFront;
	pQueueHdr->pNextFront = NULL;	// Setting pNextFront to NULL indicates that front is unlocked
	pQueueHdr->pNextBack = NULL;	// Setting pNextBack to NULL indicates that front is unlocked
}


// This grabs the next Front ptr without actually advancing the front
// This all the content of the front to be modified with actually siginalling new data ready.
// To complete process call NBQ_IncFront to signal new data.
// Rerurns NBQ_FAIL if queue is full
//
// This should only be called in producer thread.
// Producer thread only reference pQueueHdr->pBack, so we'll insure we use atomic to read this variable
// Comsumer thread only references pQueueHdr->pFront, thus atomic writes are require to write this variable.
// 
GP_StatusTypeDef NBQ_AcquireFront(volatile NBQ_QueueHdr_t * pQueueHdr, volatile void ** ppNextFront) 
{
	// Check for front locked
	// pNextFront != null if front is locked
	if (pQueueHdr->pNextFront != NULL)
	{
		return GP_FAIL;
	}
	
	// Assign pNextFront to next item in queue
	pQueueHdr->pNextFront = (int8_t*)(pQueueHdr->pFront) + pQueueHdr->itemSizeBytes;
	
	/* Wrap */
	if (pQueueHdr->pNextFront >= pQueueHdr->pBuffEnd)
	{
		pQueueHdr->pNextFront = pQueueHdr->pBuffStart;
	}
	
	/* Check for Queue full */
	if (pQueueHdr->pNextFront == pQueueHdr->pBack)
	{
		pQueueHdr->pNextFront = NULL;	// Release next front
		return GP_FAIL;							// Exit with fail
	}
	
	// Return pNextFront
	*ppNextFront = pQueueHdr->pNextFront;
	
	return GP_OK;
}


void NBQ_ReleaseFront(volatile NBQ_QueueHdr_t * pQueueHdr)
{
	pQueueHdr->pFront = pQueueHdr->pNextFront; 	// Release front, by setting pFront with next item
	pQueueHdr->pNextFront = NULL; 							// Then unlock, by setting pNextFront = NULL
}


// This gets back without advancing the back pointer, thus protecting the data
// from overwrite until released.
GP_StatusTypeDef NBQ_AcquireBack(volatile NBQ_QueueHdr_t * pQueueHdr, volatile void ** ppBack)
{
		// Error if already locked
	if (pQueueHdr->pNextBack != NULL)
	{
		return GP_FAIL;
	}
	
	if (pQueueHdr->pBack == pQueueHdr->pFront) // Return error if no data
	{
		return GP_FAIL;
	}

	// Create pNextBack to next item in queue
	pQueueHdr->pNextBack = (int8_t*)(pQueueHdr->pBack) + pQueueHdr->itemSizeBytes;
	
	/* Wrap */
	if (pQueueHdr->pNextBack >= pQueueHdr->pBuffEnd)
	{
		pQueueHdr->pNextBack = pQueueHdr->pBuffStart;
	}
		
	
	// Return current back
	*ppBack = pQueueHdr->pNextBack;
	
	return GP_OK;
}


void NBQ_ReleaseBack(volatile NBQ_QueueHdr_t * pQueueHdr)
{
	pQueueHdr->pBack = pQueueHdr->pNextBack; 	// Pop back, by setting pBack with next item
	pQueueHdr->pNextBack = NULL; 							// Then unlock, by setting pNextBack = NULL
}


// Unlock back without pop
void NBQ_UnlockBack(volatile NBQ_QueueHdr_t * pQueueHdr)
{
	pQueueHdr->pNextBack = NULL; 							// Release witout poping it off the queue, this allow you to review it an leave it
}


GP_StatusTypeDef NBQ_Empty(volatile NBQ_QueueHdr_t * pQueueHdr)
{
	if (pQueueHdr->pFront == pQueueHdr->pBack)
	{
		return GP_TRUE;
	}
	
	return GP_FALSE;
}


// Check for queue full
// Full is true when pNextFront == pBack
GP_StatusTypeDef NBQ_QueueFull(volatile NBQ_QueueHdr_t * pQueueHdr)
{
	/* Create next Front */
	volatile void * pNextFrontTemp = (int8_t *)(pQueueHdr->pFront) + pQueueHdr->itemSizeBytes;
	
	/* Wrap */
	if (pNextFrontTemp >= pQueueHdr->pBuffEnd)
	{
		pNextFrontTemp = pQueueHdr->pBuffStart;
	}
	
	/* Check for Queue full */
	if (pNextFrontTemp == pQueueHdr->pBack)
	{
		return GP_FAIL;
	}
	
	return GP_OK;
}



	

