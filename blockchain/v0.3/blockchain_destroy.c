#include "blockchain.h"

/**
 * blockchain_destroy - destroys a Blockchain
 * @blockchain: pointer to the Blockchain to destroy
 */
void blockchain_destroy(blockchain_t *blockchain)
{
	block_t *block;

	if (!blockchain)
		return;

	/*
	 * Pop the genesis block node.
	 */
	llist_pop(blockchain->chain);

	/*
	 * Pop the next block (the one allocated on the heap).
	 */
	block = llist_pop(blockchain->chain);
	if (block)
	{
		block_destroy(block);
	}

	/* Now that the chain list is empty, destroy it */
	llist_destroy(blockchain->chain, 0, NULL);
	
	/* Destroy the unspent list */
	llist_destroy(blockchain->unspent, 1, free);

	/* Finally, free the blockchain struct itself */
	free(blockchain);
}
