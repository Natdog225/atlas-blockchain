#include "blockchain.h"
#include <string.h>

/**
 * find_unspent_output - Finds an unspent output matching a transaction input
 */
static int find_unspent_output(llist_node_t node, void *in)
{
	unspent_tx_out_t *utxo = node;
	tx_in_t *tx_in = in;

	if (memcmp(utxo->block_hash, tx_in->block_hash, SHA256_DIGEST_LENGTH) == 0 &&
		memcmp(utxo->tx_id, tx_in->tx_id, SHA256_DIGEST_LENGTH) == 0 &&
		memcmp(utxo->out.hash, tx_in->tx_out_hash, SHA256_DIGEST_LENGTH) == 0)
	{
		return (0); /* Match found */
	}
	return (1); /* No match */
}

/**
 * transaction_is_valid - checks whether a transaction is valid
 * @transaction: points to the transaction to verify
 * @all_unspent: is the list of all unspent transaction outputs
 *
 * Return: 1 if the transaction is valid, and 0 otherwise
 */
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

	if (!transaction || !all_unspent)
		return (0);

	/* 1. Verify computed hash matches stored hash */
	if (!transaction_hash(transaction, hash_buf) ||
		memcmp(hash_buf, transaction->id, SHA256_DIGEST_LENGTH) != 0)
		return (0);

	num_inputs = llist_size(transaction->inputs);
	num_outputs = llist_size(transaction->outputs);

	if (num_inputs == 0) /* Regular tx must have inputs */
		return (0);

	for (i = 0; i < num_inputs; i++)
	{
		in = llist_get_node_at(transaction->inputs, i);
		/* 2. Check if input refers to a valid UTXO */
		utxo = llist_find_node(all_unspent, find_unspent_output, in);
		if (!utxo)
			return (0);

		/* 3. Verify signature */
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
		total_out += out->amount;
	}

	/* 4. Verify total in equals total out */
	if (total_in != total_out)
		return (0);

	return (1);
}
