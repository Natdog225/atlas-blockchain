#include "blockchain.h"
#include <string.h>

static int find_unspent_output(llist_node_t node, void *in)
{
	unspent_tx_out_t *utxo = node;
	tx_in_t *tx_in = in;

	if (memcmp(utxo->block_hash, tx_in->block_hash, SHA256_DIGEST_LENGTH) == 0 &&
		memcmp(utxo->tx_id, tx_in->tx_id, SHA256_DIGEST_LENGTH) == 0 &&
		memcmp(utxo->out.hash, tx_in->tx_out_hash, SHA256_DIGEST_LENGTH) == 0)
	{
		return (0);
	}
	return (1);
}

int transaction_is_valid(transaction_t const *transaction,
						 llist_t *all_unspent)
{
	uint8_t hash_buf[SHA256_DIGEST_LENGTH];
	int i, num_inputs, num_outputs;
	uint32_t total_in = 0, total_out = 0;
	tx_in_t *in;
	tx_out_t *out;
	unspent_tx_out_t *utxo;
	EC_KEY *temp_key;

	if (!transaction || !all_unspent || !transaction->inputs || !transaction->outputs)
		return (0);

	if (!transaction_hash(transaction, hash_buf) ||
		memcmp(hash_buf, transaction->id, SHA256_DIGEST_LENGTH) != 0)
		return (0);

	num_inputs = llist_size(transaction->inputs);
	num_outputs = llist_size(transaction->outputs);

	if (num_inputs == 0)
		return (0);

	for (i = 0; i < num_inputs; i++)
	{
		in = llist_get_node_at(transaction->inputs, i);
		if (!in) /* SEGFAULT FIX */
			return (0);
		utxo = llist_find_node(all_unspent, find_unspent_output, in);
		if (!utxo)
			return (0);
		temp_key = ec_from_pub(utxo->out.pub);
		if (!temp_key ||
			!ec_verify(temp_key, transaction->id, SHA256_DIGEST_LENGTH, &in->sig))
		{
			EC_KEY_free(temp_key);
			return (0);
		}
		EC_KEY_free(temp_key);
		total_in += utxo->out.amount;
	}

	for (i = 0; i < num_outputs; i++)
	{
		out = llist_get_node_at(transaction->outputs, i);
		if (!out) /* SEGFAULT FIX */
			return (0);
		total_out += out->amount;
	}

	if (total_in != total_out)
		return (0);

	return (1);
}
