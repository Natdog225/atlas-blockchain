#include "blockchain.h"
#include <string.h>
#include <time.h>

/**
 * block_create - Creates a new block and initializes it.
 * @prev:       A pointer to the previous block in the blockchain.
 * @data:       The data to be duplicated into the block's data buffer.
 * @data_len:   The number of bytes to copy from `data`.
 *
 * Return: A pointer to the newly allocated block, or NULL on failure.
 */
block_t *block_create(block_t const *prev,
		      int8_t const *data, uint32_t data_len)
{
	block_t *new_block;
	uint32_t len_to_copy;

	if (!prev || !data)
		return (NULL);

	new_block = calloc(1, sizeof(*new_block));
	if (!new_block)
		return (NULL);

	len_to_copy = data_len > BLOCKCHAIN_DATA_MAX ? BLOCKCHAIN_DATA_MAX : data_len;

	new_block->info.index = prev->info.index + 1;
	new_block->info.timestamp = time(NULL);
	memcpy(new_block->info.prev_hash, prev->hash, SHA256_DIGEST_LENGTH);

	memcpy(new_block->data.buffer, data, len_to_copy);
	new_block->data.len = len_to_copy;

	return (new_block);
}
