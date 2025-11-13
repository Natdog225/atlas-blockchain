#include "blockchain.h"

void blockchain_destroy(blockchain_t *blockchain)
{
	block_t *block;

	if (!blockchain)
		return;

	/*
	 * Manually pop and destroy the heap-allocated block
	 */
	block = llist_pop(blockchain->chain);
	if (block)
	{
		block_destroy(block);
	}
	
	/*
	 * Manually pop the const genesis block.
	 */
	llist_pop(blockchain->chain);

	/* Destroy the (now empty) lists */
	llist_destroy(blockchain->chain, 0, NULL);
	llist_destroy(blockchain->unspent, 1, free);

	/* Free the blockchain struct itself */
	free(blockchain);
}