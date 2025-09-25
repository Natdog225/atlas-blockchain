#include "hblk_crypto.h"

/**
 * ec_to_pub - extracts the public key from an EC_KEY opaque structure
 * @key: pointer to the EC_KEY structure
 * @pub: address at which to store the extracted public key
 *
 * Return: pointer to pub, or NULL on failure
 */
uint8_t *ec_to_pub(EC_KEY const *key, uint8_t pub[EC_PUB_LEN])
{
	const EC_POINT *pub_point;
	const EC_GROUP *group;

	if (!key || !pub)
		return (NULL);

	pub_point = EC_KEY_get0_public_key(key);
	if (!pub_point)
		return (NULL);

	group = EC_KEY_get0_group(key);
	if (!group)
		return (NULL);

	if (!EC_POINT_point2oct(group, pub_point,
							POINT_CONVERSION_UNCOMPRESSED, pub, EC_PUB_LEN, NULL))
		return (NULL);

	return (pub);
}
