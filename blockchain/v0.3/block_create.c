#include "blockchain.h"

/**
 * block_create - creates a new Block
 * @prev:     pointer to the previous Block in the chain
 * @data:     data to store in the Block
 * @data_len: length of @data
 *
 * Return: pointer to new Block, or NULL on failure
 */
block_t *block_create(block_t const *prev, int8_t const *data,
					  uint32_t data_len)
{
	block_t *block;
	uint32_t len;

	block = calloc(1, sizeof(*block));
	if (!block)
		return (NULL);

	block->info.index = prev->info.index + 1;
	block->info.timestamp = (uint64_t)time(NULL);
	memcpy(block->info.prev_hash, prev->hash, SHA256_DIGEST_LENGTH);

	len = data_len > BLOCK_DATA_MAX ? BLOCK_DATA_MAX : data_len;
	memcpy(block->data.buffer, data, len);
	block->data.len = len;

	/* NEW LINE FOR TASK 10 */
	block->transactions = llist_create(MT_SUPPORT_FALSE);
	if (!block->transactions)
	{
		free(block);
		return (NULL);
	}

	return (block);
}