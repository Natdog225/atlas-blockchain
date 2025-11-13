#include "blockchain.h"

/**
 * blockchain_destroy - destroys a Blockchain
 * @blockchain: pointer to the Blockchain to destroy
 */
void blockchain_destroy(blockchain_t *blockchain)
{
	if (!blockchain)
		return;

	/*
	 * Remove the genesis block's node (at index 0).
	 * pass '1' to free the list node
	 */
	llist_remove_node(blockchain->chain, 0, 1);

	/*
	 * destroy the rest of the list.
	 */
	llist_destroy(blockchain->chain, 1, (void (*)(void *))block_destroy);

	/* Destroy the unspent list (which is empty or full of heap items) */
	llist_destroy(blockchain->unspent, 1, free);

	free(blockchain);
}
