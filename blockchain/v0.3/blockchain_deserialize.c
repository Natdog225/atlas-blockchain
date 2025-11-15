#include "blockchain.h"
#include "endianness.h"
#include <stdio.h>
#include <string.h>

/**
 * _read_header - Reads and validates the file header
 */
static int _read_header(FILE *f, uint8_t *endianness,
						uint32_t *nb_blocks, uint32_t *nb_unspent)
{
	char magic[5] = {0};
	char version[4] = {0};

	fread(magic, 4, 1, f);
	if (strcmp(magic, HBLK_MAGIC) != 0)
		return (-1);
	fread(version, 3, 1, f);
	if (strcmp(version, HBLK_VERSION) != 0)
		return (-1);

	fread(endianness, 1, 1, f);
	fread(nb_blocks, 4, 1, f);
	fread(nb_unspent, 4, 1, f);

	if (*endianness != _get_endianness())
	{
		/* Cast pointers to (char *) */
		_swap_endian((char *)nb_blocks, sizeof(*nb_blocks));
		_swap_endian((char *)nb_unspent, sizeof(*nb_unspent));
	}
	return (0);
}

/**
 * _deserialize_tx - Reads a transaction from file
 */
static transaction_t *_deserialize_tx(FILE *f, uint8_t swap)
{
	transaction_t *tx = calloc(1, sizeof(*tx));
	tx_in_t *in;
	tx_out_t *out;
	uint32_t i, nb_inputs, nb_outputs;

	if (!tx)
		return (NULL);
	tx->inputs = llist_create(MT_SUPPORT_FALSE);
	tx->outputs = llist_create(MT_SUPPORT_FALSE);

	fread(tx->id, 32, 1, f);
	fread(&nb_inputs, 4, 1, f);
	fread(&nb_outputs, 4, 1, f);
	if (swap)
	{
		/* CORRECTED: Cast pointers to (char *) */
		_swap_endian((char *)&nb_inputs, 4);
		_swap_endian((char *)&nb_outputs, 4);
	}
	for (i = 0; i < nb_inputs; i++)
	{
		in = calloc(1, sizeof(*in));
		fread(in, sizeof(*in), 1, f);
		llist_add_node(tx->inputs, in, ADD_NODE_REAR);
	}
	for (i = 0; i < nb_outputs; i++)
	{
		out = calloc(1, sizeof(*out));
		fread(out, sizeof(*out), 1, f);
		if (swap)
			/* Cast pointer to (char *) */
			_swap_endian((char *)&out->amount, 4);
		llist_add_node(tx->outputs, out, ADD_NODE_REAR);
	}
	return (tx);
}

/**
 * _deserialize_block - Reads a block from file
 */
static block_t *_deserialize_block(FILE *f, uint8_t swap)
{
	block_t *block = calloc(1, sizeof(*block));
	uint32_t i, nb_transactions;

	if (!block)
		return (NULL);

	fread(&block->info, sizeof(block->info), 1, f);
	fread(&block->data.len, 4, 1, f);
	fread(block->data.buffer, block->data.len, 1, f);
	fread(block->hash, 32, 1, f);
	fread(&nb_transactions, 4, 1, f);

	if (swap)
	{
		/*Cast all pointers to (char *) */
		_swap_endian((char *)&block->info.index, 4);
		_swap_endian((char *)&block->info.difficulty, 4);
		_swap_endian((char *)&block->info.timestamp, 8);
		_swap_endian((char *)&block->info.nonce, 8);
		_swap_endian((char *)&block->data.len, 4);
		_swap_endian((char *)&nb_transactions, 4);
	}

	if (nb_transactions != (uint32_t)-1)
	{
		block->transactions = llist_create(MT_SUPPORT_FALSE);
		for (i = 0; i < nb_transactions; i++)
		{
			llist_add_node(block->transactions,
						   _deserialize_tx(f, swap), ADD_NODE_REAR);
		}
	}
	return (block);
}

/**
 * blockchain_deserialize - deserializes a Blockchain from a file
 * @path: path to a file to deserialize the Blockchain from
 *
 * Return: pointer to the deserialized Blockchain, or NULL on failure
 */
blockchain_t *blockchain_deserialize(char const *path)
{
	FILE *f;
	blockchain_t *blockchain;
	uint8_t endianness, swap = 0;
	uint32_t i, nb_blocks, nb_unspent;
	unspent_tx_out_t *utxo;

	if (!path)
		return (NULL);
	f = fopen(path, "r");
	if (!f)
		return (NULL);

	if (_read_header(f, &endianness, &nb_blocks, &nb_unspent) != 0)
	{
		fclose(f);
		return (NULL);
	}
	if (endianness != _get_endianness())
		swap = 1;

	blockchain = blockchain_create();
	block_destroy(llist_pop(blockchain->chain)); /* Free heap-alloc'd genesis */

	for (i = 0; i < nb_blocks; i++)
	{
		llist_add_node(blockchain->chain, _deserialize_block(f, swap),
					   ADD_NODE_REAR);
	}
	for (i = 0; i < nb_unspent; i++)
	{
		utxo = calloc(1, sizeof(*utxo));
		fread(utxo, sizeof(*utxo), 1, f);
		if (swap)
			/* Cast pointer to (char *) */
			_swap_endian((char *)&utxo->out.amount, 4);
		llist_add_node(blockchain->unspent, utxo, ADD_NODE_REAR);
	}
	fclose(f);
	return (blockchain);
}