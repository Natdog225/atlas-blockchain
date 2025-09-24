#include "hblk_crypto.h"

/**
 * sha256 - computes the hash of a sequence of bytes
 * @s: The sequence of bytes to be hashed
 * @len: The number of bytes to hash in s
 * @digest: The resulting hash
 *
 * Return: a pointer to digest, or NULL if digest is NULL
 */
uint8_t *sha256(int8_t const *s, size_t len,
				uint8_t digest[SHA256_DIGEST_LENGTH])
{
	if (!digest)
		return (NULL);

	return (SHA256((const unsigned char *)s, len, digest));
}
