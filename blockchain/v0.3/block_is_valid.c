#include "blockchain.h"
#include <string.h>

/**
 * block_is_valid - Verifies that a Block is valid
 * @block:      Pointer to the Block to check
 * @prev_block: Pointer to the previous Block in the Blockchain
 * (NULL if @block is the Genesis Block)
 *
 * Return: 0 if valid, 1 otherwise
 */
int block_is_valid(block_t const *block, block_t const *prev_block)
{
	uint8_t hash_buf[SHA256_DIGEST_LENGTH];
	int i;

	if (!block)
		return (1);

	/* 1. Check if computed hash matches the one stored in the block */
	if (!block_hash(block, hash_buf) ||
		memcmp(hash_buf, block->hash, SHA256_DIGEST_LENGTH) != 0)
		return (1);

	/* 2. (NEW v0.2) Check if hash matches the block's difficulty */
	if (hash_matches_difficulty(block->hash, block->info.difficulty) == 0)
		return (1);

	/* 3. Specific checks for Genesis vs. Non-Genesis blocks */
	if (block->info.index == 0) /* Genesis Block validation */
	{
		if (prev_block != NULL) /* Genesis should not have a prev_block */
			return (1);
		/* Check if prev_hash is all zeros */
		for (i = 0; i < SHA256_DIGEST_LENGTH; i++)
			if (block->info.prev_hash[i] != 0)
				return (1);
	}
	else /* Non-Genesis Block validation */
	{
		if (!prev_block) /* Must have a previous block */
			return (1);
		/* Check index linkage */
		if (block->info.index != prev_block->info.index + 1)
			return (1);
		/* Check prev_hash linkage */
		if (memcmp(block->info.prev_hash, prev_block->hash,
				   SHA256_DIGEST_LENGTH) != 0)
			return (1);
		/* Check data length */
		if (block->data.len > BLOCK_DATA_MAX_LEN)
			return (1);
	}

	return (0); /* Block is valid */
}
