#include "blockchain.h"
#include <string.h>

/**
 * is_spent - Checks if a UTXO is spent by any input in a list of transactions
 * @utxo:         The unspent output to check
 * @transactions: The list of transactions to check against
 *
 * Return: 1 if spent, 0 if not spent
 */
static int is_spent(unspent_tx_out_t const *utxo, llist_t *transactions)
{
	int i, j, tx_count, in_count;
	transaction_t *tx;
	tx_in_t *in;

	tx_count = llist_size(transactions);
	for (i = 0; i < tx_count; i++)
	{
		tx = llist_get_node_at(transactions, i);
		in_count = llist_size(tx->inputs);

		if (i > 0) /* Skip coinbase transaction's inputs */
		{
			for (j = 0; j < in_count; j++)
			{
				in = llist_get_node_at(tx->inputs, j);
				if (memcmp(in->tx_out_hash, utxo->out.hash,
						   SHA256_DIGEST_LENGTH) == 0)
				{
					return (1); /* Found, so it is spent */
				}
			}
		}
	}
	return (0); /* Not found in any input */
}

/**
 * clone_utxo - Helper to deep-copy a UTXO
 * @utxo: Source UTXO
 *
 * Return: Pointer to new UTXO, or NULL
 */
static unspent_tx_out_t *clone_utxo(unspent_tx_out_t const *utxo)
{
	unspent_tx_out_t *new_utxo;

	if (!utxo)
		return (NULL);

	new_utxo = malloc(sizeof(*new_utxo));
	if (!new_utxo)
		return (NULL);

	memcpy(new_utxo, utxo, sizeof(*new_utxo));
	return (new_utxo);
}

/**
 * update_unspent - updates the list of all unspent transaction outputs
 * @transactions: list of validated transactions
 * @block_hash:   Hash of the validated Block
 * @all_unspent:  current list of unspent transaction outputs
 *
 * Return: a new list of unspent transaction outputs
 */
llist_t *update_unspent(llist_t *transactions,
						uint8_t block_hash[SHA256_DIGEST_LENGTH],
						llist_t *all_unspent)
{
	int i, j, tx_count, out_count;
	transaction_t *tx;
	tx_out_t *out;
	unspent_tx_out_t *utxo, *cloned_utxo;
	llist_t *new_list;

	if (!transactions || !block_hash || !all_unspent)
		return (NULL);

	new_list = llist_create(MT_SUPPORT_FALSE);
	if (!new_list)
		return (NULL);

	/* 1. Copy unspent outputs from old list if they are not spent */
	for (i = 0; i < llist_size(all_unspent); i++)
	{
		utxo = llist_get_node_at(all_unspent, i);
		if (!is_spent(utxo, transactions))
		{
			cloned_utxo = clone_utxo(utxo);
			if (!cloned_utxo || llist_add_node(new_list, cloned_utxo, ADD_NODE_REAR))
			{
				llist_destroy(new_list, 1, free);
				return (NULL);
			}
		}
	}

	/* 2. Add all new outputs from the transactions */
	tx_count = llist_size(transactions);
	for (i = 0; i < tx_count; i++)
	{
		tx = llist_get_node_at(transactions, i);
		out_count = llist_size(tx->outputs);
		for (j = 0; j < out_count; j++)
		{
			out = llist_get_node_at(tx->outputs, j);
			utxo = unspent_tx_out_create(block_hash, tx->id, out);
			if (!utxo || llist_add_node(new_list, utxo, ADD_NODE_REAR) != 0)
			{
				llist_destroy(new_list, 1, free);
				return (NULL);
			}
		}
	}

	/* 3. Destroy old list and return new one */
	llist_destroy(all_unspent, 1, free);
	return (new_list);
}
