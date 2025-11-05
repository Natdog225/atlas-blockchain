#include "blockchain.h"
#include <string.h>

/**
 * tx_out_hash - Computes the hash of a transaction output
 * @out:    Pointer to the transaction output to hash
 * @hash_buf: Buffer to store the computed hash
 *
 * Return: Pointer to hash_buf
 */
static uint8_t *tx_out_hash(tx_out_t const *out,
							uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	uint8_t buf[sizeof(out->amount) + sizeof(out->pub)];

	memcpy(buf, &out->amount, sizeof(out->amount));
	memcpy(buf + sizeof(out->amount), out->pub, sizeof(out->pub));

	return (sha256((int8_t const *)buf, sizeof(buf), hash_buf));
}

/**
 * tx_out_create - allocates and initializes a transaction output structure
 * @amount: the amount of the transaction
 * @pub:    the public key of the transaction receiver
 *
 * Return: a pointer to the created transaction output upon success,
 * or NULL upon failure
 */
tx_out_t *tx_out_create(uint32_t amount, uint8_t const pub[EC_PUB_LEN])
{
	tx_out_t *out;

	if (!pub)
		return (NULL);

	out = calloc(1, sizeof(*out));
	if (!out)
		return (NULL);

	out->amount = amount;
	memcpy(out->pub, pub, EC_PUB_LEN);

	if (!tx_out_hash(out, out->hash))
	{
		free(out);
		return (NULL);
	}

	return (out);
}
