#include "blockchain.h"
#include <string.h>

/* A static constant representing the Genesis Block for validation purposes */
static const block_t GENESIS_BLOCK = {
	{
		/* info */
		0,			/* index */
		0,			/* difficulty */
		1537578000, /* timestamp */
		0,			/* nonce */
		{0}			/* prev_hash[32] */
	},
	{
		/* data */
		"Holberton School", /* buffer */
		16					/* len */
	},						/* hash */
	"\xc5\x2c\x26\xc8\xb5\x46\x16\x39"
	"\x63\x5d\x8e\xdf\x2a\x97\xd4\x8d"
	"\x0c\x8e\x00\x09\xc8\x17\xf2\xb1"
	"\xd3\xd7\xff\x2f\x04\x51\x58\x03"};

/**
 * block_is_valid - Verifies that a block is valid.
 * @block:      Pointer to the block to check.
 * @prev_block: Pointer to the previous block in the chain, or NULL
 *
 * Return: 0 if the block is valid, otherwise -1.
 */
int block_is_valid(block_t const *block, block_t const *prev_block)
{
	uint8_t hash_buf[SHA256_DIGEST_LENGTH];

	if (!block)
		return (-1);

	if (block->info.index == 0)
	{
		if (prev_block != NULL)
			return (-1);
		if (memcmp(block, &GENESIS_BLOCK, sizeof(block_t)) != 0)
			return (-1);
		return (0);
	}
	if (!prev_block ||
		block->info.index != prev_block->info.index + 1 ||
		!block_hash(prev_block, hash_buf) ||
		memcmp(hash_buf, block->info.prev_hash, SHA256_DIGEST_LENGTH) != 0 ||
		!block_hash(block, hash_buf) ||
		memcmp(hash_buf, block->hash, SHA256_DIGEST_LENGTH) != 0 ||
		block->data.len > BLOCKCHAIN_DATA_MAX)
		return (-1);

	return (0);
}
