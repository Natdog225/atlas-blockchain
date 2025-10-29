/* blockchain/v0.2/hash_matches_difficulty.c */

#include "blockchain.h"

/**
 * hash_matches_difficulty - checks whether a given hash matches a given
 * difficulty
 * @hash:       the hash to check
 * @difficulty: the minimum difficulty the hash should match
 *
 * Return: 1 if the difficulty is respected, or 0 otherwise
 */
int hash_matches_difficulty(uint8_t const hash[SHA256_DIGEST_LENGTH],
							uint32_t difficulty)
{
	uint32_t i;
	uint32_t full_bytes = difficulty / 8;
	uint8_t remaining_bits = difficulty % 8;

	/* Check full bytes that must be all zeros */
	for (i = 0; i < full_bytes; i++)
	{
		if (hash[i] != 0x00)
			return (0);
	}

	/* If no remaining bits, boom, done */
	if (remaining_bits == 0)
		return (1);

	/* Check the remaining bits in the next byte */
	if ((hash[full_bytes] >> (8 - remaining_bits)) != 0)
		return (0);

	return (1);
}
