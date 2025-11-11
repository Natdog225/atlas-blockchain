#include "blockchain.h"
#include <string.h>

/**
 * select_utxos - Selects unspent outputs for a new transaction
 * @sender_pub: Sender's public key
 * @amount:     Amount to send
 * @all_unspent: List of all unspent transaction outputs
 * @selected_utxos: List to store the selected unspent outputs
 *
 * Return: Total amount of selected UTXOs, or 0 on failure
 */
static uint32_t select_utxos(uint8_t const sender_pub[EC_PUB_LEN],
							 uint32_t amount, llist_t *all_unspent,
							 llist_t *selected_utxos)
{
	uint32_t total_in = 0;
	int i, num_unspent;
	unspent_tx_out_t *utxo;

	num_unspent = llist_size(all_unspent);
	for (i = 0; i < num_unspent; i++)
	{
		utxo = llist_get_node_at(all_unspent, i);
		if (utxo && memcmp(utxo->out.pub, sender_pub, EC_PUB_LEN) == 0)
		{
			if (llist_add_node(selected_utxos, utxo, ADD_NODE_REAR) != 0)
				return (0); /* Allocation failure */
			total_in += utxo->out.amount;
			if (total_in >= amount)
				break;
		}
	}

	if (total_in < amount)
		return (0); /* Insufficient funds */

	return (total_in);
}

/**
 * transaction_create - creates a transaction
 * @sender:      contains the private key of the transaction sender
 * @receiver:    contains the public key of the transaction receiver
 * @amount:      is the amount to send
 * @all_unspent: is the list of all the unspent outputs to date
 *
 * Return: a pointer to the created transaction upon success, or NULL
 */
transaction_t *transaction_create(EC_KEY const *sender, EC_KEY const *receiver,
								  uint32_t amount, llist_t *all_unspent)
{
	transaction_t *tx;
	tx_out_t *out_receiver, *out_change;
	unspent_tx_out_t *utxo;
	tx_in_t *in;
	uint8_t sender_pub[EC_PUB_LEN], receiver_pub[EC_PUB_LEN];
	llist_t *selected_utxos;
	uint32_t total_in, i;

	if (!sender || !receiver || !all_unspent || !ec_to_pub(sender, sender_pub) ||
		!ec_to_pub(receiver, receiver_pub))
		return (NULL);
	tx = calloc(1, sizeof(*tx));
	if (!tx)
		return (NULL);
	tx->inputs = llist_create(MT_SUPPORT_FALSE);
	tx->outputs = llist_create(MT_SUPPORT_FALSE);
	selected_utxos = llist_create(MT_SUPPORT_FALSE);
	if (!tx->inputs || !tx->outputs || !selected_utxos)
	{ /* Cleanup on list creation failure */
		transaction_destroy(tx);
		llist_destroy(selected_utxos, 0, NULL);
		return (NULL);
	}
	total_in = select_utxos(sender_pub, amount, all_unspent, selected_utxos);
	if (total_in == 0) /* Insufficient funds or error */
	{
		transaction_destroy(tx);
		llist_destroy(selected_utxos, 0, NULL);
		return (NULL);
	}
	/* Create inputs from selected UTXOs */
	for (i = 0; i < (uint32_t)llist_size(selected_utxos); i++)
	{
		utxo = llist_get_node_at(selected_utxos, i);
		in = tx_in_create(utxo);
		if (!in || llist_add_node(tx->inputs, in, ADD_NODE_REAR) != 0)
		{
			transaction_destroy(tx);
			llist_destroy(selected_utxos, 0, NULL);
			return (NULL);
		}
	}
	llist_destroy(selected_utxos, 0, NULL);

	/* Create receiver output */
	out_receiver = tx_out_create(amount, receiver_pub);
	if (!out_receiver || llist_add_node(tx->outputs, out_receiver, ADD_NODE_REAR))
	{
		transaction_destroy(tx);
		return (NULL);
	}
	/* Create change output if necessary */
	if (total_in > amount)
	{
		out_change = tx_out_create(total_in - amount, sender_pub);
		if (!out_change || llist_add_node(tx->outputs, out_change, ADD_NODE_REAR))
		{
			transaction_destroy(tx);
			return (NULL);
		}
	}
	/* Hash transaction */
	if (!transaction_hash(tx, tx->id))
	{
		transaction_destroy(tx);
		return (NULL);
	}
	/* Sign inputs */
	for (i = 0; i < (uint32_t)llist_size(tx->inputs); i++)
	{
		in = llist_get_node_at(tx->inputs, i);
		if (!tx_in_sign(in, tx->id, sender, all_unspent))
		{
			transaction_destroy(tx);
			return (NULL);
		}
	}
	return (tx);
}
