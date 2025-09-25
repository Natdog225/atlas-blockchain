#include "hblk_crypto.h"

/**
 * ec_sign - signs a given set of bytes, using a given EC_KEY private key
 * @key: pointer to the EC_KEY structure containing the private key
 * @msg: pointer to the characters to be signed
 * @msglen: length of the message
 * @sig: address at which to store the signature
 *
 * Return: a pointer to the signature buffer, or NULL on failure
 */
uint8_t *ec_sign(EC_KEY const *key, uint8_t const *msg,
				 size_t msglen, sig_t *sig)
{
	unsigned int siglen_int = 0;

	if (!key || !msg || !sig)
		return (NULL);

	if (!ECDSA_sign(0, msg, msglen, sig->sig, &siglen_int, (EC_KEY *)key))
		return (NULL);

	sig->len = (uint8_t)siglen_int;
	return (sig->sig);
}
