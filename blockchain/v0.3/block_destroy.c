#include "blockchain.h"

/**
 * block_destroy - destroys a Block
 * @block: pointer to the Block to destroy
 */
void block_destroy(block_t *block)
{
	if (!block)
		return;

	/* NEW LINE FOR TASK 10 */
	/* 1 = free content, (node_func_t)transaction_destroy = free function */
	llist_destroy(block->transactions, 1, (node_func_t)transaction_destroy);

	free(block);
}
