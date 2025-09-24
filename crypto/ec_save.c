#include "hblk_crypto.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

/**
 * ec_save - saves an existing EC key pair on the disk
 * @key: pointer to the EC key pair to be saved
 * @folder: path to the folder in which to save the keys
 *
 * Return: 1 on success, 0 on failure
 */
int ec_save(EC_KEY *key, char const *folder)
{
	char path[256];
	FILE *fp;

	if (!key || !folder)
		return (0);

	if (mkdir(folder, 0700) != 0 && errno != EEXIST)
		return (0);

	sprintf(path, "%s/key_pub.pem", folder);
	fp = fopen(path, "w");
	if (!fp || !PEM_write_EC_PUBKEY(fp, key))
	{
		if (fp)
			fclose(fp);
		return (0);
	}
	fclose(fp);

	sprintf(path, "%s/key.pem", folder);
	fp = fopen(path, "w");
	if (!fp || !PEM_write_ECPrivateKey(fp, key, NULL, NULL, 0, NULL, NULL))
	{
		if (fp)
			fclose(fp);
		return (0);
	}
	fclose(fp);

	return (1);
}
