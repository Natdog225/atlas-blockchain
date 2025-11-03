#include "blockchain.h"

/**
 * tx_in_create - allocates and initializes a transaction input structure
 * @unspent: points to the unspent transaction output to be
 * converted to a transaction input
 *
 * Return: a pointer to the created transaction input upon success,
 * or NULL upon failure
 */
tx_in_t *tx_in_create(unspent_tx_out_t const *unspent)
{
	tx_in_t *in;

	if (!unspent)
		return (NULL);

	in = calloc(1, sizeof(*in));
	if (!in)
		return (NULL);

	memcpy(in->block_hash, unspent->block_hash, SHA256_DIGEST_LENGTH);
	memcpy(in->tx_id, unspent->tx_id, SHA256_DIGEST_LENGTH);
	memcpy(in->tx_out_hash, unspent->out.hash, SHA256_DIGEST_LENGTH);

	/* The signature structure is already zeroed by calloc */

	return (in);
}
