#include "blockchain.h"

/**
 * check_genesis - Checks if a block is the genesis block
 * @block: The block to check
 *
 * Return: 0 if valid genesis, 1 otherwise
 */
static int check_genesis(block_t const *block)
{
	uint8_t hash[SHA256_DIGEST_LENGTH] = {0};
	uint8_t g_hash[SHA256_DIGEST_LENGTH] = {
		0xc5, 0x2c, 0x26, 0xc8, 0xb5, 0x46, 0x16, 0x39,
		0x63, 0x5d, 0x8e, 0xdf, 0x2a, 0x97, 0xd4, 0x8d,
		0x0c, 0x8e, 0x00, 0x09, 0xc8, 0x17, 0xf2, 0xb1,
		0xd3, 0xd7, 0xff, 0x2f, 0x04, 0x51, 0x58, 0x03};

	if (block->info.index != 0 ||
		block->info.difficulty != 0 ||
		block->info.timestamp != 1537578000 ||
		block->info.nonce != 0 ||
		memcmp(block->info.prev_hash, hash, SHA256_DIGEST_LENGTH) != 0 ||
		block->data.len != 16 ||
		strcmp((char *)block->data.buffer, "Holberton School") != 0 ||
		block->transactions != NULL ||
		memcmp(block->hash, g_hash, SHA256_DIGEST_LENGTH) != 0)
	{
		return (1);
	}
	return (0);
}

/**
 * block_is_valid - checks whether a Block is valid
 * @block:       pointer to the Block to check
 * @prev_block:  pointer to the previous Block in the chain
 * @all_unspent: list of all unspent transaction outputs
 *
 * Return: 0 if valid, 1 otherwise
 */
int block_is_valid(block_t const *block, block_t const *prev_block,
				   llist_t *all_unspent)
{
	uint8_t hash_buf[SHA256_DIGEST_LENGTH];
	int i, num_tx;
	transaction_t *tx;

	if (!block || (!prev_block && block->info.index != 0))
		return (1);
	if (block->info.index == 0)
		return (check_genesis(block));

	if (block->info.index != prev_block->info.index + 1)
		return (1);
	if (memcmp(block->info.prev_hash, prev_block->hash, SHA256_DIGEST_LENGTH))
		return (1);
	if (block->data.len > BLOCK_DATA_MAX)
		return (1);
	if (!hash_matches_difficulty(block->hash, block->info.difficulty))
		return (1);
	if (!block_hash(block, hash_buf) ||
		memcmp(hash_buf, block->hash, SHA256_DIGEST_LENGTH))
		return (1);

	if (!block->transactions)
		return (1);
	num_tx = llist_size(block->transactions);
	if (num_tx < 1)
		return (1);

	tx = llist_get_node_at(block->transactions, 0);
	if (!coinbase_is_valid(tx, block->info.index))
		return (1);

	for (i = 1; i < num_tx; i++)
	{
		tx = llist_get_node_at(block->transactions, i);
		if (!transaction_is_valid(tx, all_unspent))
			return (1);
	}
	return (0);

}