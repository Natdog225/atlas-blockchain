#include "blockchain.h"
#include <string.h>

/**
 * coinbase_create - creates a coinbase transaction
 * @receiver:    contains the public key of the miner
 * @block_index: is the index of the Block the coinbase will belong to
 *
 * Return: a pointer to the created transaction upon success, or NULL
 */
transaction_t *coinbase_create(EC_KEY const *receiver, uint32_t block_index)
{
	transaction_t *tx;
	tx_in_t *in;
	tx_out_t *out;
	uint8_t receiver_pub[EC_PUB_LEN];

	if (!receiver || !ec_to_pub(receiver, receiver_pub))
		return (NULL);

	tx = calloc(1, sizeof(*tx));
	in = calloc(1, sizeof(*in));
	if (!tx || !in)
	{
		free(tx);
		free(in);
		return (NULL);
	}

	tx->inputs = llist_create(MT_SUPPORT_FALSE);
	tx->outputs = llist_create(MT_SUPPORT_FALSE);
	if (!tx->inputs || !tx->outputs)
	{
		free(tx);
		free(in);
		return (NULL);
	}

	/* Copy block index into tx_out_hash to make input unique */
	memcpy(in->tx_out_hash, &block_index, sizeof(block_index));

	if (llist_add_node(tx->inputs, in, ADD_NODE_REAR) != 0)
	{
		transaction_destroy(tx); /* Will free in */
		return (NULL);
	}

	out = tx_out_create(COINBASE_AMOUNT, receiver_pub);
	if (!out || llist_add_node(tx->outputs, out, ADD_NODE_REAR) != 0)
	{
		transaction_destroy(tx);
		return (NULL);
	}

	if (!transaction_hash(tx, tx->id))
	{
		transaction_destroy(tx);
		return (NULL);
	}

	return (tx);
}
