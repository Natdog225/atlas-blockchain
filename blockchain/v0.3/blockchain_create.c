#include "blockchain.h"
#include <string.h>

/**
 * blockchain_create - Creates a Blockchain structure and initializes it with a
 * Genesis Block.
 *
 * Return: A pointer to the newly created blockchain, or NULL on failure.
 */
blockchain_t *blockchain_create(void)
{
	blockchain_t *blockchain;
	block_t *genesis_block;

	blockchain = calloc(1, sizeof(*blockchain));
	if (!blockchain)
		return (NULL);

	blockchain->chain = llist_create(MT_SUPPORT_FALSE);
	if (!blockchain->chain)
	{
		free(blockchain);
		return (NULL);
	}

	/* THIS IS THE VERSION THAT ALLOCATES GENESIS */
	genesis_block = calloc(1, sizeof(*genesis_block));
	if (!genesis_block)
	{
		llist_destroy(blockchain->chain, 0, NULL);
		free(blockchain);
		return (NULL);
	}

	genesis_block->info.index = 0;
	genesis_block->info.difficulty = 0;
	genesis_block->info.timestamp = 1537578000;
	genesis_block->info.nonce = 0;
	memset(genesis_block->info.prev_hash, 0, SHA256_DIGEST_LENGTH);

	memcpy(genesis_block->data.buffer, "Holberton School", 16);
	genesis_block->data.len = 16;
	genesis_block->transactions = NULL; /* Genesis has NULL tx list */

	memcpy(genesis_block->hash,
		   "\xc5\x2c\x26\xc8\xb5\x46\x16\x39"
		   "\x63\x5d\x8e\xdf\x2a\x97\xd4\x8d"
		   "\x0c\x8e\x00\x09\xc8\x17\xf2\xb1"
		   "\xd3\xd7\xff\x2f\x04\x51\x58\x03",
		   SHA256_DIGEST_LENGTH);

	if (llist_add_node(blockchain->chain, genesis_block, ADD_NODE_FRONT) != 0)
	{
		llist_destroy(blockchain->chain, 0, NULL);
		free(genesis_block);
		free(blockchain);
		return (NULL);
	}

	blockchain->unspent = llist_create(MT_SUPPORT_FALSE);
	if (!blockchain->unspent)
	{
		llist_destroy(blockchain->chain, 1, (void (*)(void *))block_destroy);
		free(blockchain);
		return (NULL);
	}

	return (blockchain);
}
