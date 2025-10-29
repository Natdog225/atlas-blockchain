#include "blockchain.h"
#include <string.h> /* for memcpy */

/**
 * block_mine - Mines a Block in order to insert it in the Blockchain
 * @block: Pointer to the Block to be mined
 *
 * Desc: increments the block's nonce until its hash
 * matches its difficulty. The valid hash is stored in block->hash.
 */
void block_mine(block_t *block)
{
	if (!block)
		return;

	/* Compute hash with current nonce first */
	block_hash(block, block->hash);

	/* Loop until the hash matches the difficulty */
	while (hash_matches_difficulty(block->hash, block->info.difficulty) == 0)
	{
		block->info.nonce++;
		block_hash(block, block->hash);
	}
}
