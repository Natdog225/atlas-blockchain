#include "blockchain.h"
#include <string.h> /* For memcmp */
#include <stdio.h>	/* For fprintf, stderr */

/**
 * tx_in_sign - signs a transaction input, given the transaction id it is from
 * @in: points to the transaction input structure to sign
 * @tx_id: contains the ID (hash) of the transaction the transaction input
 * to sign is stored in
 * @sender: contains the private key of the receiver of the coins
 * @all_unspent: the list of all unspent transaction outputs to date
 *
 * Return: resulting signature struct, or NULL on failure
 */
sig_t *tx_in_sign(
	tx_in_t *in,
	uint8_t const tx_id[SHA256_DIGEST_LENGTH],
	EC_KEY const *sender,
	llist_t *all_unspent)
{
	uint8_t pub_from_sender[EC_PUB_LEN] = {0};
	unspent_tx_out_t *unspent = NULL, *target_unspent = NULL;
	int i;

	if (!in || !tx_id || !sender || !all_unspent)
	{
		fprintf(stderr, "!param\n");
		return (NULL);
	}

	/* verification */

	for (i = 0; i < llist_size(all_unspent); i++) /* find unspent transaction */
	{
		unspent = (unspent_tx_out_t *)llist_get_node_at(all_unspent, i);
		if (memcmp(in->tx_out_hash, unspent->out.hash, SHA256_DIGEST_LENGTH) == 0)
		{
			target_unspent = unspent;
			break;
		}
	}

	if (!target_unspent)
	{
		fprintf(stderr, "No target_unspent\n");
		return (NULL);
	}

	ec_to_pub(sender, pub_from_sender);

	if (memcmp(pub_from_sender, target_unspent->
