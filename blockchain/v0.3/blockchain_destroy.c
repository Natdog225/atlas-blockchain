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

	while (llist_size(blockchain->chain) > 1)
	{
		block = llist_get_node_at(blockchain->chain,
			llist_size(blockchain->chain) - 1);
		
		llist_remove_node(blockchain->chain, llist_size(blockchain->chain) - 1, 0);

		block_destroy(block);
	}

	llist_pop(blockchain->chain);

	llist_destroy(blockchain->chain, 0, NULL);
	llist_destroy(blockchain->unspent, 1, free);

	free(blockchain);
}