#include "blockchain.h"
#include <string.h>

/**
 * find_unspent_output - Finds an unspent output matching a transaction input
 * @node: Pointer to the unspent_tx_out_t node
 * @in:   Pointer to the tx_in_t to match
 *
 * Return: 0 if match, 1 otherwise
 */
static int find_unspent_output(llist_node_t node, void *in)
{
	unspent_tx_out_t *utxo = node;
	tx_in_t *tx_in = in;

	/* A UTXO is uniquely identified by the hash of the output */
	if (memcmp(utxo->out.hash, tx_in->tx_out_hash, SHA256_DIGEST_LENGTH) == 0)
	{
		return (0); /* Match found */
	}
	return (1); /* No match */
}

/**
 * tx_in_sign - signs a transaction input
 * @in:          points to the transaction input structure to sign
 * @tx_id:       contains the ID (hash) of the transaction
 * @sender:      contains the private key of the receiver of the coins
 * @all_unspent: is the list of all unspent transaction outputs
 *
 * Return: a pointer to the resulting signature structure upon success,
 * or NULL upon failure
 */
sig_t *tx_in_sign(tx_in_t *in, uint8_t const tx_id[SHA256_DIGEST_LENGTH],
				  EC_KEY const *sender, llist_t *all_unspent)
{
	unspent_tx_out_t *utxo;
	uint8_t sender_pub[EC_PUB_LEN];

	if (!in || !tx_id || !sender || !all_unspent)
		return (NULL);

	/* 1. Find the unspent output this input refers to */
	utxo = llist_find_node(all_unspent, find_unspent_output, in);
	if (!utxo)
		return (NULL);

	/* 2. Verify the public key matches the sender's private key */
	if (!ec_to_pub(sender, sender_pub))
		return (NULL);

	if (memcmp(utxo->out.pub, sender_pub, EC_PUB_LEN) != 0)
		return (NULL);

	/* 3. Sign the transaction ID */
	if (!ec_sign(sender, tx_id, SHA256_DIGEST_LENGTH, &in->sig))
		return (NULL);

	return (&in->sig);
}
