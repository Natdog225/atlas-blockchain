#include "blockchain.h"

/**
 * transaction_hash - computes the ID (hash) of a transaction
 * @transaction: points to the transaction to compute the hash of
 * @hash_buf:    a buffer in which to store the computed hash
 * Return: a pointer to @hash_buf
 */
uint8_t *transaction_hash(transaction_t const *transaction,
							uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	size_t num_inputs, num_outputs, i;
	ssize_t buf_size;
	uint8_t *buf, *ptr;
	tx_in_t *in;
	tx_out_t *out;
	if (!transaction || !hash_buf)
		return (NULL);
	num_inputs = llist_size(transaction->inputs);
	num_outputs = llist_size(transaction->outputs);
	/* 32*3 bytes per input, 32 bytes per output */
	buf_size = (num_inputs * (SHA256_DIGEST_LENGTH * 3)) +
		(num_outputs * SHA256_DIGEST_LENGTH);
	if (buf_size == 0)
		return (NULL);
	buf = malloc(buf_size);
	if (!buf)
		return (NULL);
	ptr = buf;
	/* Copy all inputs */
	for (i = 0; i < num_inputs; i++)
	{
		in = llist_get_node_at(transaction->inputs, i);
		memcpy(ptr, in->block_hash, SHA256_DIGEST_LENGTH);
		ptr += SHA256_DIGEST_LENGTH;
		memcpy(ptr, in->tx_id, SHA256_DIGEST_LENGTH);
		ptr += SHA256_DIGEST_LENGTH;
		memcpy(ptr, in->tx_out_hash, SHA256_DIGEST_LENGTH);
		ptr += SHA256_DIGEST_LENGTH;
	}
	/* Copy all outputs */
	for (i = 0; i < num_outputs; i++)
	{
		out = llist_get_node_at(transaction->outputs, i);
		memcpy(ptr, out->hash, SHA256_DIGEST_LENGTH);
		ptr += SHA256_DIGEST_LENGTH;
	}
	sha256((int8_t const *)buf, buf_size, hash_buf);
	free(buf);
	return (hash_buf);
}
