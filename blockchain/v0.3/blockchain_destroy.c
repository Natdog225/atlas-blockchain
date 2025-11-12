#include "blockchain.h"

/**
 * blockchain_destroy - destroys a Blockchain
 * @blockchain: pointer to the Blockchain to destroy
 */
void blockchain_destroy(blockchain_t *blockchain)
{
	if (!blockchain)
		return;

	/* Pop the genesis block (const, not to be freed) */
	llist_pop(blockchain->chain);

	llist_destroy(blockchain->chain, 1, (void (*)(void *))block_destroy);

	/* The `unspent` list holds pointers that can be `free`d directly */
	llist_destroy(blockchain->unspent, 1, free);

	free(blockchain);
}
