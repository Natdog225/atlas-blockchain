#include "hblk_crypto.h"
#include <stdio.h>

/**
 * ec_load - loads an EC key pair from the disk
 * @folder: path to the folder from which to load the keys
 *
 * Return: a pointer to the created EC key pair upon success, or NULL
 */
EC_KEY *ec_load(char const *folder)
{
	char path[256];
	FILE *fp;
	EC_KEY *key = NULL;

	if (!folder)
		return (NULL);

	/* Construct the path to the private key file */
	sprintf(path, "%s/key.pem", folder);

	fp = fopen(path, "r");
	if (!fp)
		return (NULL);

	/* Read the private key, which includes public key data */
	if (!PEM_read_ECPrivateKey(fp, &key, NULL, NULL))
	{
		fclose(fp);
		return (NULL);
	}

	fclose(fp);
	return (key);
}
