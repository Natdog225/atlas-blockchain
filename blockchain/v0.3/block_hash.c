#include "blockchain.h"

/**
 * block_hash - computes the hash of a Block
 * @block:    pointer to the Block to be hashed
 * @hash_buf: buffer to store the computed hash
 *
 * Return: pointer to @hash_buf
 */
uint8_t *block_hash(block_t const *block,
					uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	size_t len_to_hash, tx_hashes_len = 0;
	int8_t *ptr, *tx_hashes_buf = NULL;
	int i, num_tx;

	if (!block)
		return (NULL);

	num_tx = llist_size(block->transactions);
	if (num_tx > 0)
	{
		tx_hashes_len = num_tx * SHA256_DIGEST_LENGTH;
		tx_hashes_buf = malloc(tx_hashes_len);
		if (!tx_hashes_buf)
			return (NULL);

		for (i = 0, ptr = tx_hashes_buf; i < num_tx; i++)
		{
			memcpy(ptr,
				   ((transaction_t *)llist_get_node_at(block->transactions, i))->id,
				   SHA256_DIGEST_LENGTH);
			ptr += SHA256_DIGEST_LENGTH;
		}
	}

	len_to_hash = sizeof(block->info) + sizeof(block->data.len) +
				  block->data.len + tx_hashes_len;

	ptr = malloc(len_to_hash);
	if (!ptr)
	{
		free(tx_hashes_buf);
		return (NULL);
	}

	memcpy(ptr, &block->info, sizeof(block->info));
	memcpy(ptr + sizeof(block->info), &block->data.len, sizeof(block->data.len));
	memcpy(ptr + sizeof(block->info) + sizeof(block->data.len),
		   block->data.buffer, block->data.len);

	if (tx_hashes_buf)
		memcpy(ptr + sizeof(block->info) + sizeof(block->data.len) +
				   block->data.len,
			   tx_hashes_buf, tx_hashes_len);

	sha256((int8_t const *)ptr, len_to_hash, hash_buf);

	free(ptr);
	free(tx_hashes_buf);
	return (hash_buf);
}
