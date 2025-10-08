#include "blockchain.h"
#include "provided/endianness.h"
#include <llist.h>
#include <stdio.h>
#include <string.h>

/**
 * blockchain_deserialize - Deserializes a blockchain from a file.
 * @path: The path to the file to load the blockchain from.
 *
 * Return: A pointer to the deserialized blockchain, or NULL on failure.
 */
blockchain_t *blockchain_deserialize(char const *path)
{
	FILE *f;
	blockchain_t *blockchain;
	uint32_t num_blocks, i;
	char magic[5] = {0}, version[4] = {0};

	if (!path)
		return (NULL);
	f = fopen(path, "rb");
	if (!f)
		return (NULL);
	fread(magic, 4, 1, f);
	fread(version, 3, 1, f);
	if (strcmp(magic, "HBLK") != 0 || strcmp(version, "0.1") != 0)
	{
		fclose(f);
		return (NULL);
	}
	fseek(f, 1, SEEK_CUR); /* Skip endianness byte */
	fread(&num_blocks, 4, 1, f);
	blockchain = calloc(1, sizeof(*blockchain));
	blockchain->chain = llist_create(MT_SUPPORT_FALSE);
	for (i = 0; i < num_blocks; ++i)
	{
		block_t *block = calloc(1, sizeof(*block));

		fread(&(block->info), sizeof(block->info), 1, f);
		fread(&(block->data.len), sizeof(block->data.len), 1, f);
		fread(block->data.buffer, block->data.len, 1, f);
		fread(block->hash, sizeof(block->hash), 1, f);
		llist_add_node(blockchain->chain, block, ADD_NODE_REAR);
	}
	fclose(f);
	return (blockchain);
}
