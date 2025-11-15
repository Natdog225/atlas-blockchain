#include "blockchain.h"
#include "endianness.h"
#include <stdio.h>
#include <string.h>

/* Helper to write a tx_in */
static int _serialize_tx_in(llist_node_t node, unsigned int idx, void *arg)
{
	(void)idx;
	/* Use hardcoded size from prompt */
	fwrite(node, 1, 169, (FILE *)arg);
	return (0);
}

/* Helper to write a tx_out */
static int _serialize_tx_out(llist_node_t node, unsigned int idx, void *arg)
{
	(void)idx;
	/* Use hardcoded size from prompt */
	fwrite(node, 1, 101, (FILE *)arg);
	return (0);
}

/* Helper to write a transaction */
static int _serialize_tx(llist_node_t node, unsigned int idx, void *arg)
{
	transaction_t *tx = (transaction_t *)node;
	FILE *f = (FILE *)arg;
	uint32_t nb_inputs = llist_size(tx->inputs);
	uint32_t nb_outputs = llist_size(tx->outputs);
	(void)idx;

	fwrite(tx->id, 32, 1, f);
	fwrite(&nb_inputs, 4, 1, f);
	fwrite(&nb_outputs, 4, 1, f);

	llist_for_each(tx->inputs, _serialize_tx_in, f);
	llist_for_each(tx->outputs, _serialize_tx_out, f);
	return (0);
}

/* Helper to write a block */
static int _serialize_block(llist_node_t node, unsigned int idx, void *arg)
{
	block_t *block = (block_t *)node;
	FILE *f = (FILE *)arg;
	uint32_t nb_transactions = -1;
	(void)idx;

	fwrite(&block->info, sizeof(block->info), 1, f);
	fwrite(&block->data.len, 4, 1, f);
	fwrite(block->data.buffer, block->data.len, 1, f);
	fwrite(block->hash, 32, 1, f);

	if (block->transactions)
		nb_transactions = llist_size(block->transactions);

	fwrite(&nb_transactions, 4, 1, f);
	if (block->transactions)
		llist_for_each(block->transactions, _serialize_tx, f);

	return (0);
}

/* Helper to write an unspent tx output */
static int _serialize_unspent(llist_node_t node, unsigned int idx, void *arg)
{
	(void)idx;
	/* CORRECTED: Use hardcoded size from prompt */
	fwrite(node, 1, 165, (FILE *)arg);
	return (0);
}

int blockchain_serialize(blockchain_t const *blockchain, char const *path)
{
	FILE *f;
	uint8_t endianness;
	uint32_t nb_blocks, nb_unspent;

	if (!blockchain || !path)
		return (-1);
	f = fopen(path, "w");
	if (!f)
		return (-1);

	endianness = _get_endianness();
	nb_blocks = llist_size(blockchain->chain);
	nb_unspent = llist_size(blockchain->unspent);

	fwrite(HBLK_MAGIC, 4, 1, f);
	fwrite(HBLK_VERSION, 3, 1, f);
	fwrite(&endianness, 1, 1, f);
	fwrite(&nb_blocks, 4, 1, f);
	fwrite(&nb_unspent, 4, 1, f);

	llist_for_each(blockchain->chain, _serialize_block, f);
	llist_for_each(blockchain->unspent, _serialize_unspent, f);

	fclose(f);
	return (0);
}