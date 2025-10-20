#include "blockchain.h"
#include <llist.h>

/**
 * blockchain_destroy - Deletes an existing blockchain and all its blocks.
 * @blockchain: A pointer to the blockchain to be destroyed.
 */
void blockchain_destroy(blockchain_t *blockchain)
{
	if (!blockchain)
		return;

	llist_destroy(blockchain->chain, 1, (node_dtor_t)block_destroy);
	free(blockchain);
}
