#include "blockchain.h"
#include "provided/endianness.h"
#include <llist.h>
#include <stdio.h>
#include <string.h>

/**
 * blockchain_serialize - Serializes a blockchain to a file.
 * @blockchain: A pointer to the blockchain to serialize.
 * @path:       The path to the file to save the blockchain to.
 *
 * Return: 0 on success, -1 on failure.
 */
int blockchain_serialize(blockchain_t const *blockchain, char const *path)
{
	FILE *f;
	uint32_t num_blocks, i;
	const char magic[] = "HBLK";
	const char version[] = "0.1";

	if (!blockchain || !path)
		return (-1);

	f = fopen(path, "wb");
	if (!f)
		return (-1);

	num_blocks = llist_size(blockchain->chain);

	fwrite(magic, 1, 4, f);
	fwrite(version, 1, 3, f);
	fwrite((uint8_t[]){_get_endianness()}, 1, 1, f);
	fwrite(&num_blocks, sizeof(num_blocks), 1, f);

	for (i = 0; i < num_blocks; ++i)
	{
		block_t *block = llist_get_node_at(blockchain->chain, i);

		fwrite(&(block->info), sizeof(block->info), 1, f);
		fwrite(&(block->data.len), sizeof(block->data.len), 1, f);
		fwrite(block->data.buffer, block->data.len, 1, f);
		fwrite(block->hash, sizeof(block->hash), 1, f);
	}
	fclose(f);
	return (0);
}
