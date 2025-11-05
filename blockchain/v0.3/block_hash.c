#include "blockchain.h"

/**
 * block_hash - Computes the SHA256 hash of a block.
 * @block:      The block to be hashed.
 * @hash_buf:   The buffer where the resulting hash will be stored.
 * Return: A pointer to `hash_buf`, or NULL on failure.
 */
uint8_t *block_hash(block_t const *block,
					uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	size_t len;
	if (!block || !hash_buf)
		return (NULL);
	len = sizeof(block->info) + block->data.len;
	return (sha256((int8_t const *)block, len, hash_buf));
}
