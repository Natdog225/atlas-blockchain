#include "blockchain.h"

void blockchain_destroy(blockchain_t *blockchain)
{
	if (!blockchain)
		return;

	llist_destroy(blockchain->chain, 1, (void (*)(void *))block_destroy);
	
	llist_destroy(blockchain->unspent, 1, free);

	free(blockchain);
}