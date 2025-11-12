#include "blockchain.h"
#include <string.h>

/**
 * coinbase_is_valid - checks whether a coinbase transaction is valid
 * @coinbase:    points to the coinbase transaction to verify
 * @block_index: is the index of the Block
 *
 * Return: 1 if valid, and 0 otherwise
 */
int coinbase_is_valid(transaction_t const *coinbase, uint32_t block_index)
{
	uint8_t hash_buf[SHA256_DIGEST_LENGTH];
	uint8_t zero_hash[SHA256_DIGEST_LENGTH] = {0};
	tx_in_t *in;
	tx_out_t *out;

	if (!coinbase)
		return (0);

	/* 1. Verify computed hash matches stored hash */
	if (!transaction_hash(coinbase, hash_buf) ||
		memcmp(hash_buf, coinbase->id, SHA256_DIGEST_LENGTH) != 0)
		return (0);

	/* 2. Check for exactly 1 input and 1 output */
	if (llist_size(coinbase->inputs) != 1 ||
		llist_size(coinbase->outputs) != 1)
		return (0);

	in = llist_get_head(coinbase->inputs);
	out = llist_get_head(coinbase->outputs);

	/* 3. Check tx_out_hash first 4 bytes match block_index */
	if (memcmp(in->tx_out_hash, &block_index, sizeof(block_index)) != 0)
		return (0);

	/* 4. Check other input fields are zeroed */
	if (memcmp(in->block_hash, zero_hash, SHA256_DIGEST_LENGTH) != 0 ||
		memcmp(in->tx_id, zero_hash, SHA256_DIGEST_LENGTH) != 0 ||
		in->sig.len != 0)
		return (0);

	/* 5. Check output amount is COINBASE_AMOUNT */
	if (out->amount != COINBASE_AMOUNT)
		return (0);

	return (1);
}
