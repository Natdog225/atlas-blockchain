#include "blockchain.h"

/**
 * block_destroy - destroys a Block
 * @block: pointer to the Block to destroy
 */
void block_destroy(block_t *block)
{
	if (!block)
		return;

	llist_destroy(block->transactions, 1,
				  (void (*)(void *))transaction_destroy);

	free(block);
}
