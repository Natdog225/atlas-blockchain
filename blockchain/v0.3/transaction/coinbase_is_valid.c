#include "blockchain.h"
#include <string.h>

int coinbase_is_valid(transaction_t const *coinbase, uint32_t block_index)
{
	uint8_t hash_buf[SHA256_DIGEST_LENGTH];
	uint8_t zero_hash[SHA256_DIGEST_LENGTH] = {0};
	tx_in_t *in;
	tx_out_t *out;

	/* SEGFAULT FIX: Check for NULL pointers before use */
	if (!coinbase || !coinbase->inputs || !coinbase->outputs)
		return (0);

	if (!transaction_hash(coinbase, hash_buf) ||
		memcmp(hash_buf, coinbase->id, SHA256_DIGEST_LENGTH) != 0)
		return (0);

	if (llist_size(coinbase->inputs) != 1 ||
		llist_size(coinbase->outputs) != 1)
		return (0);

	in = llist_get_head(coinbase->inputs);
	out = llist_get_head(coinbase->outputs);

	if (!in || !out) /* SEGFAULT FIX */
		return (0);

	if (memcmp(in->tx_out_hash, &block_index, sizeof(block_index)) != 0)
		return (0);

	if (memcmp(in->block_hash, zero_hash, SHA256_DIGEST_LENGTH) != 0 ||
		memcmp(in->tx_id, zero_hash, SHA256_DIGEST_LENGTH) != 0 ||
		in->sig.len != 0)
		return (0);

	if (out->amount != COINBASE_AMOUNT)
		return (0);

	return (1);
}
